#pragma once

#include "text.hh"

namespace ast {
    inline const std::string& Text::get_text() const { return text_; }

    inline void Text::accept(ConstVisitor& visitor) const { visitor(*this); }

    inline void Text::accept(InvasiveVisitor& visitor) { visitor(*this); }
} // namespace ast
