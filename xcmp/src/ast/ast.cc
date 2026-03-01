#include "ast.hh"

namespace ast {
    Ast::Ast(const parse::location& loc)
        : loc_(loc) {}

    Ast::~Ast() {}
} // namespace ast
