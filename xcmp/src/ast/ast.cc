#include "ast.hh"

namespace ast {
    Ast::Ast(const parse::Location& loc)
        : loc_(loc) {}

    Ast::~Ast() {}
} // namespace ast
