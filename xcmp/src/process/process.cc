#include "process.hh"

#include <expected>

#include <ast/document.hh>
#include <ast/pretty-printer.hh>
#include <parse/driver.hh>
#include "processor.hh"

namespace process {
    static std::expected<ast::Document::UPtr, std::string>
    parse_stream(std::istream& is, const std::string& filename) {
        parse::Driver driver;
        if (driver.parse(is, filename) != 0)
            return std::unexpected("unable to parse input file: " + filename);

        ast::Document::UPtr document = driver.grab_document();
        return document;
    }

    void process(std::istream& is, std::ostream& os,
                 const std::string& filename) {
        auto document = parse_stream(is, filename);
        if (!document.has_value()) {
            std::cerr << document.error() << std::endl;
            return;
        }

        Processor processor;
        auto new_document = processor.process(**document);
        if (!new_document.has_value()) {
            std::cerr << new_document.error() << std::endl;
            return;
        }

        ast::PrettyPrinter printer(os);
        (*new_document)->accept(printer);
    }
} // namespace process
