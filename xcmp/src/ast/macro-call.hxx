#pragma once

#include "macro-call.hh"

namespace ast {
    inline const std::string& MacroCall::get_identifier() const { return id_; }
    inline const MacroCall::MacroArgs& MacroCall::get_args() const {
        return args_;
    }
    inline bool MacroCall::get_spaced() const { return spaced_; }

    inline void MacroCall::accept(ConstVisitor& visitor) const {
        visitor(*this);
    }

    inline void MacroCall::accept(InvasiveVisitor& visitor) { visitor(*this); }
} // namespace ast
