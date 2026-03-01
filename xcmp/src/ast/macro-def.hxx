#pragma once

#include "macro-def.hh"

namespace ast {
    inline const std::string& MacroDef::get_name() const { return name_; }
    inline const MacroDef::MacroPars& MacroDef::get_pars() const {
        return pars_;
    }
} // namespace ast
