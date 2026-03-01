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

    private:
        std::string id_;
    };
} // namespace ast

#include "identifier.hxx"
