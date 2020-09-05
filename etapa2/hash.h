#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 997

typedef struct hash_node
{
    int type;
    char *text;
    struct hash_node *next;
} HASH_NODE;

void hashInit(void);
void freeTable(void);
int hashAddres(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(char *text, int type);
void hashPrint(void);

#endif
