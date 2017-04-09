%{
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <cstring>
#include "error.h"
#include <iostream>

#define YYERROR_VERBOSE 1

// prototypes
nodeType *opr(int oper, int nops, ...);
nodeType *id(int i);
nodeType *con(double value);
void freeNode(nodeType *p);
int ex(nodeType *p);
int yylex(void);

void yyerror(const char *s);
int sym[26];                    // symbol table 
%}

%union {
    double dValue;                 // double value
    char sIndex;                // symbol table index
    nodeType *nPtr;             // node pointer
};

%token <dValue> NUMBER
%token <sIndex> IDENT
%token EOF_
%token IF THEN ELSE WHILE DO READ WRITE BEGIN_ END SKIP TRUE FALSE
%nonassoc IFX
%nonassoc ASSIGNMENT
%nonassoc SEMICOLON
%nonassoc LEFT_BRACKET
%nonassoc RIGHT_BRACKET
%nonassoc ELSE

%left OR
%left AND
%left EQ NEQ
%left GE LE GT LT 
%left PLUS MINUS 
%left MUL DIV MOD 
%left POW
%nonassoc UMINUS

%type <nPtr> stmt expr stmt_list
%start program

%%


program:
        function EOF_ { exit(0); }
        ;

function:
        | function SEMICOLON stmt { ex($3); freeNode($3); }
        | stmt { ex($1); freeNode($1); }
        ;

stmt:
        expr { $$ = $1; }
        | WRITE LEFT_BRACKET expr RIGHT_BRACKET { $$ = opr(WRITE, 1, $3); }
        | READ LEFT_BRACKET expr RIGHT_BRACKET { $$ = opr(READ, 1, $3); }
        | IDENT ASSIGNMENT expr { $$ = opr(ASSIGNMENT, 2, id($1), $3); }
        | WHILE LEFT_BRACKET expr RIGHT_BRACKET DO stmt { $$ = opr(WHILE, 2, $3, $6); }
        | IF LEFT_BRACKET expr RIGHT_BRACKET THEN stmt %prec IFX { $$ = opr(IF, 2, $3, $6); }
        | IF LEFT_BRACKET expr RIGHT_BRACKET THEN stmt ELSE stmt { $$ = opr(IF, 3, $3, $6, $8); }
        | SKIP           { $$ = opr(SKIP, 0); }
        | BEGIN_ stmt_list END { $$ = $2; }
        ;

stmt_list:
        stmt { $$ = $1; }
        | stmt_list SEMICOLON stmt        { $$ = opr(';', 2, $1, $3); }
        ;

expr:
          NUMBER                { $$ = con($1); }
        | IDENT              { $$ = id($1); }
        | MINUS expr %prec UMINUS { $$ = opr(UMINUS, 1, $2); }
        | expr PLUS expr         { $$ = opr(PLUS, 2, $1, $3); }
        | expr MINUS expr         { $$ = opr(MINUS, 2, $1, $3); }
        | expr MUL expr         { $$ = opr(MUL, 2, $1, $3); }
        | expr DIV expr         { $$ = opr(DIV, 2, $1, $3); }
        | expr LT expr         { $$ = opr(LT, 2, $1, $3); }
        | expr GT expr         { $$ = opr(GT, 2, $1, $3); }
        | expr GE expr          { $$ = opr(GE, 2, $1, $3); }
        | expr LE expr          { $$ = opr(LE, 2, $1, $3); }
        | expr EQ expr          { $$ = opr(EQ, 2, $1, $3); }
        | expr NEQ expr          { $$ = opr(NEQ, 2, $1, $3); }
        | expr AND expr          { $$ = opr(AND, 2, $1, $3); }
        | expr OR expr          { $$ = opr(OR, 2, $1, $3); }
        | expr MOD expr          { $$ = opr(MOD, 2, $1, $3); }
        | expr POW expr          { $$ = opr(POW, 2, $1, $3); }
        | TRUE { $$ = opr(TRUE, 0); }
        | FALSE { $$ = opr(FALSE, 0);  }
        | LEFT_BRACKET expr RIGHT_BRACKET          { $$ = $2; }
        ;

%%

nodeType *con(double value) {
    nodeType *p;

    /* allocate node */
    if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeCon;
    p->con.value = value;

    return p;
}

nodeType *id(int i) {
    nodeType *p;

    /* allocate node */
    if ((p = (nodeType*)malloc(sizeof(nodeType))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeId;
    p->id.i = i;

    return p;
}

nodeType *opr(int oper, int nops, ...) {
    va_list ap;
    nodeType *p;
    int i;

    /* allocate node, extending op array */
    if ((p = (nodeType*)malloc(sizeof(nodeType) + (nops-1) * sizeof(nodeType *))) == NULL)
        yyerror("out of memory");

    /* copy information */
    p->type = typeOpr;
    p->opr.oper = oper;
    p->opr.nops = nops;
    va_start(ap, nops);
    for (i = 0; i < nops; i++)
        p->opr.op[i] = va_arg(ap, nodeType*);
    va_end(ap);
    return p;
}

void freeNode(nodeType *p) {
    int i;

    if (!p) return;
    if (p->type == typeOpr) {
        for (i = 0; i < p->opr.nops; i++)
            freeNode(p->opr.op[i]);
    }
    free (p);
}

void yyerror(const char *s) {
    print_error(const_cast<char*>(s));
    fprintf(stdout, "%s\n", s);
}
