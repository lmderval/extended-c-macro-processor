#pragma once

#include <vector>

#include "ast.hh"

namespace ast {
    class MacroDef : public Ast {
    public:
        using UPtr = std::unique_ptr<MacroDef>;
        using MacroPars = std::vector<std::string>;

        MacroDef(const parse::location& loc, const std::string& name,
                 const MacroPars& pars);
        MacroDef(const MacroDef&) = delete;
        MacroDef& operator=(const MacroDef&) = delete;

        ~MacroDef() override = default;

    public:
        const std::string& get_name() const;
        const MacroPars& get_pars() const;

    private:
        std::string name_;
        MacroPars pars_;
    };
} // namespace ast

#include "macro-def.hxx"
