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

%code requires{
    #include <iostream>
    #include <sstream>
    #include <string>
    #include <vector>

    namespace parse {
        class Lexer;
    } // namespace parse
}

%parse-param {parse::Lexer& lexer}
%lex-param {int dummy}

%code{
    #include "lexer.hh"

    #define yylex lexer.yylex
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

%nterm <std::string>              text
%nterm <std::vector<std::string>> args

%{
    int dummy = 0;
%}

%start input

%%
input:
    %empty
  | input NEWLINE   { std::cout << std::endl; }
  | input text      { std::cout << $2; }
  ;

text:
    TEXT                    { $$ = $1; }
  | ID                      { $$ = $1; }
  | ID "(" args ")"         {
                                auto args = $3;
                                std::ostringstream oss;
                                oss << $1 << "(";
                                for (std::size_t i = 0; i < args.size(); i++) {
                                    if (i != 0) oss << ",";
                                    oss << args[i];
                                }
                                oss << ")" << std::endl;
                                $$ = oss.str();
                            }
  ;

args:
    %empty          { $$ = std::vector<std::string>(); }
  | text            {
                        auto args = std::vector<std::string>();
                        args.emplace_back($1);
                        $$ = args;
                    }
  | args "," text   {
                        auto args = $1;
                        args.emplace_back($3);
                        $$ = args;
                    }
  ;
%%

namespace parse {
    void Parser::error (const location_type& l, const std::string& m)
    {
        std::cerr << l << ": " << m << '\n';
    }
} // namespace parse
