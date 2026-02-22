#pragma once

#include "identifier.hh"

namespace ast {
    inline const std::string& Identifier::get_id() const { return id_; }

    inline void Identifier::accept(ConstVisitor& visitor) const {
        visitor(*this);
    }

    inline void Identifier::accept(InvasiveVisitor& visitor) { visitor(*this); }
} // namespace ast
