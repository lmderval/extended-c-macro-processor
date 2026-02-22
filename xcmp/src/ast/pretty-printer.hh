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
        struct pstream {
            std::ostream& os;
            int indent_level = 0;

            template <typename T>
            pstream& operator<<(const T& t);
            pstream& operator<<(std::ostream& (*fx)(std::ostream&));
            pstream& operator<<(pstream& (*fx)(pstream&));
        };

        static pstream& incindent(pstream& os);
        static pstream& indent(pstream& os);
        static pstream& decindent(pstream& os);

    private:
        pstream ps_;
    };
} // namespace ast

#include "pretty-printer.hxx"
