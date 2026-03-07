#pragma once

#include "fwd.hh"

namespace ast {
    template <typename T>
    struct const_trait {
        using type = const T;
    };

    template <typename T>
    struct invasive_trait {
        using type = T;
    };

    template <template <typename> typename Const>
    class GenVisitor {
    public:
        template <typename T>
        using const_t = typename Const<T>::type;

    public:
        virtual void operator()(const_t<Ast>& e);
        virtual void operator()(const_t<Document>& e) = 0;
        virtual void operator()(const_t<Identifier>& e) = 0;
        virtual void operator()(const_t<MacroCall>& e) = 0;
        virtual void operator()(const_t<MacroDef>& e) = 0;
        virtual void operator()(const_t<Text>& e) = 0;
    };

    using ConstVisitor = GenVisitor<const_trait>;

    using InvasiveVisitor = GenVisitor<invasive_trait>;
} // namespace ast

#include "visitor.hxx"
