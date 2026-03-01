#pragma once

#include <memory>
#include <parse/location.hh>

#include "visitor.hh"

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

        virtual void accept(ConstVisitor& visitor) const = 0;
        virtual void accept(InvasiveVisitor& visitor) = 0;

    private:
        parse::Location loc_;
    };
} // namespace ast

#include "ast.hxx"
