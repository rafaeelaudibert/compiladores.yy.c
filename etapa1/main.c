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

    printf("\n-----\n");
    hashPrint();

    printf("\n-----\n");
    printf("[hashFind] aaf: %s\n", hashFind("aaf")->text);
    printf("[hashFind] ba: %s\n", hashFind("ba")->text);
    printf("[hashFind] \"rafael\": %s\n", hashFind("\"rafael\"")->text);
    printf("[hashFind] doesnt_exist: PLEASE_DONT (Is pointer at NULL? %d) \n", hashFind("doesnt_exist") == NULL);

    printf("\n-----\n");
    freeTable();

    printf("\n-----\n");
    hashPrint();

    return 0;
}