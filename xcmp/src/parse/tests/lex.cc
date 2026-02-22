#include <FlexLexer.h>

int main() {
    yyFlexLexer lexer;
    while (lexer.yylex() != 0) continue;
    return 0;
}
