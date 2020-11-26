#include "hash.h"
#include <ctype.h>

// We need to redefine, so that we don't have a redeclaration loop
const int TK_IDENTIFIER = 274;
const int LIT_INTEGER = 275;
const int LIT_FLOAT = 276;
const int LIT_FALSE = 277;
const int LIT_TRUE = 278;
const int LIT_CHAR = 279;
const int LIT_STRING = 280;

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

        new_node->init_values = NULL;

        // Filled later by the Semantic Analysis, to easily
        // verify if we:
        //      * redeclared an identifier
        //      * declared an identifier twice
        //      * used an identifier as it should (scalar, vector, function)
        //      * used an identifier with its type (int, float, char, bool)
        //      * declared it as a parameter in the local scope, with a different type
        new_node->data_type = DT_NONE;
        new_node->identifier_type = IT_NONE;
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

void add_init_value(HASH_NODE *node, HASH_NODE *value_node)
{

    char *value = (char *)malloc(64 * sizeof(char));
    if (value_node->type == LIT_INTEGER)
    {
        sprintf(value, "0x%s", value_node->text);
    }
    else if (value_node->type == LIT_FLOAT)
    {
        // TODO: SEE HOW TO DEAL WITH FLOATS
        sprintf(value, "0x0");
    }
    else if (value_node->type == LIT_CHAR)
    {
        // value_node->text is already on the format with single quotes: 'c'
        sprintf(value, value_node->text);
    }
    else if (value_node->type == LIT_FALSE)
    {
        // FALSE is 0
        sprintf(value, "0x0");
    }
    else if (value_node->type == LIT_TRUE)
    {
        // TRUE is 1
        sprintf(value, "0x1");
    }
    else if (value_node->type == TYPE_TMP)
    {
        // Temporary are 0 by default
        sprintf(value, "0x0");
    }

    ChainedList *this_value = create_chained_list(value);
    if (!node->init_values)
    {
        node->init_values = this_value;
    }
    else
    {
        ChainedList *val = node->init_values;
        while (val->next)
        {
            val = val->next;
        }
        val->next = this_value;
    }
}

HASH_NODE *make_temp()
{
    static unsigned int counter = 1;
    char buffer[255] = {0};

    sprintf(buffer, "$temp_%u", counter++);

    HASH_NODE *node = hash_insert(buffer, TYPE_TMP);
    add_init_value(node, node); // We pass itself

    return node;
}

HASH_NODE *make_label()
{
    static unsigned int counter = 1;
    char buffer[255] = {0};

    sprintf(buffer, "$label_%u", counter++);

    return hash_insert(buffer, TYPE_LBL);
}

char *escaped_string(char *str)
{
    char *buffer = (char *)calloc(255, sizeof(char));

    strncpy(buffer, str, 253 * sizeof(char));

    int size = 'z' - 'a';
    int i = 0;
    for (; buffer[i]; i++)
    {
        if (isspace((unsigned char)buffer[i]))
        {
            buffer[i] = '_';
        }
        else if (
            (buffer[i] < 'a' || buffer[i] > 'z') &&
            (buffer[i] < 'A' || buffer[i] > 'Z') &&
            buffer[i] != '_')
        {
            buffer[i] = (buffer[i] % size) + 'a';
        }
    }

    // Removes the last char
    buffer[i - 1] = '\0';

    return buffer + 1;
}

char *explicit_slash_0(char *str)
{
    int len = strlen(str);
    char *buffer = (char *)calloc(len + 5, sizeof(char));

    strncpy(buffer, str, (len + 5) * sizeof(char));

    // Adds a \0 at the end
    buffer[len - 1] = '\\';
    buffer[len] = '0';
    buffer[len + 1] = '"';

    return buffer;
}

void hash_generate_global_ASM(FILE *fout)
{
    if (table)
    {
        fprintf(fout, "\n");
        for (int table_idx = 0; table_idx < HASH_SIZE; table_idx++)
            for (HASH_NODE *node = table[table_idx]; node; node = node->next)
            {
                if (node->init_values)
                {
                    fprintf(fout, "_%s: \n", node->text);
                    for (ChainedList *init_val = node->init_values; init_val; init_val = init_val->next)
                    {
                        fprintf(fout, "\t.long\t%s\n", (char *)init_val->val);
                    }
                }
                else if (node->type == TK_IDENTIFIER && node->identifier_type != IT_FUNCTION)
                {
                    // Local variable
                    fprintf(fout, "_%s: \n\t.long\t0x0\n", node->text);
                }
                else if (node->data_type == DT_STRING || node->type == LIT_STRING)
                {
                    char *escaped_str = escaped_string(node->text);
                    char *str_with_0 = explicit_slash_0(node->text);
                    fprintf(
                        fout,
                        "__%s_string_r4nd0m: \n"
                        "\t.ascii\t%s\n",
                        escaped_str,
                        str_with_0);
                }
                else if (node->type == TYPE_TMP)
                {
                    fprintf(
                        fout,
                        "_%s: \n"
                        "\t.long\t0x0\n",
                        node->text);
                }
            }
    }
}