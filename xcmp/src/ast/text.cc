#include "text.hh"

namespace ast {
    Text::Text(const parse::Location& loc, const std::string& text)
        : Ast(loc)
        , text_(text) {}
} // namespace ast
