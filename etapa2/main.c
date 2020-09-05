#include <stdio.h>
#include <stdlib.h>

#include "lex.yy.h"

// Fornecido pelo lexer em lex.yy.c
extern char *yytext;
extern FILE *yyin;
extern int yylex();

// Fornecido pelo parser y.tab.c
extern int yyparse();

void main(const int argc, const char **argv)
{
    initMe();

    if (argc < 2)
    {
        fprintf(stderr, "Call: %s input\n", argv[0]);
        exit(1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL)
    {
        fprintf(stderr, "Cannot open  file %s", argv[1]);
        exit(2);
    }

    yyparse();
    fprintf(stdout, "Succesful Compilation!\n");

    hashPrint();

    exit(0);
}