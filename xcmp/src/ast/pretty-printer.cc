#include "pretty-printer.hh"

#include "all.hh"

namespace ast {
    PrettyPrinter::PrettyPrinter(std::ostream& os)
        : ps_(os) {}

    void PrettyPrinter::operator()(const Document& e) {
        for (const Ast::UPtr& child : e.get_body()) child->accept(*this);
    }

    void PrettyPrinter::operator()(const Identifier& e) { ps_ << e.get_id(); }

    void PrettyPrinter::operator()(const MacroCall& e) {
        ps_ << e.get_identifier() << "(";
        for (auto it = e.get_args().cbegin(); it != e.get_args().cend(); it++) {
            if (it != e.get_args().cbegin()) ps_ << ",";
            (*it)->accept(*this);
        }
        ps_ << ")";
    }

    void PrettyPrinter::operator()(const MacroDef& e) {
        ps_ << indent << incindent << "%define " << e.get_name();
        if (!e.get_pars().empty()) {
            ps_ << " (";
            for (auto it = e.get_pars().cbegin(); it != e.get_pars().cend();
                 it++) {
                if (it != e.get_pars().cbegin()) ps_ << ", ";
                ps_ << *it;
            }
            ps_ << ")";
        }
        ps_ << "\n";
        e.get_body().accept(*this);
        ps_ << decindent << indent << "%end\n";
    }

    void PrettyPrinter::operator()(const Text& e) { ps_ << e.get_text(); }

    PrettyPrinter::pstream&
    PrettyPrinter::pstream::operator<<(std::ostream& (*fx)(std::ostream&)) {
        fx(os);
        return *this;
    }

    PrettyPrinter::pstream&
    PrettyPrinter::pstream::operator<<(pstream& (*fx)(pstream&)) {
        return fx(*this);
    }

    PrettyPrinter::pstream& PrettyPrinter::incindent(pstream& ps) {
        ps.indent_level++;
        return ps;
    }

    PrettyPrinter::pstream& PrettyPrinter::indent(pstream& ps) {
        for (int i = 0; i < ps.indent_level; i++) ps << "    ";
        return ps;
    }

    PrettyPrinter::pstream& PrettyPrinter::decindent(pstream& ps) {
        if (ps.indent_level == 0)
            throw std::runtime_error("cannot decindent pstream");
        ps.indent_level--;
        return ps;
    }
} // namespace ast
