#include <ast/pretty-printer.hh>
#include <parse/driver.hh>

int main() {
    parse::Driver driver(true, true);
    if (driver.parse(std::cin, "<stdin>")) {
        std::cerr << "Unable to parse file" << std::endl;
        return 1;
    }
    const ast::Document& document = *driver.get_document();
    ast::PrettyPrinter printer(std::cout);
    document.accept(printer);
}
