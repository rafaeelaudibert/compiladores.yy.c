#include "semanticParser.h"

// Ex: Identifier abcd at line 15 being already declared before
const char *MESSAGE_REDECLARATION = "Identifier %s at line %d already declared before";

// Ex: Identifier abcd at line 15 received incompatible initialization value .0f12c
const char *MESSAGE_INCOMPATIBLE_INITIALIZATION = "Identifier %s at line %d received incompatible initialization value %s";

// Ex: Identifier abcd at line 15 should receive 10 itens in initialization but received 4
const char *MESSAGE_WRONG_NUMBER_ITENS_VECTOR = "Identifier %s at line %d should receive %d itens in initialization but received %d";

// @returns A fixed size buffer for error messages, with enough size
char *create_error_message_buffer()
{
    const int BUFFER_SIZE = 1024;

    char *buffer = (char *)malloc(1024 * sizeof(char));
    return buffer;
}

/// Creates a ChainedList node, pointing to NULL, and a val as `val`
///
/// @param val A pointer to void value, which this ChainedList will point to
///
/// @returns A new pointer to a ChainedList node
ChainedList *create_chained_list(void *val)
{
    ChainedList *cl = (ChainedList *)malloc(sizeof(ChainedList));
    cl->next = NULL;
    cl->val = (void *)val;

    return cl;
}

DATA_TYPE map_lit_to_dt(int lit)
{
    DATA_TYPE mapped_dt = DT_NONE;
    switch (lit)
    {
    case LIT_INTEGER:
        mapped_dt = DT_INT;
        break;
    case LIT_FLOAT:
        mapped_dt = DT_FLOAT;
        break;
    case LIT_CHAR:
        mapped_dt = DT_CHAR;
        break;
    case LIT_TRUE:
    case LIT_FALSE:
        mapped_dt = DT_BOOL;
        break;
    default:
        break;
    }

    return mapped_dt;
}

ChainedList *fill_hash_table_declv_not_vector(AST *decl)
{
    ChainedList *error = NULL;

    HASH_NODE *symbol = decl->child[0]->symbol;
    DATA_TYPE symbol_data_type = symbol->data_type; // DT_NONE or DT_BOOL
    char *symbol_text = symbol->text;               // b
    int symbol_line = decl->child[0]->line_number;  // 3
    int symbol_type = decl->child[1]->symbol->type; // KW_BOOL

    // Check first for redeclarations
    if (symbol_data_type != DT_NONE)
    {
        // Already declared before
        char *error_message = create_error_message_buffer();
        sprintf(error_message, MESSAGE_REDECLARATION, symbol_text, symbol_line);
        error = create_chained_list((void *)error_message);
    }
    else
    {
        // Fill symbol IT and DT, and it is not declared
        symbol->identifier_type = IT_SCALAR;
        switch (symbol_type)
        {
        case KW_INT:
            symbol->data_type = DT_INT;
            break;
        case KW_FLOAT:
            symbol->data_type = DT_FLOAT;
            break;
        case KW_CHAR:
            symbol->data_type = DT_CHAR;
            break;
        case KW_BOOL:
            symbol->data_type = DT_BOOL;
            break;
        }
    }

    // Check for wrong initialization, only if there is no problem
    // with a redeclaration

    if (!error)
    {
        char *initialized_value = decl->child[2]->symbol->text;                  // FALSE
        int initialized_value_type = decl->child[2]->symbol->type;               // LIT_FALSE
        DATA_TYPE mapped_initialized_dt = map_lit_to_dt(initialized_value_type); // DT_BOOL

        if (!is_compatible(mapped_initialized_dt, symbol->data_type))
        {
            // Has an incompatible declaration
            char *error_message = create_error_message_buffer();
            sprintf(error_message, MESSAGE_INCOMPATIBLE_INITIALIZATION, symbol_text, symbol_line, initialized_value);
            error = create_chained_list((void *)error_message);
        }
    }

    return error;
}

/*
AST_DECLV_VECTOR
    AST_SYMBOL(v1)
    AST_SYMBOL(int)
    AST_SYMBOL(10)
    AST_LVECTOR
        AST_SYMBOL(0)
        AST_LVECTOR
            AST_SYMBOL(0)
            AST_LVECTOR
                AST_SYMBOL(0)
                AST_LVECTOR
                    AST_SYMBOL(0)
                    AST_LVECTOR
                        AST_SYMBOL(0)
                        AST_LVECTOR
                            AST_SYMBOL(0)
                            AST_LVECTOR
                                AST_SYMBOL(0)
                                AST_LVECTOR
                                    AST_SYMBOL(0)
                                    AST_LVECTOR
                                        AST_SYMBOL(0)
                                        AST_LVECTOR
                                            AST_SYMBOL(0)
*/
ChainedList *fill_hash_table_declv_vector(AST *decl)
{
    ChainedList *error = NULL;

    HASH_NODE *symbol = decl->child[0]->symbol;
    DATA_TYPE symbol_data_type = symbol->data_type;     // DT_NONE or DT_INT
    char *symbol_text = symbol->text;                   // v1
    int symbol_line = decl->child[0]->line_number;      // 5
    int symbol_type = decl->child[1]->symbol->type;     // KW_INT
    int qty_itens = atoi(decl->child[2]->symbol->text); // 10

    // Check first for redeclarations
    if (symbol_data_type != DT_NONE)
    {
        // Already declared before
        char *error_message = create_error_message_buffer();
        sprintf(error_message, MESSAGE_REDECLARATION, symbol_text, symbol_line);
        error = create_chained_list((void *)error_message);
    }
    else
    {
        // Fill symbol IT and DT, and it is not declared
        symbol->identifier_type = IT_VECTOR;
        switch (symbol_type)
        {
        case KW_INT:
            symbol->data_type = DT_INT;
            break;
        case KW_FLOAT:
            symbol->data_type = DT_FLOAT;
            break;
        case KW_CHAR:
            symbol->data_type = DT_CHAR;
            break;
        case KW_BOOL:
            symbol->data_type = DT_BOOL;
            break;
        }
    }

    // Check for wrong initialization, only if there is no problem
    // with a redeclaration
    //
    // For vector a wrong initialization can be:
    //      - Not enough values
    //      - Too many values
    //      - Wrong type on values
    if (!error)
    {
        int values_count = 0;
        AST *values = decl->child[3];
        while (values)
        {
            values_count++;

            char *initialized_value = values->child[0]->symbol->text;                // 0
            int initialized_value_type = values->child[0]->symbol->type;             // LIT_INT
            DATA_TYPE mapped_initialized_dt = map_lit_to_dt(initialized_value_type); // DT_INT

            if (!is_compatible(mapped_initialized_dt, symbol->data_type))
            {
                // Has an incompatible declaration
                char *error_message = create_error_message_buffer();
                sprintf(error_message, MESSAGE_INCOMPATIBLE_INITIALIZATION, symbol_text, symbol_line, initialized_value);
                ChainedList *new_error = create_chained_list((void *)error_message);
                new_error->next = error;
                error = new_error;
            }

            values = values->child[1];
        }

        if (values_count && values_count != qty_itens)
        {
            // Has a different quantity of itens in the list declaraction
            char *error_message = create_error_message_buffer();
            sprintf(error_message, MESSAGE_WRONG_NUMBER_ITENS_VECTOR, symbol_text, symbol_line, qty_itens, values_count);
            ChainedList *new_error = create_chained_list((void *)error_message);
            new_error->next = error;
            error = new_error;
        }
    }

    return error;
}

ChainedList *fill_hash_table_types(AST *ast)
{
    if (!ast)
    {
        return NULL;
    }

    // Here we have, as *decl a node with type AST_DECLV_NOT_VECTOR
    // or AST_DECLV_VECTOR or AST_FUNC
    //
    // TODO: Do for the other case AST_FUNC
    AST *decl = ast->child[0];

    ChainedList *error = NULL;
    switch (decl->type)
    {
    case AST_DECLV_NOT_VECTOR:
        error = fill_hash_table_declv_not_vector(decl);
        break;
    case AST_DECLV_VECTOR:
        error = fill_hash_table_declv_vector(decl);
        break;
    default:
        fprintf(stdout, "NOT IMPLEMENTED fill_hash_table_types FOR THIS TYPE");
        break;
    }

    return error;
}

/// Given an AST root, makes the semantic analysis.
/// It will:
///     - Fill the hash table with the correct data and identifier types
///         (for redeclared types, it will use the first one to continue the analysis)
///     - Check for redeclarations
///     - Check for usage without declaration
///     - Check for invalid variable initialization
///     - TODO MORE
///
/// @param ast An AST root, which will be iterated
///
/// @returns A ChainedList* with strings containing the errors which it found (can be null)
ChainedList *get_semantic_errors(AST *ast)
{
    ChainedList *decl_errors = fill_hash_table_types(ast);
    return decl_errors;
}
