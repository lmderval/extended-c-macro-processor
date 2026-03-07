#pragma once

#include <ast/document.hh>

#include "location.hh"

namespace parse {
    class Driver {
    public:
        Driver(bool scan_trace = false, bool parse_trace = false);

        friend class Parser;

    public:
        Location& get_loc();
        const ast::Document* get_document() const;

        int parse(std::istream& is, const std::string& filename);
        int parse(const std::string& filename);

    private:
        bool scan_trace_;
        bool parse_trace_;
        Location loc_;
        ast::Document::UPtr document_;
    };
} // namespace parse

#include "driver.hxx"
