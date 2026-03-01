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
    #include <iostream>
    #include <sstream>
    #include <string>
    #include <vector>

    #include <ast/fwd.hh>

    namespace parse {
        class Lexer;
    } // namespace parse
}

%parse-param {parse::Lexer& lexer}
%lex-param {int dummy}

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

%nterm <std::vector<ast::Ast*>*>                input
%nterm <ast::Ast*>                             text
%nterm <std::vector<ast::Ast*>*>                arg
%nterm <std::vector<ast::Ast*>*>                arg.1
%nterm <std::vector<std::vector<ast::Ast*>*>*>  args

%{
    int dummy = 0;
%}

%start input

%%
input:
    %empty          { $$ = new std::vector<ast::Ast*>(); }
  | input text      {
                        auto vec = $1;
                        vec->push_back($2);
                        $$ = vec;
                    }
  ;

text:
    TEXT                    { $$ = new ast::Text(@$, $1); }
  | ID                      { $$ = new ast::Identifier(@$, $1); }
  | NEWLINE                 { $$ = new ast::Text(@$, "\n"); }
  | ID "(" args ")"         {
                                auto vec = $3;
                                ast::MacroCall::MacroArgs args;
                                for (auto arg_it : *vec) {
                                    ast::MacroCall::MacroArg arg;
                                    for (auto it : *arg_it) {
                                        arg.emplace_back(&*it);
                                    }
                                    args.push_back(std::move(arg));
                                }
                                $$ = new ast::MacroCall(@$, $1, std::move(args));
                            }
  ;

arg:
    %empty  { $$ = new std::vector<ast::Ast*>(); }
  | arg.1   { $$ = $1; }
  ;

arg.1:
    text        {
                    auto vec = new std::vector<ast::Ast*>();
                    vec->push_back($1);
                    $$ = vec;
                }
  | arg.1 text  {
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
%%

namespace parse {
    void Parser::error (const location_type& l, const std::string& m)
    {
        std::cerr << l << ": " << m << '\n';
    }
} // namespace parse
