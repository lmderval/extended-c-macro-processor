#pragma once

#include <ostream>

#include "visitor.hh"

namespace ast {
    class PrettyPrinter : public ConstVisitor {
    public:
        PrettyPrinter(std::ostream& os);

    public:
        void operator()(const Document& e) override;
        void operator()(const Identifier& e) override;
        void operator()(const MacroCall& e) override;
        void operator()(const MacroDef& e) override;
        void operator()(const Text& e) override;

    private:
        std::ostream& os_;
    };
} // namespace ast
