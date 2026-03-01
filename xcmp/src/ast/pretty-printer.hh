#pragma once

#include <ostream>

#include "visitor.hh"

namespace ast {
    class PrettyPrinter : public ConstVisitor {
    public:
        PrettyPrinter(std::ostream& os);

    public:
        void operator()(const Identifier& e);
        void operator()(const MacroCall& e);
        void operator()(const MacroDef& e);
        void operator()(const Text& e);

    private:
        std::ostream& os_;
    };
} // namespace ast
