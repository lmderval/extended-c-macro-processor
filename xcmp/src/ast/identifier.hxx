#pragma once

#include "identifier.hh"

namespace ast {
    inline const std::string& Identifier::get_id() const { return id_; }
} // namespace ast
