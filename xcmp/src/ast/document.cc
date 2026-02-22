#include "document.hh"

namespace ast {
    Document::Document(const parse::Location& loc, std::vector<Ast::UPtr> body)
        : Ast(loc)
        , body_(std::move(body)) {}
} // namespace ast
