#include "driver.hh"

#include <fstream>

#include "lexer.hh"
#include "parser.hh"

namespace parse {
    Driver::Driver(bool scan_trace, bool parse_trace)
        : scan_trace_(scan_trace)
        , parse_trace_(parse_trace)
        , loc_()
        , document_() {}

    int Driver::parse(std::istream& is, const std::string& filename) {
        loc_.initialize(&filename);

        Lexer lexer(is, std::cout);
        lexer.set_debug(scan_trace_);

        Parser parser(lexer, *this);
        parser.set_debug_level(parse_trace_);

        return parser.parse();
    }

    int Driver::parse(const std::string& filename) {
        std::ifstream ifs(filename);
        return parse(ifs, filename);
    }
} // namespace parse
