#include "semanticParser.h"
#include "messages.h"
#include "typeInfer.h"

// @returns A fixed size buffer for error messages, with enough size
char *create_error_message_buffer()
{
    const int BUFFER_SIZE = 256;

    char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));
    return buffer;
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
    DATA_TYPE symbol_data_type = symbol->data_type;                 // DT_NONE or DT_INT
    char *symbol_text = symbol->text;                               // v1
    int symbol_line = decl->child[0]->line_number;                  // 5
    int symbol_type = decl->child[1]->symbol->type;                 // KW_INT
    int qty_itens = strtol(decl->child[2]->symbol->text, NULL, 16); // 10

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
    int symbol_type = decl->child[2]->symbol->type; // KW_INT

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

            // Will be adding from the beginning to the end of the list 3->2->1
            ChainedList *new_param_type = create_chained_list((void *)mapped_initialized_dt);
            if (param_types_curr)
                param_types_curr->next = new_param_type;

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

        symbol->params_data_type = params_types_start;
        symbol->params_names = revert_chained_list(param_names);
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
        error_list = append_end(new_errors, error_list);

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
            symbol->local_data_type = type;

        // Go to the next parameter
        func_params = func_params->child[2];
    }

    DATA_TYPE func_return_type = func_header->child[0]->symbol->data_type;

    // Iterate through the function body finding semantic inconsistences, marking the node types
    AST *func_body = func->child[1];
    while (func_body)
    {
        AST *command = func_body->child[0];
        if (command)
        {
            int command_line = command->line_number;

            DATA_TYPE dt = infer_type(command);
            if (!is_basic_data_type(dt))
            {
                // Need to create a new error for this command
                char *error_message = create_error_message_buffer();

                switch (dt)
                {
                case DT_INVALID_IDENTIFIER_TYPE:
                    sprintf(error_message, MESSAGE_INVALID_IDENTIFIER_TYPE, command_line);
                    break;
                case DT_UNDEFINED_SYMBOL:
                    sprintf(error_message, MESSAGE_UNDEFINED_SYMBOL, command_line);
                    break;
                case DT_WRONG_INDEX_TYPE:
                    sprintf(error_message, MESSAGE_WRONG_INDEX_TYPE, command_line);
                    break;
                case DT_NON_ARITHMETIC:
                    sprintf(error_message, MESSAGE_NON_ARITHMETIC, command_line);
                    break;
                case DT_NON_BOOLEAN:
                    sprintf(error_message, MESSAGE_NON_BOOLEAN, command_line);
                    break;
                case DT_INVALID_PARAMETER_QUANTITY:
                    sprintf(error_message, MESSAGE_INVALID_PARAMETER_QUANTITY, command_line);
                    break;
                case DT_INVALID_PARAMETER_TYPE:
                    sprintf(error_message, MESSAGE_INVALID_PARAMETER_TYPE, command_line);
                    break;
                case DT_INVALID_ATTRIBUTION:
                    sprintf(error_message, MESSAGE_INVALID_ATTRIBUTION, command_line);
                    break;
                case DT_NOT_INFERED:
                default:
                    sprintf(error_message, MESSAGE_NOT_INFERED, dt, command_line);
                    break;
                }

                ChainedList *new_error = create_chained_list((void *)error_message);
                new_error->next = errors;
                errors = new_error;
            }

            if (command->type == AST_RETURN)
            {
                if (!is_compatible(dt, func_return_type))
                {
                    char *error_message = create_error_message_buffer();
                    sprintf(error_message, MESSAGE_WRONG_RETURN_TYPE, command_line);

                    ChainedList *new_error = create_chained_list((void *)error_message);
                    new_error->next = errors;
                    errors = new_error;
                }
            }
        }

        func_body = func_body->child[1];
    }

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

/// Recursively iterate over the AST building the errors
///
/// @param ast An AST to iter onto
///
/// @returns A ChainedList* of errors
ChainedList *get_type_errors(AST *ast)
{

    ChainedList *type_errors = NULL;
    while (ast)
    {
        AST *node = ast->child[0];
        if (node->type == AST_FUNC)
        {
            ChainedList *func_errors = get_type_errors_for_func(node);

            // Append to the start of type_errors
            type_errors = append_end(func_errors, type_errors);
        }

        // Go to the next LDECL
        ast = ast->child[1];
    }

    ChainedList *reversed_error_list = revert_chained_list(type_errors);
    return reversed_error_list;
}

/// Given an AST root, makes the semantic analysis.
///
/// @param ast An AST root, which will be iterated
///
/// @returns A ChainedList* with strings containing the errors which it found (can be NULL)
ChainedList *get_semantic_errors(AST *ast)
{
    ChainedList *decl_errors = fill_global_types(ast);
    ChainedList *type_errors = get_type_errors(ast);

    ChainedList *appended = append_end(decl_errors, type_errors);

    return appended;
}
