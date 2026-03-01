#include <parse/parser.hh>
#include <parse/lexer.hh>

int main() {
    parse::Lexer lexer;
    parse::Parser parser(lexer);
    parser.set_debug_level(1);
    parser.parse();
}
