#pragma once

#include <string>
#include <vector>

#include "ast.hh"

namespace ast {
    class MacroCall : public Ast {
    public:
        using UPtr = std::unique_ptr<MacroCall>;
        using MacroArg = std::vector<Ast::UPtr>;
        using MacroArgs = std::vector<MacroArg>;

        MacroCall(const parse::Location& loc, const std::string& id,
                  MacroArgs args, bool spaced = false);
        MacroCall(const MacroCall&) = delete;
        MacroCall& operator=(const MacroCall&) = delete;

        ~MacroCall() override = default;

    public:
        const std::string& get_identifier() const;
        const MacroArgs& get_args() const;
        bool get_spaced() const;

    private:
        std::string id_;
        MacroArgs args_;
        bool spaced_;
    };
} // namespace ast

#include "macro-call.hxx"
