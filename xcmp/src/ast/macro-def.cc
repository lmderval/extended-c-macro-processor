#include "macro-def.hh"

namespace ast {
    MacroDef::MacroDef(const parse::Location& loc, const std::string& name,
                       const MacroPars& pars, Ast::UPtr body)
        : Ast(loc)
        , name_(name)
        , pars_(pars)
        , body_(std::move(body)) {}
} // namespace ast
