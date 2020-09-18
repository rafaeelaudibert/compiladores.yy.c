#include <stdio.h>
#include <stdlib.h>

#include "lex.yy.h"
#include "decompiler.h"

// Fornecido pelo lexer em lex.yy.c
extern char *yytext;
extern FILE *yyin;
extern int yylex();

// Fornecido pelo parser y.tab.c
extern int yyparse();
extern AST *ASTroot;

void main(const int argc, const char **argv)
{
    FILE *outfile;
    initMe();

    if (argc < 3)
    {
        fprintf(stderr, "Call: %s input output\n", argv[0]);
        exit(1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL)
    {
        fprintf(stderr, "Cannot open input file %s", argv[1]);
        exit(2);
    }

    outfile = fopen(argv[2], "w");
    if (yyin == NULL)
    {
        fprintf(stderr, "Cannot open output file %s", argv[2]);
        exit(2);
    }

    yyparse();
    decompile(ASTroot, outfile);
    fprintf(stdout, "Succesful Compilation!\n");

    exit(0);
}