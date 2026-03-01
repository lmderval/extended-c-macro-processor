#include <ast/pretty-printer.hh>
#include <parse/driver.hh>

int main() {
    parse::Driver driver(true, true);
    driver.parse(std::cin, "<stdin>");
    const std::vector<ast::Ast::UPtr>& document = driver.get_document();
    ast::PrettyPrinter printer(std::cout);
    for (auto it = document.cbegin(); it != document.cend(); it++) {
        (*it)->accept(printer);
    }
}
