#include "pretty-printer.hh"

#include "all.hh"

namespace ast {
    PrettyPrinter::PrettyPrinter(std::ostream& os)
        : os_(os) {}

    void PrettyPrinter::operator()(const Document& e) {
        for (const Ast::UPtr& child : e.get_body()) child->accept(*this);
    }

    void PrettyPrinter::operator()(const Identifier& e) { os_ << e.get_id(); }

    void PrettyPrinter::operator()(const MacroCall& e) {
        os_ << e.get_identifier() << "(";
        for (auto arg_it = e.get_args().cbegin(); arg_it != e.get_args().cend();
             arg_it++) {
            if (arg_it != e.get_args().cbegin()) os_ << ",";
            for (auto it = arg_it->cbegin(); it != arg_it->cend(); it++) {
                (*it)->accept(*this);
            }
        }
        os_ << ")";
    }

    void PrettyPrinter::operator()(const MacroDef&) {}

    void PrettyPrinter::operator()(const Text& e) { os_ << e.get_text(); }
} // namespace ast
