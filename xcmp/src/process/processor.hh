#pragma once

#include <expected>
#include <string>

#include <ast/ast.hh>
#include <ast/macro-call.hh>
#include <ast/visitor.hh>
#include <misc/containers/scoped_map.hh>

namespace process {
    class Processor : public ast::ConstVisitor {
    public:
        Processor();
        ~Processor();

    public:
        std::expected<ast::Ast::UPtr, std::string> process(const ast::Ast& e);

    private:
        std::expected<ast::Ast::UPtr, std::string>
        expand(const ast::MacroDef& def, const ast::MacroCall& e);

        std::expected<ast::Ast::UPtr, std::string>
        expand(const ast::MacroDef& def, const ast::Identifier& e);

        std::expected<ast::Ast::UPtr, std::string>
        expand_as_identifier(const ast::MacroDef& def, const ast::MacroCall& e);

        std::expected<ast::Ast::UPtr, std::string>
        output_args(const ast::MacroCall& e);

    public:
        void operator()(const ast::Document& e) override;
        void operator()(const ast::Identifier& e) override;
        void operator()(const ast::MacroCall& e) override;
        void operator()(const ast::MacroDef& e) override;
        void operator()(const ast::Text& e) override;

    private:
        misc::scoped_map<std::string, const ast::MacroDef*> macros_;
        std::expected<ast::Ast::UPtr, std::string> result_;
        bool prev_expanded_ = false;
    };
} // namespace process
