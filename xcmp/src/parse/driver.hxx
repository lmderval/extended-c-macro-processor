#pragma once

#include "driver.hh"

namespace parse {
    inline Location& Driver::get_loc() { return loc_; }
    inline const std::vector<ast::Ast::UPtr>& Driver::get_document() const {
        return document_;
    }
} // namespace parse
