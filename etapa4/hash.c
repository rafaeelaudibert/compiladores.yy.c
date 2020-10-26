#include "hash.h"

HASH_NODE **table = NULL;

void hash_init(void)
{
    if (table)
        free_table();

    table = (HASH_NODE **)calloc(HASH_SIZE, sizeof(HASH_NODE *));
}

void free_table(void)
{
    HASH_NODE *node, *next_node;

    // For each position in the table
    for (int table_idx = 0; table_idx < HASH_SIZE; table_idx++)
    {
        // Get the pointer in the current position
        node = table[table_idx];

        // While pointing at something that is not NULL
        while (node)
        {
            // Save the next
            next_node = node->next;

            // Free the text, and the node itself
            free(node->text);
            free(node);

            // Now we will look for the next
            node = next_node;
        }
    }

    // Finally, free the table pointer, and reset it
    free(table);
    table = NULL;
}

int hash_address(char *text)
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

HASH_NODE *hash_find(char *text)
{
    int address = hash_address(text);
    HASH_NODE *node = table[address];

    while (node)
    {
        if (strcmp(text, node->text) == 0)
            return node;

        node = node->next;
    }

    return NULL;
}

HASH_NODE *hash_insert(char *text, int type)
{
    HASH_NODE *new_node = hash_find(text);

    if (!new_node)
    {
        new_node = (HASH_NODE *)calloc(1, sizeof(HASH_NODE));
        new_node->type = type;
        new_node->text = (char *)calloc(strlen(text) + 1, sizeof(char));
        strcpy(new_node->text, text);

        // Filled later by the Semantic Analysis, to easily
        // verify if we:
        //      * redeclared an identifier
        //      * declared an identifier twice
        //      * used an identifier as it should (scalar, vector, function)
        //      * used an identifier with its type (int, float, char, bool)
        //      * declared it as a parameter in the local scope, with a different type
        new_node->data_type = DT_NONE;
        new_node->identifier_type = DT_NONE;
        new_node->local_data_type = DT_NONE;

        int address = hash_address(text);
        new_node->next = table[address];
        table[address] = new_node;
    }

    return new_node;
}

void hash_print(void)
{
    int list_idx;
    HASH_NODE *node;

    if (table)
    {
        printf("Hash: \n");
        for (int table_idx = 0; table_idx < HASH_SIZE; table_idx++)
            for (node = table[table_idx], list_idx = 0; node; node = node->next, list_idx++)
                printf(
                    "Table[%d][%d] -> %s (Type %d) (DataType %d)\n",
                    table_idx,
                    list_idx,
                    node->text,
                    node->type,
                    node->data_type);
    }
}