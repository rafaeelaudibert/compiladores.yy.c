#include <stdio.h>
#include "ast.h"

AST *AST_create(int type, HASH_NODE *symbol, AST *s0, AST *s1, AST *s2, AST *s3, AST *s4)
{
    AST *node = (AST *)calloc(1, sizeof(AST));

    node->type = type;
    node->symbol = symbol;
    node->child[0] = s0;
    node->child[1] = s1;
    node->child[2] = s2;
    node->child[3] = s3;
    node->child[4] = s4;

    return node;
}

void AST_print(AST *node, int level)
{
    if (!node)
    {
        return;
    }

    for (int i = 0; i < level; i++)
    {
        printf("  ");
    }

    switch (node->type)
    {
    case AST_SYMBOL:
        printf("AST_SYMBOL");
        break;
    case AST_SYMBOL_VECTOR:
        printf("AST_SYMBOL_VECTOR");
        break;
    case AST_ADD:
        printf("AST_ADD");
        break;
    case AST_SUB:
        printf("AST_SUB");
        break;
    case AST_MULTIPLY:
        printf("AST_MULTIPLY");
        break;
    case AST_DIVIDE:
        printf("AST_DIVIDE");
        break;
    case AST_LT:
        printf("AST_LT");
        break;
    case AST_GT:
        printf("AST_GT");
        break;
    case AST_OR:
        printf("AST_OR");
        break;
    case AST_AND:
        printf("AST_AND");
        break;
    case AST_LE:
        printf("AST_LE");
        break;
    case AST_GE:
        printf("AST_GE");
        break;
    case AST_EQ:
        printf("AST_EQ");
        break;
    case AST_DIF:
        printf("AST_DIF");
        break;
    case AST_UNARY_MINUS:
        printf("AST_UNARY_MINUS");
        break;
    case AST_UNARY_NEGATION:
        printf("AST_UNARY_NEGATION");
        break;
    case AST_FUNC_CALL:
        printf("AST_FUNC_CALL");
        break;
    case AST_LPARAMETER:
        printf("AST_LPARAMETER");
        break;
    case AST_LDECL:
        printf("AST_LDECL");
        break;
    case AST_DECLV_NOT_VECTOR:
        printf("AST_DECLV_NOT_VECTOR");
        break;
    case AST_DECLV_VECTOR:
        printf("AST_DECLV_VECTOR");
        break;
    case AST_LVECTOR:
        printf("AST_LVECTOR");
        break;
    case AST_FUNC:
        printf("AST_FUNC");
        break;
    case AST_FUNC_HEADER:
        printf("AST_FUNC_HEADER");
        break;
    case AST_FUNC_PARAMS:
        printf("AST_FUNC_PARAMS");
        break;
    case AST_LCOMMAND:
        printf("AST_LCOMMAND");
        break;
    case AST_ATTRIB:
        printf("AST_ATTRIB");
        break;
    case AST_ATTRIB_VEC:
        printf("AST_ATTRIB_VEC");
        break;
    case AST_READ:
        printf("AST_READ");
        break;
    case AST_PRINT:
        printf("AST_PRINT");
        break;
    case AST_PRINT_PARAMS:
        printf("AST_PRINT_PARAMS");
        break;
    case AST_RETURN:
        printf("AST_RETURN");
        break;
    case AST_IF:
        printf("AST_IF");
        break;
    case AST_IF_ELSE:
        printf("AST_IF_ELSE");
        break;
    case AST_WHILE:
        printf("AST_WHILE");
        break;
    case AST_LOOP:
        printf("AST_LOOP");
        break;
    default:
        printf("AST_UNKNOWN");
        break;
    }

    if (node->symbol)
    {
        printf("(%s)", node->symbol->text);
    }

    printf("\n");
    for (int i = 0; i < MAX_CHILDREN; i++)
        AST_print(node->child[i], level + 1);
}
