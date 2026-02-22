%option noyywrap

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
    std::string str;
%}

%x DIRECTIVE
%x STRING

%%
^{SPACE}*"%define"  {
                        std::cout << "Open directive: define" << std::endl;
                        BEGIN(DIRECTIVE);
                    }
^{SPACE}*"%end"     { std::cout << "Close directive" << std::endl; }

<*>{
    "("              { std::cout << "Open parenthesis" << std::endl; }
    ")"              { std::cout << "Close parenthesis" << std::endl; }
    "{"              { std::cout << "Open brace" << std::endl; }
    "}"              { std::cout << "Close brace" << std::endl; }
    ","              { std::cout << "Comma" << std::endl; }
}

<DIRECTIVE>{
    {IDENTIFIER}    { std::cout << "Identifier: " << yytext << std::endl; }
    {NEWLINE}       {
                        std::cout << "Directive line end" << std::endl;
                        BEGIN(INITIAL);
                    }
    {SPACE}+        { std::cout << "Space" << std::endl; }
    <<EOF>>         {
                        std::cerr << "Unexpected EOF inside directive line"
                                << std::endl;
                    }
    .               {
                        std::cerr << "Unexpected character '" << yytext
                                << "' inside directive line" << std::endl;
                        BEGIN(INITIAL);
                    }
}

"\""    {
            str.clear();
            BEGIN(STRING);
        }

<STRING>{
    "\\\""      { str.append("\""); }
    "\""        {
                    std::cout << "String: '" << str << "'" << std::endl;
                    BEGIN(INITIAL);
                }
    {NEWLINE}   {
                    std::cerr << "Unexpected new line inside string"
                            << std::endl;
                    BEGIN(INITIAL);
                }
    <<EOF>>     {
                    std::cerr << "Unexpected EOF inside directive string"
                            << std::endl;
                    BEGIN(INITIAL);
                }
    .           { str.append(yytext); }
}

{IDENTIFIER}     { std::cout << "Identifier: " << yytext << std::endl; }
"\\"{NEWLINE}    { std::cout << "Line continuation" << std::endl; }
{NEWLINE}        { std::cout << "New line" << std::endl; }
^{SPACE}+        { std::cout << "Indentation" << std::endl; }
{SPACE}+         { std::cout << "Space" << std::endl; }
.                { std::cout << "Other: " << yytext << std::endl; }
%%
