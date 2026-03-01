#pragma once

#include <memory>
#include <parse/location.hh>

namespace ast {
    class Ast {
    public:
        using UPtr = std::unique_ptr<Ast>;

        Ast(const parse::Location& loc);
        Ast(const Ast&) = delete;
        Ast& operator=(const Ast&) = delete;

        virtual ~Ast() = 0;

    public:
        const parse::Location& get_loc() const;

    private:
        parse::Location loc_;
    };
} // namespace ast

#include "ast.hxx"
