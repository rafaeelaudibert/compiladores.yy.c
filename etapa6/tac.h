#ifndef TAC_H
#define TAC_H

#include "hash.h"

typedef enum
{
    TAC_UNKNOWN,

    TAC_LABEL,

    TAC_SYMBOL,
    TAC_VECTOR_ACCESS,

    TAC_COPY,
    TAC_COPY_IDX,
    TAC_BEGINFUN,
    TAC_ENDFUN,

    TAC_ADD,
    TAC_SUB,
    TAC_MULTIPLY,
    TAC_DIVIDE,
    TAC_LT,
    TAC_GT,
    TAC_OR,
    TAC_AND,
    TAC_LE,
    TAC_GE,
    TAC_EQ,
    TAC_DIF,
    TAC_UNARY_MINUS,
    TAC_UNARY_NEGATION,

    TAC_FUNC_CALL,
    TAC_FUNC_PARAM,

    TAC_ATTRIB,
    TAC_ATTRIB_VECTOR,

    TAC_READ,
    TAC_PRINT,

    TAC_RETURN,

    TAC_JUMP,
    TAC_JUMPFALSE
} TAC_TYPE;

typedef struct TAC
{
    TAC_TYPE type;
    HASH_NODE *res;
    HASH_NODE *op1;
    HASH_NODE *op2;

    struct TAC *prev;
    struct TAC *next;
} TAC;

TAC *tac_create(TAC_TYPE type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2);
TAC *tac_join(TAC *tac1, TAC *tac2);
TAC *tac_reverse(TAC *tac);
void tac_print_recursive(TAC *tac);
void tac_print(TAC *tac);

#endif