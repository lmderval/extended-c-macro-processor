#include <parse/lexer.hh>
#include <parse/parser.hh>

int main() {
    parse::Lexer lexer;
    parse::Parser parser(lexer);
    parser.parse();
}
