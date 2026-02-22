#pragma once

#include <vector>

#include "ast.hh"

namespace ast {
    class Document : public Ast {
    public:
        using UPtr = std::unique_ptr<Document>;

        Document(const parse::Location& loc, std::vector<Ast::UPtr> body);
        Document(const Document&) = delete;
        Document& operator=(const Document&) = delete;

        ~Document() override = default;

    public:
        const std::vector<Ast::UPtr>& get_body() const;

        void accept(ConstVisitor& visitor) const override;
        void accept(InvasiveVisitor& visitor) override;

    private:
        std::vector<Ast::UPtr> body_;
    };
} // namespace ast

#include "document.hxx"
