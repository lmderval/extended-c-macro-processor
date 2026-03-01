#pragma once

#include <ast/ast.hh>
#include <vector>

#include "location.hh"

namespace parse {
    class Driver {
    public:
        Driver(bool scan_trace = false, bool parse_trace = false);

        friend class Parser;

    public:
        Location& get_loc();
        const std::vector<ast::Ast::UPtr>& get_document() const;

        int parse(std::istream& is, const std::string& filename);
        int parse(const std::string& filename);

    private:
        bool scan_trace_;
        bool parse_trace_;
        Location loc_;
        std::vector<ast::Ast::UPtr> document_;
    };
} // namespace parse

#include "driver.hxx"
