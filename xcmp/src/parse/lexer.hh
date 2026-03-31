#pragma once

#include "driver.hh"
#include "parser.hh"

#if !defined(yyFlexLexerOnce)
#    include <FlexLexer.h>
#endif

namespace parse {
    class Lexer : public yyFlexLexer {
    public:
        Lexer(std::istream& is = std::cin, std::ostream& os = std::cout);

    public:
        parse::Parser::symbol_type yylex(Driver& driver);

    private:
        int yylex() override { return 1; }
    };
} // namespace parse
