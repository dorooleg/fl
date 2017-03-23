%{
    #include <ctype.h> 
    int pos = 0;
    int lineno = 0;
    void print_token(const char* type);
%}

KW          "if"|"then"|"else"|"while"|"do"|"read"|"write"|"begin"|"end"
OP          \+|-|\*|\/|%|==|!=|>|>=|<|<=|&&|\|\|
BOOL        "true"|"false"
IDENT       [\_a-zA-Z][\_a-zA-Z0-9]* 
D           [0-9]
DB          [1-9]
R           (e[\+-]{D}+|0)?
NUMBER      \-?(({DB}{D}*\.{D}+{R}|0\.{D}+{R}|\.{D}+{R})|({DB}{D}*{R})|0)
SPACE       [ ]
NEW_LINE    \n
TAB         \t
COLON       [\(|\)|;]
COMMENT     \/\/.*$
ERROR       .

%%
{KW} {
    print_token("KW");
    pos += yyleng;
}

{NUMBER} {
    print_token("Num");
    pos += yyleng;
}

{OP} {
    print_token("Op");
    pos += yyleng;
}

{BOOL} {
    print_token("Bool");
    pos += yyleng;
}

{IDENT} {
    print_token("Ident");
    pos += yyleng;
}

{SPACE} {
    pos += yyleng;
}

{NEW_LINE} {
    lineno++;
    pos = 0;
}

{TAB} {
    pos += yyleng;
}

{COLON} {
    print_token("Colon");
    pos += yyleng;
}

{COMMENT} {
    lineno++;
    pos = 0;
}

{ERROR} {
    print_token("Error");
    exit(EXIT_FAILURE);
}

%%

void print_token(const char* type)
{
    if (!strcmp(type, "KW"))
    {
        printf("%s_%c%s(%d, %d, %lu); ", type, toupper(yytext[0]), yytext + 1, lineno, pos, pos + yyleng - 1);  
    }
    else if (!strcmp(type, "Colon"))
    {
        printf("%s(%d, %d, %lu); ", type, lineno, pos, pos + yyleng - 1);  
    }
    else if (!strcmp(type, "Ident"))
    {
        printf("%s(\"%s\", %d, %d, %lu); ", type, yytext, lineno, pos, pos + yyleng - 1);  
    }
    else if (!strcmp(type, "Op") || !strcmp(type, "Bool") || !strcmp(type, "Num"))
    {
        printf("%s(%s, %d, %d, %lu); ", type, yytext, lineno, pos, pos + yyleng - 1);  
    }
    else
    {
        printf("%s(%s, %d, %d, %lu); ", type, yytext, lineno, pos, pos + yyleng - 1);  
    }
}

int main()
{
    yylex();
}
