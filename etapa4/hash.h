#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataType.h"
#include "identifierType.h"

#define HASH_SIZE 997

typedef struct hash_node
{
    int type;
    char *text;
    struct hash_node *next;

    DATA_TYPE data_type;
    IDENTIFIER_TYPE identifier_type;
} HASH_NODE;

void hashInit(void);
void freeTable(void);
int hashAddres(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(char *text, int type);
void hashPrint(void);

#endif
