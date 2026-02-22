#pragma once

#include <string>

#include "ast.hh"

namespace ast {
    class Identifier : public Ast {
    public:
        using UPtr = std::unique_ptr<Identifier>;

        Identifier(const parse::Location& loc, const std::string& id);
        Identifier(const Identifier&) = delete;
        Identifier& operator=(const Identifier&) = delete;

        ~Identifier() override = default;

    public:
        const std::string& get_id() const;

        void accept(ConstVisitor& visitor) const override;
        void accept(InvasiveVisitor& visitor) override;

    private:
        std::string id_;
    };
} // namespace ast

#include "identifier.hxx"
