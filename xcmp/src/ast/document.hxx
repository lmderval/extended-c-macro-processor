#pragma once

#include "document.hh"

namespace ast {
    inline const std::vector<Ast::UPtr>& Document::get_body() const {
        return body_;
    }

    inline void Document::accept(ConstVisitor& visitor) const {
        visitor(*this);
    }

    inline void Document::accept(InvasiveVisitor& visitor) { visitor(*this); }
} // namespace ast
