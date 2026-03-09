#pragma once

#include <expected>
#include <map>
#include <string>

#include <ast/ast.hh>
#include <ast/macro-call.hh>
#include <ast/visitor.hh>

namespace process {
    class Processor : public ast::ConstVisitor {
    public:
        std::expected<ast::Ast::UPtr, std::string> process(const ast::Ast& e);

    private:
        std::expected<ast::Ast::UPtr, std::string>
        expand(const ast::MacroDef& def,
                    const ast::MacroCall::MacroArgs& args);

    public:
        void operator()(const ast::Document& e) override;
        void operator()(const ast::Identifier& e) override;
        void operator()(const ast::MacroCall& e) override;
        void operator()(const ast::MacroDef& e) override;
        void operator()(const ast::Text& e) override;

    private:
        std::map<std::string, const ast::MacroDef*> macros_;
        std::expected<ast::Ast::UPtr, std::string> result_;
    };
} // namespace process
