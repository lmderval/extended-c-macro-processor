#include "macro-def.hh"

namespace ast {
    MacroDef::MacroDef(const parse::location& loc, const std::string& name,
                       const MacroPars& pars)
        : Ast(loc)
        , name_(name)
        , pars_(pars) {}
} // namespace ast
