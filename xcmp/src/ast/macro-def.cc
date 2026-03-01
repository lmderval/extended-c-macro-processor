#include "macro-def.hh"

namespace ast {
    MacroDef::MacroDef(const parse::Location& loc, const std::string& name,
                       const MacroPars& pars)
        : Ast(loc)
        , name_(name)
        , pars_(pars) {}
} // namespace ast
