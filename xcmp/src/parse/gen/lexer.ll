%option noyywrap

%option c++

%option outfile="lex.yy.c"

%top{
    #include <algorithm>
    #include <cctype>
    #include <iostream>
    #include <string>
}

IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*
NEWLINE (\n|\r|\r\n|\n\r)
SPACE [ \t]

%{
    #include <parse/lexer.hh>

    #include "parser.hh"

    #undef YY_DECL
    #define YY_DECL                                                            \
        parse::Parser::symbol_type                                             \
        parse::Lexer::yylex([[maybe_unused]] int dummy)

    std::string str;
    parse::location loc;

    #define YY_USER_ACTION loc.columns (yyleng);
%}

%option yyclass="parse::Lexer"

%option debug

%x DIRECTIVE
%x STRING

%%
^{SPACE}*"%define"  {
                        BEGIN(DIRECTIVE);
                        return parse::Parser::make_DEFINE_DIRECTIVE(loc);
                    }
^{SPACE}*"%end"     { return parse::Parser::make_END_DIRECTIVE(loc); }

<*>{
    "("             { return parse::Parser::make_LPAREN(loc); }
    ")"             { return parse::Parser::make_RPAREN(loc); }
    "{"             { return parse::Parser::make_LBRACE(loc); }
    "}"             { return parse::Parser::make_RBRACE(loc); }
    ","             { return parse::Parser::make_COMMA(loc); }
}

<DIRECTIVE>{
    {IDENTIFIER}    { return parse::Parser::make_ID(yytext, loc); }
    {NEWLINE}       {
                        loc.lines();
                        loc.step();
                        BEGIN(INITIAL);
                        return parse::Parser::make_NEWLINE(loc);
                    }
    {SPACE}+        { loc.step(); }
    <<EOF>>         {
                        throw parse::Parser::syntax_error(
                                loc, "Unexpected EOF inside directive line");
                    }
    .               {
                        throw parse::Parser::syntax_error(loc,
                                "Unexpected character " + std::string(yytext) +
                                    " inside directive line");
                    }
}

"\""    {
            str.clear();
            BEGIN(STRING);
        }

<STRING>{
    "\\\""      { str.append("\""); }
    "\""        {
                    BEGIN(INITIAL);
                    return parse::Parser::make_STRING(str, loc);
                }
    {NEWLINE}   {
                    loc.lines();
                    loc.step();
                    throw parse::Parser::syntax_error(
                            loc, "Unexpected new line inside string");
                }
    <<EOF>>     {
                    throw parse::Parser::syntax_error(
                            loc, "Unexpected EOF inside string");
                }
    .           { str.append(yytext); }
}

{IDENTIFIER}    { return parse::Parser::make_ID(yytext, loc); }
"\\"{NEWLINE}   {
                    loc.lines();
                    loc.step();
                }
{NEWLINE}       {
                    loc.lines();
                    loc.step();
                    return parse::Parser::make_NEWLINE(loc);
                }
^{SPACE}+       { return parse::Parser::make_TEXT(yytext, loc); }
{SPACE}+        { return parse::Parser::make_TEXT(" ", loc); }
.               { return parse::Parser::make_TEXT(yytext, loc); }
<<EOF>>         { return parse::Parser::make_YYEOF(loc); }
%%
