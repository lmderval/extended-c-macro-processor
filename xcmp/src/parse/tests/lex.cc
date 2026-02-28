#include <parse/lexer.hh>

int main() {
    parse::Lexer lexer;
    lexer.set_debug(1);
    while (lexer.yylex(0).type_get() != parse::Parser::symbol_kind::S_YYEOF)
        continue;
    return 0;
}
