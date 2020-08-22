#include <stdio.h>
#include "lex.yy.h"

// Fornecido pelo lexer em lex.yy.c
extern char *yytext;
extern char *str;

int main(int argc, int *argv)
{

    int tok;
    while (isRunning() == TRUE)
    {
        tok = yylex();
        printf(
            "[%d] %d (%s)\n",
            getLineNumber(),
            tok,
            yytext);
    }

    return 0;
}