#pragma once

#include "pretty-printer.hh"

namespace ast {
    template <typename T>
    PrettyPrinter::pstream& PrettyPrinter::pstream::operator<<(const T& t) {
        os << t;
        return *this;
    }
} // namespace ast
