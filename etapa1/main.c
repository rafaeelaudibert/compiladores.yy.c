#include <stdio.h>
#include "lex.yy.h"

// Fornecido pelo lexer em lex.yy.c
extern char *yytext;

int main(int argc, int *argv)
{
    initMe();

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