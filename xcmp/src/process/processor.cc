#include "processor.hh"

#include <cassert>
#include <sstream>

#include <ast/all.hh>

namespace process {
    namespace {
        ast::Text::UPtr make_begin_directive(const ast::Ast& e) {
            std::stringstream ss;
            ss << "\n#line " << e.get_loc().begin.line << " \""
               << *e.get_loc().end.filename << "\"\n";
            return std::make_unique<ast::Text>(e.get_loc(), ss.str());
        }

        ast::Text::UPtr make_end_directive(const ast::Ast& e) {
            std::stringstream ss;
            ss << "#line " << e.get_loc().end.line << " \""
               << *e.get_loc().end.filename << "\"\n";
            return std::make_unique<ast::Text>(e.get_loc(), ss.str());
        }
    } // namespace

    Processor::Processor()
        : macros_()
        , result_() {
        macros_.enter();
    }

    Processor::~Processor() {
        assert(!macros_.empty());
        macros_.leave();
        assert(macros_.empty());
    }

    std::expected<ast::Ast::UPtr, std::string>
    Processor::process(const ast::Ast& e) {
        e.accept(*this);
        return std::move(result_);
    }

    std::expected<ast::Ast::UPtr, std::string>
    Processor::expand(const ast::MacroDef& def, const ast::MacroCall& e) {
        auto& args = e.get_args();

        if (def.get_pars().size() != args.size()) {
            return std::unexpected<std::string>(
                "invalid number of arguments when calling macro: "
                + def.get_name());
        }

        std::vector<ast::Ast::UPtr> body;

        body.push_back(make_begin_directive(def.get_body()));

        macros_.enter();
        {
            std::vector<ast::MacroDef::UPtr> pars;
            for (std::size_t i = 0; i < def.get_pars().size(); i++) {
                auto arg = process(*args[i]);
                if (!arg.has_value()) return arg;
                std::string name = def.get_pars()[i];
                std::vector<ast::Ast::UPtr> body;
                body.push_back(std::move(*arg));
                body.push_back(std::make_unique<ast::Text>(e.get_loc(), "\n"));
                auto par_def = std::make_unique<ast::MacroDef>(
                    def.get_loc(), name, ast::MacroDef::MacroPars(),
                    std::make_unique<ast::Document>(e.get_loc(),
                                                    std::move(body)));
                pars.push_back(std::move(par_def));
                macros_.emplace(name, pars.back().get());
            }

            auto expanded = process(def.get_body());
            if (!expanded.has_value()) return expanded;
            body.push_back(std::move(*expanded));
        }
        macros_.leave();

        body.push_back(make_end_directive(e));

        return std::make_unique<ast::Document>(e.get_loc(), std::move(body));
    }

    std::expected<ast::Ast::UPtr, std::string>
    Processor::expand(const ast::MacroDef& def, const ast::Identifier& e) {
        if (!def.get_pars().empty()) {
            return std::unexpected<std::string>(
                "invalid number of arguments when calling macro: "
                + def.get_name());
        }

        std::vector<ast::Ast::UPtr> body;

        body.push_back(make_begin_directive(def.get_body()));

        macros_.enter();
        {
            auto expanded = process(def.get_body());
            if (!expanded.has_value()) return expanded;
            body.push_back(std::move(*expanded));
        }
        macros_.leave();

        body.push_back(make_end_directive(e));

        return std::make_unique<ast::Document>(e.get_loc(), std::move(body));
    }

    std::expected<ast::Ast::UPtr, std::string>
    Processor::expand_as_identifier(const ast::MacroDef& def,
                                    const ast::MacroCall& e) {
        std::vector<ast::Ast::UPtr> body;

        body.push_back(make_begin_directive(def.get_body()));

        macros_.enter();
        {
            auto expanded = process(def.get_body());
            if (!expanded.has_value()) return expanded;
            body.push_back(std::move(*expanded));
        }
        macros_.leave();

        body.push_back(make_end_directive(e));

        auto args = output_args(e);
        if (!args.has_value()) return args;
        body.push_back(std::move(*args));

        return std::make_unique<ast::Document>(e.get_loc(), std::move(body));
    }

    std::expected<ast::Ast::UPtr, std::string>
    Processor::output_args(const ast::MacroCall& e) {
        std::vector<ast::Ast::UPtr> body;

        if (e.get_spaced())
            body.push_back(std::make_unique<ast::Text>(e.get_loc(), " "));

        body.push_back(std::make_unique<ast::Text>(e.get_loc(), "("));
        for (auto it = e.get_args().cbegin(); it != e.get_args().cend(); it++) {
            if (it != e.get_args().cbegin())
                body.push_back(std::make_unique<ast::Text>(e.get_loc(), ","));
            auto arg = process(**it);
            if (!arg.has_value()) return arg;
            body.push_back(std::move(*arg));
        }
        body.push_back(std::make_unique<ast::Text>(e.get_loc(), ")"));

        return std::make_unique<ast::Document>(e.get_loc(), std::move(body));
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
            result_ = expand(*macros_.at(e.get_id()), e);
            return;
        }

        result_ = std::make_unique<ast::Text>(e.get_loc(), e.get_id());
    }

    void Processor::operator()(const ast::MacroCall& e) {
        if (macros_.contains(e.get_identifier())) {
            const ast::MacroDef& def = *macros_.at(e.get_identifier());
            result_ = def.get_pars().empty() ? expand_as_identifier(def, e)
                                             : expand(def, e);
            return;
        }

        std::vector<ast::Ast::UPtr> body;
        body.push_back(
            std::make_unique<ast::Text>(e.get_loc(), e.get_identifier()));

        auto args = output_args(e);
        if (!args.has_value()) {
            result_ = std::move(args);
            return;
        }
        body.push_back(std::move(*args));

        result_ = std::make_unique<ast::Document>(e.get_loc(), std::move(body));
    }

    void Processor::operator()(const ast::MacroDef& e) {
        if (macros_.top_contains(e.get_name())) {
            result_ = std::unexpected<std::string>("try to redefine macro: "
                                                   + e.get_name());
            return;
        }

        macros_.emplace(e.get_name(), &e);

        result_ = make_end_directive(e);
    }

    void Processor::operator()(const ast::Text& e) {
        result_ = std::make_unique<ast::Text>(e.get_loc(), e.get_text());
    }
} // namespace process
