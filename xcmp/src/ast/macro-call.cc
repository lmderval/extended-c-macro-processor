#include "macro-call.hh"

namespace ast {
    MacroCall::MacroCall(const parse::location& loc, const std::string& id,
                         MacroArgs args, bool spaced)
        : Ast(loc)
        , id_(std::move(id))
        , args_(std::move(args))
        , spaced_(spaced) {}
} // namespace ast
