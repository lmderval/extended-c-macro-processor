#pragma once

#include <string>

#include "ast.hh"

namespace ast {
    class Text : public Ast {
    public:
        using UPtr = std::unique_ptr<Text>;

        Text(const parse::Location& loc, const std::string& text);
        Text(const Text&) = delete;
        Text& operator=(const Text&) = delete;

        ~Text() override = default;

    public:
        const std::string& get_text() const;

    private:
        std::string text_;
    };
} // namespace ast

#include "text.hxx"
