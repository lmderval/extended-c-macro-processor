#include "macro-def.hh"

namespace ast {
    MacroDef::MacroDef(const parse::Location& loc, const std::string& name,
                       const MacroPars& pars, Ast::UPtr body, bool parameter)
        : Ast(loc)
        , name_(name)
        , pars_(pars)
        , body_(std::move(body))
        , parameter_(parameter) {}
} // namespace ast
