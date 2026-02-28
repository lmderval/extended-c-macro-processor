#pragma once

#include "parser.hh"

#if !defined(yyFlexLexerOnce)
#    include <FlexLexer.h>
#endif

namespace parse {
    class Lexer : public yyFlexLexer {
    public:
        parse::Parser::symbol_type yylex(int dummy);

    private:
        int yylex() override { return 1; }
    };
} // namespace parse
