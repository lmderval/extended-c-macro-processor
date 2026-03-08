#pragma once

#include <string>
#include <vector>

#include "ast.hh"
#include "document.hh"

namespace ast {
    class MacroCall : public Ast {
    public:
        using UPtr = std::unique_ptr<MacroCall>;
        using MacroArgs = std::vector<ast::Document::UPtr>;

        MacroCall(const parse::Location& loc, const std::string& id,
                  MacroArgs args, bool spaced = false);
        MacroCall(const MacroCall&) = delete;
        MacroCall& operator=(const MacroCall&) = delete;

        ~MacroCall() override = default;

    public:
        const std::string& get_identifier() const;
        const MacroArgs& get_args() const;
        bool get_spaced() const;

        void accept(ConstVisitor& visitor) const override;
        void accept(InvasiveVisitor& visitor) override;

    private:
        std::string id_;
        MacroArgs args_;
        bool spaced_;
    };
} // namespace ast

#include "macro-call.hxx"
