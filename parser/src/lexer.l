%{
    #include "parser.h"
    #include "y.tab.h"
    #include "error.h"
    #include <iostream>

    #define YY_INPUT(buf,result,max_size)  {\
        result = get_next_char(buf, max_size); \
        if (  result <= 0  ) \
          result = YY_NULL; \
        }

    FILE *file;
    static void multiline_comment();

%}

KW          "if"|"then"|"else"|"while"|"do"|"read"|"write"|"begin"|"end"|"skip"
OP          \+|-|\*|\/|%|==|!=|>|>=|<|<=|&&|\|\||\*\*
BOOL        "true"|"false"
IDENT       [\_a-zA-Z][\_a-zA-Z0-9]*
D           [0-9]
DB          [1-9]
R           (e[\+-]{D}+|0)?
NUMBER      \-?(({DB}{D}*\.{D}+{R}|0\.{D}+{R}|\.{D}+{R})|({DB}{D}*{R})|0)
SPACE       [ ]
NEW_LINE    \n
TAB         \t
DELIMITER   \(|\)|;
COMMENT     \/\/.*$
ASSIGNMENT  :=
MULTILINE_COMMENT \(\*
ERROR       .

%%
{KW} {
    begin_token(yytext);
    if (!strcmp(yytext, "if")) return IF;
    else if (!strcmp(yytext, "then")) return THEN;
    else if (!strcmp(yytext, "else")) return ELSE;
    else if (!strcmp(yytext, "while")) return WHILE;
    else if (!strcmp(yytext, "do")) return DO;
    else if (!strcmp(yytext, "read")) return READ;
    else if (!strcmp(yytext, "write")) return WRITE;
    else if (!strcmp(yytext, "begin")) return BEGIN_;
    else if (!strcmp(yytext, "end")) return END;
    else if (!strcmp(yytext, "skip")) return SKIP;
}

{NUMBER} {
    begin_token(yytext);
    yylval.dValue = strtod(yytext, nullptr);
    return NUMBER;
}


<<EOF>> { return EOF_; }

{OP} {
    begin_token(yytext);
    if (!strcmp(yytext, "+")) return PLUS;
    else if (!strcmp(yytext, "-")) return MINUS;
    else if (!strcmp(yytext, "*")) return MUL;
    else if (!strcmp(yytext, "/")) return DIV;
    else if (!strcmp(yytext, "%")) return MOD;
    else if (!strcmp(yytext, "==")) return EQ;
    else if (!strcmp(yytext, "!=")) return NEQ;
    else if (!strcmp(yytext, ">")) return GT;
    else if (!strcmp(yytext, ">=")) return GE;
    else if (!strcmp(yytext, "<")) return LT;
    else if (!strcmp(yytext, "<=")) return LE;
    else if (!strcmp(yytext, "&&")) return AND;
    else if (!strcmp(yytext, "||")) return OR;
    else if (!strcmp(yytext, "**")) return POW;
}

{BOOL} {
    begin_token(yytext);
    if (!strcmp(yytext, "true")) return TRUE;
    else if (!strcmp(yytext, "false")) return FALSE;
}

{IDENT} {
    begin_token(yytext);
    yylval.sIndex = *yytext - 'a';
    return IDENT;
}

{SPACE} {
    begin_token(yytext);
}

{NEW_LINE} {
    begin_token(yytext);
}

{TAB} {
    begin_token(yytext);
}

{DELIMITER} {
    begin_token(yytext);
    switch (yytext[0])
    {
        case ';': return SEMICOLON;
        case '(': return LEFT_BRACKET;
        case ')': return RIGHT_BRACKET;
    }
}

{COMMENT} {
    begin_token(yytext);
}

{MULTILINE_COMMENT} {
    begin_token(yytext);
    multiline_comment();
}

{ASSIGNMENT} {
    begin_token(yytext);
    return ASSIGNMENT;
}

{ERROR} {
    exit(EXIT_FAILURE);
}

%%

static bool is_open_comment(char c1, char c2)
{
    return c1 == '(' && c2 == '*'; 
}

static bool is_close_comment(char c1, char c2)
{
    return c1 == '*' && c2 == ')'; 
}

static void multiline_comment()
{
    int balance = 1;
    int c1 = 0;
    int c2 = 0;
    while (balance > 0)
    {
        c1 = c2;
        c2 = yyinput();
        if (c2 <= 0)
        {
            exit(EXIT_FAILURE);
        }
        if (c2 == '\n')
        {
            continue;
        }
        balance += is_open_comment(c1, c2) - is_close_comment(c1, c2);
    }
}

int yywrap()
{
    return 1;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Invalid arguments" << std::endl;
        return EXIT_FAILURE;
    } 

    file = fopen(argv[1], "r");
    if (file == nullptr)
    {
        std::cerr << "Can't open file" << std::endl;
        return EXIT_FAILURE;
    }
    yyparse();
    std::cout << "EXIT_FAILURE" << std::endl;
    return EXIT_FAILURE;
}
