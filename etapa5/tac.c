#include "tac.h"

TAC *tac_create(TAC_TYPE type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2)
{
    TAC *new_tac = NULL;
    new_tac = (TAC *)malloc(sizeof(TAC));

    new_tac->type = type;
    new_tac->res = res;
    new_tac->op1 = op1;
    new_tac->op2 = op2;

    new_tac->prev = NULL;
    new_tac->next = NULL;

    return new_tac;
}

TAC *tac_join(TAC *tac1, TAC *tac2)
{
    if (!tac1)
        return tac2;
    if (!tac2)
        return tac1;

    TAC *point = tac2;
    while (point->prev)
        point = point->prev;

    point->prev = tac1;

    return tac2;
}

void tac_print(TAC *tac)
{

    if (!tac || tac->type == TAC_SYMBOL)
        return;

    printf("TAC(");
    switch (tac->type)
    {
    case TAC_SYMBOL:
        printf("TAC_SYMBOL");
        break;
    case TAC_TEMP:
        printf("TAC_TEMP");
        break;
    case TAC_LABEL:
        printf("TAC_LABEL");
        break;
    case TAC_VECTOR_ACCESS:
        printf("TAC_VECTOR_ACCESS");
        break;
    case TAC_COPY:
        printf("TAC_COPY");
        break;
    case TAC_COPY_IDX:
        printf("TAC_COPY_IDX");
        break;
    case TAC_BEGINFUN:
        printf("TAC_BEGINFUN");
        break;
    case TAC_ENDFUN:
        printf("TAC_ENDFUN");
        break;
    case TAC_ADD:
        printf("TAC_ADD");
        break;
    case TAC_SUB:
        printf("TAC_SUB");
        break;
    case TAC_MULTIPLY:
        printf("TAC_MULTIPLY");
        break;
    case TAC_DIVIDE:
        printf("TAC_DIVIDE");
        break;
    case TAC_LT:
        printf("TAC_LT");
        break;
    case TAC_GT:
        printf("TAC_GT");
        break;
    case TAC_OR:
        printf("TAC_OR");
        break;
    case TAC_AND:
        printf("TAC_AND");
        break;
    case TAC_LE:
        printf("TAC_LE");
        break;
    case TAC_GE:
        printf("TAC_GE");
        break;
    case TAC_EQ:
        printf("TAC_EQ");
        break;
    case TAC_DIF:
        printf("TAC_DIF");
        break;
    case TAC_UNARY_MINUS:
        printf("TAC_UNARY_MINUS");
        break;
    case TAC_UNARY_NEGATION:
        printf("TAC_UNARY_NEGATION");
        break;
    case TAC_FUNC_CALL:
        printf("TAC_FUNC_CALL");
        break;
    case TAC_PUSH:
        printf("TAC_PUSH");
        break;

    case TAC_ATTRIB:
        printf("TAC_ATTRIB");
        break;
    case TAC_ATTRIB_VECTOR:
        printf("TAC_ATTRIB_VECTOR");
        break;
    case TAC_READ:
        printf("TAC_READ");
        break;
    case TAC_PRINT:
        printf("TAC_PRINT");
        break;
    case TAC_RETURN:
        printf("TAC_RETURN");
        break;
    case TAC_JUMP:
        printf("TAC_JUMP");
        break;
    case TAC_JUMPFALSE:
        printf("TAC_JUMPFALSE");
        break;
    case TAC_UNKNOWN:
    default:
        printf("TAC_UNKNOWN");
        break;
    }

    printf(", %s", tac->res ? tac->res->text : "*");
    printf(", %s", tac->op1 ? tac->op1->text : "*");
    printf(", %s", tac->op2 ? tac->op2->text : "*");
    printf(");\n");
}

void tac_print_recursive(TAC *tac)
{
    if (!tac)
        return;

    tac_print_recursive(tac->prev);
    tac_print(tac);
    fflush(stdout);
}