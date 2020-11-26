#ifndef AST_H
#define AST_H

#include "hash.h"

#include "dataType.h"
#include "identifierType.h"

#define MAX_CHILDREN 5

enum
{
    AST_SYMBOL = 1,
    AST_SYMBOL_VECTOR,
    AST_ADD,
    AST_SUB,
    AST_MULTIPLY,
    AST_DIVIDE,
    AST_LT,
    AST_GT,
    AST_OR,
    AST_AND,
    AST_LE,
    AST_GE,
    AST_EQ,
    AST_DIF,
    AST_UNARY_MINUS,
    AST_UNARY_NEGATION,
    AST_FUNC_CALL,
    AST_LPARAMETER,
    AST_LDECL,
    AST_DECLV_NOT_VECTOR,
    AST_DECLV_VECTOR,
    AST_LVECTOR,
    AST_FUNC,
    AST_FUNC_HEADER,
    AST_FUNC_PARAMS,
    AST_LCOMMAND,
    AST_ATTRIB,
    AST_ATTRIB_VEC,
    AST_READ,
    AST_PRINT,
    AST_PRINT_PARAMS,
    AST_RETURN,
    AST_IF,
    AST_IF_ELSE,
    AST_WHILE,
    AST_LOOP
};

typedef struct ast_node
{
    int type;
    struct ast_node *child[MAX_CHILDREN];
    HASH_NODE *symbol;

    DATA_TYPE data_type;
    IDENTIFIER_TYPE id_type;

    // Helper to know in which line of the source code this AST node is, to be able
    // to print meaningful messages on semantic analysis
    int line_number;
} AST;

AST *AST_create(int type, HASH_NODE *symbol, AST *s0, AST *s1, AST *s2, AST *s3, AST *s4);
void AST_print(AST *node, int level);
AST *AST_optimize(AST *node);

#endif