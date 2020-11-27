#include <stdio.h>
#include "ast.h"
#include "lex.yy.h"

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

    // Initialize AST without knowing their data and identifier type
    node->data_type = DT_NONE;
    node->id_type = IT_NONE;

    // This has some problems with '\n's, but is it not a huge problem,
    // as it is mainly for debugging purposes (semantic analysis error messages)
    node->line_number = getLineNumber();

    // Fill hash table with initialization value, if it is a declaration
    if (node->type == AST_DECLV_NOT_VECTOR)
    {
        add_init_value(node->child[0]->symbol, node->child[2]->symbol);
    }
    else if (node->type == AST_DECLV_VECTOR)
    {
        if (node->child[3] != NULL)
        {
            AST *current = node->child[3];
            while (current)
            {
                add_init_value(node->child[0]->symbol, current->child[0]->symbol);
                current = current->child[1];
            }
        }
        else
        {
            // Not declared vector, we initialize with 0, because yes
            HASH_NODE *node_0 = hash_insert("0", LIT_INTEGER);
            for (int i = 0; i < strtol(node->child[2]->symbol->text, NULL, 16); i++)
            {
                add_init_value(node->child[0]->symbol, node_0);
            }
        }
    }
    else if (node->type == AST_SYMBOL && node->symbol->identifier_type == IT_SCALAR)
    {
        // If we have a symbol, we add an initial value of itself
        add_init_value(node->symbol, node->symbol);
    }

    return node;
}

void AST_print(AST *node, int level)
{
    if (!node)
        return;

    for (int i = 0; i < level; i++)
        printf("  ");

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
        printf("(%s)", node->symbol->text);

    printf("\n");
    for (int i = 0; i < MAX_CHILDREN; i++)
        AST_print(node->child[i], level + 1);
}

AST *AST_optimize(AST *node)
{
    if (!node)
        return NULL;

    for (int i = 0; i < MAX_CHILDREN; i++)
        node->child[i] = AST_optimize(node->child[i]);

    if (
        node->child[0] &&
        node->child[0]->type == AST_SYMBOL &&
        node->child[0]->symbol->type == LIT_INTEGER &&
        node->child[1] && node->child[1]->type == AST_SYMBOL &&
        node->child[1]->symbol->type == LIT_INTEGER)
    {
        long long op_1 = strtoll(node->child[0]->symbol->text, NULL, 16);
        long long op_2 = strtoll(node->child[1]->symbol->text, NULL, 16);
        long long new_val = 0;
        int changed_number = 0, changed_bool = 0;
        if (node->type == AST_ADD)
        {
            new_val = op_1 + op_2;
            changed_number = 1;
        }
        else if (node->type == AST_SUB)
        {
            new_val = op_1 - op_2;
            changed_number = 1;
        }
        else if (node->type == AST_MULTIPLY)
        {
            new_val = op_1 * op_2;
            changed_number = 1;
        }
        else if (node->type == AST_DIVIDE)
        {
            new_val = op_1 / op_2;
            changed_number = 1;
        }
        else if (node->type == AST_GT)
        {
            new_val = op_1 > op_2;
            changed_bool = 1;
        }
        else if (node->type == AST_GE)
        {
            new_val = op_1 >= op_2;
            changed_bool = 1;
        }
        else if (node->type == AST_LT)
        {
            new_val = op_1 < op_2;
            changed_bool = 1;
        }
        else if (node->type == AST_LE)
        {
            new_val = op_1 <= op_2;
            changed_bool = 1;
        }
        else if (node->type == AST_EQ)
        {
            new_val = op_1 == op_2;
            changed_bool = 1;
        }
        else if (node->type == AST_DIF)
        {
            new_val = op_1 != op_2;
            changed_bool = 1;
        }

        if (changed_number)
        {
            char *new_val_str = (char *)malloc(64 * sizeof(char));
            sprintf(new_val_str, "0%X", (int)new_val);
            HASH_NODE *hash_node = hash_insert(new_val_str, LIT_INTEGER);

            free(node);
            node = AST_create(AST_SYMBOL, hash_node, NULL, NULL, NULL, NULL, NULL);
        }

        if (changed_bool)
        {
            HASH_NODE *hash_node = hash_insert(
                new_val ? "TRUE" : "FALSE",
                new_val ? LIT_TRUE : LIT_FALSE);

            free(node);
            node = AST_create(AST_SYMBOL, hash_node, NULL, NULL, NULL, NULL, NULL);
        }
    }

    // Boolean optimization
    if (
        node->child[0] &&
        node->child[0]->type == AST_SYMBOL &&
        (node->child[0]->symbol->type == LIT_TRUE || node->child[0]->symbol->type == LIT_FALSE) &&
        node->child[1] && node->child[1]->type == AST_SYMBOL &&
        (node->child[1]->symbol->type == LIT_TRUE || node->child[1]->symbol->type == LIT_FALSE))
    {
        int op_1 = node->child[0]->symbol->type == LIT_TRUE;
        int op_2 = node->child[1]->symbol->type == LIT_TRUE;
        int new_val = 0;
        int changed = 0;
        if (node->type == AST_AND)
        {
            new_val = op_1 && op_2;
            changed = 1;
        }
        else if (node->type == AST_OR)
        {
            new_val = op_1 || op_2;
            changed = 1;
        }

        if (changed)
        {
            HASH_NODE *hash_node = hash_insert(
                new_val ? "TRUE" : "FALSE",
                new_val ? LIT_TRUE : LIT_FALSE);

            free(node);
            node = AST_create(AST_SYMBOL, hash_node, NULL, NULL, NULL, NULL, NULL);
        }
    }

    return node;
}
