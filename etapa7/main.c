#include <stdio.h>
#include <stdlib.h>

#include "lex.yy.h"
#include "status.h"
#include "semanticParser.h"
#include "chainedList.h"
#include "codegen.h"
#include "tac.h"
#include "asmGeneration.h"

// Fornecido pelo lexer em lex.yy.c
extern char *yytext;
extern FILE *yyin;
extern int yylex();
extern int syntax_error;

// Fornecido pelo parser y.tab.c
extern int yyparse();
extern AST *ASTroot;

int main(const int argc, const char **argv)
{
    initMe();

    if (argc < 2)
    {
        fprintf(stderr, "Call: %s input\n", argv[0]);
        exit(STATUS_EXIT_NO_INPUT_FILE);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL)
    {
        fprintf(stderr, "Cannot open input file %s", argv[1]);
        exit(STATUS_EXIT_INVALID_INPUT_FILE);
    }

    // LEXER AND PARSER
    yyparse();

    // SEMANTIC ANALYSIS
    ChainedList *semantic_errors = get_semantic_errors(ASTroot);

    if (semantic_errors)
    {
        fprintf(stdout, "Found some semantic errors!!\n\n");
        do
        {
            fprintf(stdout, "%s\n", (char *)semantic_errors->val);
        } while ((semantic_errors = semantic_errors->next));

        exit(STATUS_EXIT_SEMANTIC_ERROR);
    }

    if (syntax_error)
    {
        fprintf(stdout, "Found syntax errors, exiting...");
        exit(STATUS_EXIT_SYNTAX_ERROR);
    }

    // Optimize AST
    ASTroot = AST_optimize(ASTroot);

    // Gera os TACS
    TAC *tac = generate_code(ASTroot);
    TAC *tac_reversed = tac_reverse(tac);

    // Gera assembly
    generate_ASM(tac_reversed);

    fprintf(stdout, "Succesful Compilation!\n");
    exit(STATUS_EXIT_SUCCESS);
}