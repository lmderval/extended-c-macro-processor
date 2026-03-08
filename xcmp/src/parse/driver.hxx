#pragma once

#include "driver.hh"

namespace parse {
    inline Location& Driver::get_loc() { return loc_; }
    inline const ast::Document* Driver::get_document() const {
        return document_.get();
    }
    inline ast::Document::UPtr Driver::grab_document() {
        return std::move(document_);
    }
} // namespace parse
