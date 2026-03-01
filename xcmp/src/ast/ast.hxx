#pragma once

#include "ast.hh"

namespace ast {
    inline const parse::location& Ast::get_loc() const { return loc_; }
} // namespace ast
