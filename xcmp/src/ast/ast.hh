#pragma once

#include <memory>
#include <parse/location.hh>

namespace ast {
    class Ast {
    public:
        using UPtr = std::unique_ptr<Ast>;

        Ast(const parse::location& loc);
        Ast(const Ast&) = delete;
        Ast& operator=(const Ast&) = delete;

        virtual ~Ast() = 0;

    public:
        const parse::location& get_loc() const;

    private:
        parse::location loc_;
    };
} // namespace ast

#include "ast.hxx"
