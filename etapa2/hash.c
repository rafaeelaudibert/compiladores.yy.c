#include "hash.h"

HASH_NODE **table = NULL;

void hashInit(void)
{
    if (table)
        freeTable();

    table = (HASH_NODE **)calloc(HASH_SIZE, sizeof(HASH_NODE *));
}

void freeTable(void)
{
    int table_idx, list_idx;
    HASH_NODE *node, *nextNode;

    // For each position in the table
    for (int tableIdx = 0; tableIdx < HASH_SIZE; tableIdx++)
    {
        // Get the pointer in the current position
        node = table[tableIdx];

        // While pointing at something that is not NULL
        while (node)
        {
            // Save the next
            nextNode = node->next;

            // Free the text, and the node itself
            free(node->text);
            free(node);

            // Now we will look for the next
            node = nextNode;
        }
    }

    // Finally, free the table pointer, and reset it
    free(table);
    table = NULL;
}

int hashAddress(char *text)
{
    int address = 1;
    int text_idx;

    // Initializing in one, and taking modulo (HASH_SIZE + 1)
    // we are in range [1, HASH_SIZE + 1)
    for (text_idx = 0; text_idx < strlen(text); text_idx++)
        address = (address * text[text_idx]) % HASH_SIZE + 1;

    // We decrement 1 to be in the range [0, HASH_SIZE),
    //so that we can access the table correctly
    return address - 1;
}

HASH_NODE *hashFind(char *text)
{
    int address = hashAddress(text);
    HASH_NODE *node = table[address];

    while (node)
    {
        if (strcmp(text, node->text) == 0)
            return node;

        node = node->next;
    }

    return NULL;
}

HASH_NODE *hashInsert(char *text, int type)
{
    HASH_NODE *new_node = hashFind(text);

    if (!new_node)
    {
        new_node = (HASH_NODE *)calloc(1, sizeof(HASH_NODE));
        new_node->type = type;
        new_node->text = (char *)calloc(strlen(text) + 1, sizeof(char));
        strcpy(new_node->text, text);

        int address = hashAddress(text);
        new_node->next = table[address];
        table[address] = new_node;
    }

    return new_node;
}

void hashPrint(void)
{
    int table_idx, list_idx;
    HASH_NODE *node;

    if (table)
    {
        printf("Hash: \n");
        for (int tableIdx = 0; tableIdx < HASH_SIZE; tableIdx++)
            for (node = table[tableIdx], list_idx = 0; node; node = node->next, list_idx++)
                printf("Table[%d][%d] -> %s (Type %d)\n", tableIdx, list_idx, node->text, node->type);
    }
}