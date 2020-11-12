#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataType.h"
#include "identifierType.h"
#include "chainedList.h"

#define HASH_SIZE 997

#define TYPE_TMP 2001
#define TYPE_LBL 2002

typedef struct hash_node
{
    int type;
    char *text;
    struct hash_node *next;

    DATA_TYPE data_type;
    IDENTIFIER_TYPE identifier_type;

    // Init values
    ChainedList *init_values;

    // This is used in the local scope, so that we have a easy lookup table
    // for parameter declarations in functions
    // It is filled with the local data type when entering the AST_FUNC AST's node
    // and returned to DT_NONE when leaving its body
    DATA_TYPE local_data_type;

    // Used if id_type is IT_FUNCTION, to know the order of the parameters types
    ChainedList *params_data_type;
} HASH_NODE;

void hash_init(void);
void free_table(void);
int hash_address(char *text);
HASH_NODE *hash_find(char *text);
HASH_NODE *hash_insert(char *text, int type);
void hash_print(void);

HASH_NODE *make_temp();
HASH_NODE *make_label();

void add_init_value(HASH_NODE *node, HASH_NODE *value_node);
void hash_generate_global_ASM(FILE *);

char *escaped_string(char *str);
#endif
