#pragma once

#include <vector>

#include "ast.hh"

namespace ast {
    class MacroDef : public Ast {
    public:
        using UPtr = std::unique_ptr<MacroDef>;
        using MacroPars = std::vector<std::string>;

        MacroDef(const parse::Location& loc, const std::string& name,
                 const MacroPars& pars, Ast::UPtr body);
        MacroDef(const MacroDef&) = delete;
        MacroDef& operator=(const MacroDef&) = delete;

        ~MacroDef() override = default;

    public:
        const std::string& get_name() const;
        const MacroPars& get_pars() const;
        const Ast& get_body() const;

        void accept(ConstVisitor& visitor) const override;
        void accept(InvasiveVisitor& visitor) override;

    private:
        std::string name_;
        MacroPars pars_;
        Ast::UPtr body_;
    };
} // namespace ast

#include "macro-def.hxx"
