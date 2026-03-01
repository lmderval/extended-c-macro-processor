#include "identifier.hh"

namespace ast {
    Identifier::Identifier(const parse::location& loc,
                                 const std::string& id)
        : Ast(loc)
        , id_(id) {}
} // namespace ast
