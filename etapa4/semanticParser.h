#ifndef SEMANTIC_PARSER_H
#define SEMANTIC_PARSER_H

#include "dataType.h"
#include "identifierType.h"

#include "ast.h"
#include "hash.h"
#include "lex.yy.h"

typedef struct ChainedList
{
    struct ChainedList *next;
    void *val;
} ChainedList;

ChainedList *get_semantic_errors(AST *);

#endif