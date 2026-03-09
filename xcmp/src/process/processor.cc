#include "processor.hh"

#include <sstream>

#include <ast/all.hh>

namespace process {
    std::expected<ast::Ast::UPtr, std::string>
    Processor::process(const ast::Ast& e) {
        e.accept(*this);
        return std::move(result_);
    }

    std::expected<ast::Ast::UPtr, std::string>
    Processor::expand(const ast::MacroDef& def,
                      const ast::MacroCall::MacroArgs& args) {
        if (def.get_pars().size() != args.size()) {
            return std::unexpected<std::string>(
                "invalid number of arguments when calling macro: "
                + def.get_name());
        }

        std::vector<ast::MacroDef::UPtr> pars;
        for (std::size_t i = 0; i < def.get_pars().size(); i++) {
            auto arg = process(*args[i]);
            if (!arg.has_value()) return arg;
            std::string name = def.get_pars()[i];
            auto par_def = std::make_unique<ast::MacroDef>(
                def.get_loc(), name, ast::MacroDef::MacroPars(),
                std::move(*arg));
            pars.push_back(std::move(par_def));
            macros_.emplace(name, pars.back().get());
        }

        auto body = process(def.get_body());
        return body;
    }

    void Processor::operator()(const ast::Document& e) {
        std::vector<ast::Ast::UPtr> body;
        for (const ast::Ast::UPtr& child : e.get_body()) {
            auto new_child = process(*child);
            if (!new_child.has_value()) {
                result_ = std::move(new_child);
                return;
            }
            body.push_back(std::move(*new_child));
        }
        result_ = std::make_unique<ast::Document>(e.get_loc(), std::move(body));
    }

    void Processor::operator()(const ast::Identifier& e) {
        if (macros_.contains(e.get_id())) {
            result_ = expand(*macros_[e.get_id()], ast::MacroCall::MacroArgs());
            return;
        }

        result_ = std::make_unique<ast::Text>(e.get_loc(), e.get_id());
    }

    void Processor::operator()(const ast::MacroCall& e) {
        if (macros_.contains(e.get_identifier())) {
            result_ = expand(*macros_[e.get_identifier()], e.get_args());
            return;
        }

        std::vector<ast::Ast::UPtr> body;
        body.push_back(
            std::make_unique<ast::Text>(e.get_loc(), e.get_identifier()));

        if (e.get_spaced())
            body.push_back(std::make_unique<ast::Text>(e.get_loc(), " "));

        body.push_back(std::make_unique<ast::Text>(e.get_loc(), "("));
        for (auto it = e.get_args().cbegin(); it != e.get_args().cend(); it++) {
            if (it != e.get_args().cbegin())
                body.push_back(std::make_unique<ast::Text>(e.get_loc(), ","));
            auto arg = process(**it);
            if (!arg.has_value()) {
                result_ = std::move(arg);
                return;
            }
            body.push_back(std::move(*arg));
        }
        body.push_back(std::make_unique<ast::Text>(e.get_loc(), ")"));
        result_ = std::make_unique<ast::Document>(e.get_loc(), std::move(body));
    }

    void Processor::operator()(const ast::MacroDef& e) {
        if (macros_.contains(e.get_name())) {
            result_ = std::unexpected<std::string>("try to redefine macro: "
                                                   + e.get_name());
            return;
        }

        macros_.emplace(e.get_name(), &e);

        std::stringstream ss;
        ss << "#line " << e.get_loc().end.line << " \""
           << *e.get_loc().end.filename << "\"\n";
        result_ = std::make_unique<ast::Text>(e.get_loc(), ss.str());
    }

    void Processor::operator()(const ast::Text& e) {
        result_ = std::make_unique<ast::Text>(e.get_loc(), e.get_text());
    }
} // namespace process
