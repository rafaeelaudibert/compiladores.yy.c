#include "semanticParser.h"
#include "messages.h"

// @returns A fixed size buffer for error messages, with enough size
char *create_error_message_buffer()
{
    const int BUFFER_SIZE = 1024;

    char *buffer = (char *)malloc(1024 * sizeof(char));
    return buffer;
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

DATA_TYPE map_kw_to_dt(int kw)
{
    DATA_TYPE mapped_dt = DT_NONE;
    switch (kw)
    {
    case KW_INT:
        mapped_dt = DT_INT;
        break;
    case KW_FLOAT:
        mapped_dt = DT_FLOAT;
        break;
    case KW_CHAR:
        mapped_dt = DT_CHAR;
        break;
    case KW_BOOL:
        mapped_dt = DT_BOOL;
        break;
    default:
        break;
    }

    return mapped_dt;
}

ChainedList *fill_global_types_declv_not_vector(AST *decl)
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

ChainedList *fill_global_types_declv_vector(AST *decl)
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

ChainedList *fill_global_types_func(AST *decl)
{
    ChainedList *error = NULL;

    // Acess the header, as it is the only important
    decl = decl->child[0];

    HASH_NODE *symbol = decl->child[0]->symbol;
    DATA_TYPE symbol_data_type = symbol->data_type; // DT_NONE or DT_INT
    char *symbol_text = symbol->text;               // b
    int symbol_line = decl->child[0]->line_number;  // 3
    int symbol_type = decl->child[2]->type;         // KW_INT

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
        symbol->identifier_type = IT_FUNCTION;
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

    // OBS: On this run through the AST, we don't check for the parameter types,
    // we only store them in the AST, to later use when checking if a function was
    // called with the correct types
    if (!error)
    {
        AST *values = decl->child[1];
        ChainedList *params_types_start = NULL, *param_types_curr = NULL;
        ChainedList *param_names = NULL;

        while (values)
        {

            DATA_TYPE *mapped_initialized_dt = (DATA_TYPE *)malloc(sizeof(DATA_TYPE));
            char *param_name = values->child[0]->symbol->text;           // x
            int initialized_value_kw = values->child[1]->symbol->type;   // KW_INT
            *mapped_initialized_dt = map_kw_to_dt(initialized_value_kw); // DT_INT

            ChainedList *new_param_type = create_chained_list((void *)mapped_initialized_dt);
            if (param_types_curr)
            {
                param_types_curr->next = new_param_type;
            }
            param_types_curr = new_param_type;
            if (!params_types_start)
                params_types_start = param_types_curr;

            // TODO: Improve this linear time searching for duplicate name in param list
            ChainedList *curr_param_name = param_names;
            int has_name_conflict = 0;
            while (curr_param_name)
            {
                // Error
                if (strcmp((char *)curr_param_name->val, param_name) == 0)
                {
                    has_name_conflict = 1;
                    break;
                }
                curr_param_name = curr_param_name->next;
            }

            if (has_name_conflict)
            {
                // Already declared before
                char *error_message = create_error_message_buffer();
                sprintf(error_message, MESSAGE_PARAM_REDECLARATION, param_name, symbol_line);
                ChainedList *new_error = create_chained_list((void *)error_message);
                new_error->next = error;
                error = new_error;
            }
            else
            {
                // Add the name to the list, so that it can be compared later
                ChainedList *new_param_name = create_chained_list((void *)param_name);
                new_param_name->next = param_names;
                param_names = new_param_name;
            }

            // Point to the next children
            values = values->child[2];
        }

        free_chained_list(param_names);

        symbol->params_data_type = params_types_start;
    }

    return error;
}

ChainedList *fill_global_types(AST *ast)
{

    ChainedList *error_list = NULL;

    while (ast)
    {
        // Here we have, as *decl a node with type AST_DECLV_NOT_VECTOR
        // or AST_DECLV_VECTOR or AST_FUNC
        AST *decl = ast->child[0];
        ChainedList *new_errors = NULL;

        switch (decl->type)
        {
        case AST_DECLV_NOT_VECTOR:
            new_errors = fill_global_types_declv_not_vector(decl);
            break;
        case AST_DECLV_VECTOR:
            new_errors = fill_global_types_declv_vector(decl);
            break;
        case AST_FUNC:
            new_errors = fill_global_types_func(decl);
            break;
        default:
            fprintf(stderr, "NOT IMPLEMENTED fill_hash_table_types FOR THIS TYPE");
            break;
        }

        // Append new_errors to the end of error_list
        if (new_errors)
        {
            // Make curr_error, the last of the itens in the chainedList
            ChainedList *curr_error = new_errors;
            while (curr_error->next)
            {
                curr_error = curr_error->next;
            }

            curr_error->next = error_list;
            error_list = new_errors;
        }

        // Go to the next LDECL node (which might be NULL, meaning the end of LDECLs)
        ast = ast->child[1];
    }

    ChainedList *reversed_error_list = revert_chained_list(error_list);
    return reversed_error_list;
}

ChainedList *get_type_errors_for_func(AST *func)
{
    ChainedList *errors = NULL;

    AST *func_header = func->child[0];

    // Fill the hashTable with the local types
    AST *func_params = func_header->child[1];
    while (func_params)
    {
        HASH_NODE *symbol = func_params->child[0]->symbol;
        DATA_TYPE type = map_kw_to_dt(func_params->child[1]->symbol->type);

        // Use the first type, for the cases where it is being redeclared
        if (symbol->local_data_type == DT_NONE)
        {
            symbol->local_data_type = type;
        }

        // Go to the next parameter
        func_params = func_params->child[2];
    }

    // TODO:
    //      Iterate through the tree checking for correct types
    //          Mark DT on AST nodes
    //      Check the returned value is from the right type
    //          find the return AST node
    //          check that the node DT is the same as the function return value

    // Remove the local types from the hashTable
    func_params = func_header->child[1];
    while (func_params)
    {
        HASH_NODE *symbol = func_params->child[0]->symbol;
        symbol->local_data_type = DT_NONE;

        // Go to the next parameter
        func_params = func_params->child[2];
    }

    return errors;
}

/// TODO DOCS: Recursively iterate over the AST building the errors
ChainedList *get_type_errors(AST *ast)
{

    ChainedList *type_errors = NULL;
    while (ast)
    {
        AST *node = ast->child[0];
        if (node->type == AST_FUNC)
        {
            ChainedList *func_errors = get_type_errors_for_func(node);

            // Append to the end of type_errors
            if (func_errors)
            {
                ChainedList *curr_error = func_errors;
                while (curr_error->next)
                {
                    curr_error = curr_error->next;
                }

                curr_error->next = type_errors;
                type_errors = curr_error;
            }
        }

        // Go to the next LDECL
        ast = ast->child[1];
    }

    return type_errors;
}

/// Given an AST root, makes the semantic analysis.
/// It will:
///     - Fill the hash table with the correct data and identifier types
///         (for redeclared types, it will use the first one to continue the analysis)
///     - Check for redeclarations
///     - Check for invalid variable initialization
///     - TODO MORE
///
/// @param ast An AST root, which will be iterated
///
/// @returns A ChainedList* with strings containing the errors which it found (can be null)
ChainedList *get_semantic_errors(AST *ast)
{
    ChainedList *decl_errors = fill_global_types(ast);
    ChainedList *type_errors = get_type_errors(ast);

    if (!decl_errors)
    {
        return type_errors;
    }

    // Go to the end of decl_errors, and append type_errors to return both of them
    ChainedList *curr = decl_errors;
    while (curr->next)
    {
        curr = curr->next;
    }
    curr->next = type_errors;

    return decl_errors;
}
