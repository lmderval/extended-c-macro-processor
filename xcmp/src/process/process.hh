#pragma once

#include <iostream>
#include <string>

namespace process {
    void process(std::istream& is, std::ostream& os,
                 const std::string& filename);
} // namespace process
