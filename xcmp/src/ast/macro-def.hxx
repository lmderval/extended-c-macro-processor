#pragma once

#include "macro-def.hh"

namespace ast {
    inline const std::string& MacroDef::get_name() const { return name_; }
    inline const MacroDef::MacroPars& MacroDef::get_pars() const {
        return pars_;
    }
    inline const Ast& MacroDef::get_body() const { return *body_; }
    inline bool MacroDef::is_parameter() const { return parameter_; }

    inline void MacroDef::accept(ConstVisitor& visitor) const {
        visitor(*this);
    }

    inline void MacroDef::accept(InvasiveVisitor& visitor) { visitor(*this); }
} // namespace ast
