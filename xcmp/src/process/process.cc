#include "process.hh"

#include <cstddef>
#include <string>

namespace process {
    void process(std::istream& is, std::ostream& os) {
        std::size_t line_number = 1;
        for (std::string line; std::getline(is, line);) {
            os << line_number << '\t' << line << std::endl;
            line_number++;
        }
    }
} // namespace process
