#include <parse/driver.hh>
#include <parse/lexer.hh>

int main() {
    parse::Driver driver;
    parse::Lexer lexer;
    lexer.set_debug(1);
    while (lexer.yylex(driver).type_get()
           != parse::Parser::symbol_kind::S_YYEOF)
        continue;
    return 0;
}
