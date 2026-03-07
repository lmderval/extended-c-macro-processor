%require "3.8.1"

%language "c++"

%skeleton "lalr1.cc"

%header
%locations

%define api.namespace {parse}
%define api.parser.class {Parser}
%define api.value.type variant
%define api.token.constructor
%define api.token.prefix {TOK_}

%define parse.trace

%code requires{
    #include <parse/driver.hh>
    #include <iostream>
    #include <sstream>
    #include <string>
    #include <vector>

    #include <ast/fwd.hh>
    #include <ast/macro-def.hh>

    namespace parse {
        class Lexer;
    } // namespace parse
}

%parse-param {parse::Lexer& lexer}
%parse-param {parse::Driver& driver}
%lex-param {parse::Driver& driver}

%code{
    #include <parse/lexer.hh>

    #define yylex lexer.yylex

    #include <ast/all.hh>
}

%token DEFINE_DIRECTIVE "%define"
%token END_DIRECTIVE    "%end"

%token LPAREN "("
%token RPAREN ")"
%token LBRACE "{"
%token RBRACE "}"
%token COMMA  ","

%token <std::string> ID   "id"
%token <std::string> TEXT "text"

%token <std::string> STRING "string"

%token NEWLINE "newline"

%nterm <ast::Document*>                         document
%nterm <std::vector<ast::Ast*>*>                document.1
%nterm <ast::Ast*>                              directive
%nterm <ast::Ast*>                              text text.1
%nterm <std::vector<ast::Ast*>*>                arg  arg.1
%nterm <std::vector<std::vector<ast::Ast*>*>*>  args
%nterm <ast::MacroDef::MacroPars*>              pars

%expect 0

%precedence ID
%precedence LPAREN

%start input

%%
input: document { driver.document_ = ast::Document::UPtr($1); }

document: document.1    {
                            auto vec = $1;
                            std::vector<ast::Ast::UPtr> body;
                            for (auto it : *vec) body.emplace_back(it);
                            $$ = new ast::Document(@$, std::move(body));
                            delete vec;
                        }

document.1:
    %empty          { $$ = new std::vector<ast::Ast*>();  }
  | document.1 text {
                        auto vec = $1;
                        vec->push_back($2);
                        $$ = vec;
                    }
  ;

text:
    COMMA       { $$ = new ast::Text(@$, ","); }
  | directive   { $$ = $1; }
  | text.1      { $$ = $1; }
  ;

text.1:
    TEXT                    { $$ = new ast::Text(@$, $1); }
  | ID                      { $$ = new ast::Identifier(@$, $1); }
  | STRING                  { $$ = new ast::Text(@$, $1); }
  | NEWLINE                 { $$ = new ast::Text(@$, "\n"); }
  | "(" document.1 ")"      {
                                auto vec = $2;
                                std::vector<ast::Ast::UPtr> body;
                                body.emplace_back(new ast::Text(@1, "("));
                                for (auto it : *vec) body.emplace_back(it);
                                body.emplace_back(new ast::Text(@3, ")"));
                                $$ = new ast::Document(@$, std::move(body));
                                delete vec;
                            }
  | "{" document.1 "}"      {
                                auto vec = $2;
                                std::vector<ast::Ast::UPtr> body;
                                body.emplace_back(new ast::Text(@1, "{"));
                                for (auto it : *vec) body.emplace_back(it);
                                body.emplace_back(new ast::Text(@3, "}"));
                                $$ = new ast::Document(@$, std::move(body));
                                delete vec;
                            }
  | ID "(" args ")"         {
                                auto vec = $3;
                                ast::MacroCall::MacroArgs args;
                                for (auto arg_it : *vec) {
                                    ast::MacroCall::MacroArg arg;
                                    for (auto it : *arg_it) {
                                        arg.emplace_back(it);
                                    }
                                    args.push_back(std::move(arg));
                                    delete arg_it;
                                }
                                $$ = new ast::MacroCall(@$, $1, std::move(args));
                                delete vec;
                            }
  ;

arg:
    %empty  { $$ = new std::vector<ast::Ast*>(); }
  | arg.1   { $$ = $1; }
  ;

arg.1:
    text.1          {
                        auto vec = new std::vector<ast::Ast*>();
                        vec->push_back($1);
                        $$ = vec;
                    }
  | arg.1 text.1    {
                        auto vec = $1;
                        vec->push_back($2);
                        $$ = vec;
                    }
  ;

args:
    arg             {
                        auto vec = new std::vector<std::vector<ast::Ast*>*>();
                        vec->push_back($1);
                        $$ = vec;
                    }
  | args "," arg    {
                        auto vec = $1;
                        vec->push_back($3);
                        $$ = vec;
                    }
  ;

directive:
    "%define" ID NEWLINE document "%end" NEWLINE {
        $$ = new ast::MacroDef(@$, $2, ast::MacroDef::MacroPars(),
                               ast::Document::UPtr($4));
    }
  | "%define" ID "(" pars ")" NEWLINE document "%end" NEWLINE {
        auto pars = $4;
        $$ = new ast::MacroDef(@$, $2, *pars, ast::Document::UPtr($7));
        delete pars;
    }
  ;

pars:
    ID          {
                    auto pars = new ast::MacroDef::MacroPars();
                    pars->push_back($1);
                    $$ = pars;
                }
  | pars "," ID {
                    auto pars = $1;
                    pars->push_back($3);
                    $$ = pars;
                }
  ;
%%

namespace parse {
    void Parser::error (const location_type& l, const std::string& m) {
        std::cerr << l << ": " << m << '\n';
    }
} // namespace parse
