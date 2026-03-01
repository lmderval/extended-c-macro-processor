#pragma once

#include "visitor.hh"

namespace ast {
    template <template <typename> typename Const>
    inline void GenVisitor<Const>::operator()(const_t<Ast>& e) {
        e.accept(*this);
    }
} // namespace ast
