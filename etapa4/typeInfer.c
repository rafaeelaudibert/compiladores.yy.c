#include "typeInfer.h"
#include "lex.yy.h"

#define ASSERT_BASIC_DATA_TYPE(x, error) \
    if (!is_basic_data_type(x))          \
    return error

#define ASSERT_COMPATIBLE(a, b, error) \
    if (!is_compatible(a, b))          \
    return error

#define ASSERT_ARITHMETIC(x, error) \
    if (!is_arithmetic(x))          \
    return error

DATA_TYPE higher_type_for_arithmetic(DATA_TYPE left_op, DATA_TYPE right_op)
{
    ASSERT_ARITHMETIC(left_op, DT_NON_ARITHMETIC);
    ASSERT_ARITHMETIC(right_op, DT_NON_ARITHMETIC);

    if (is_float(left_op) || is_float(right_op))
        return DT_FLOAT;

    if (is_integer(left_op) || is_integer(right_op))
        return DT_INT;

    return DT_CHAR;
}

DATA_TYPE infer_type_symbol(AST *node)
{
    HASH_NODE *symbol = node->symbol;

    // Literals
    switch (symbol->type)
    {
    case LIT_INTEGER:
        return DT_INT;
    case LIT_FLOAT:
        return DT_FLOAT;
    case LIT_CHAR:
        return DT_CHAR;
    case LIT_TRUE:
    case LIT_FALSE:
        return DT_BOOL;
    case LIT_STRING:
        return DT_STRING;
    }

    if (!symbol->local_data_type && !symbol->data_type)
        return DT_UNDEFINED_SYMBOL;

    // Important to notice that if it is local, then it is a scalar
    if (!symbol->local_data_type && symbol->identifier_type != IT_SCALAR)
        return DT_INVALID_IDENTIFIER_TYPE;

    return symbol->local_data_type ? symbol->local_data_type : symbol->data_type;
}

DATA_TYPE infer_type_symbol_vector(AST *node)
{
    AST *symbol_node = node->child[0];
    AST *expression_node = node->child[1];

    // Check for earlier error
    DATA_TYPE expression_dt = infer_type(expression_node);
    ASSERT_BASIC_DATA_TYPE(expression_dt, expression_dt);

    // This means that this was not defined
    if (!symbol_node->symbol->local_data_type && !symbol_node->symbol->data_type)
        return DT_UNDEFINED_SYMBOL;

    // Important to notice that if it is local, then it is a scalar
    if (symbol_node->symbol->local_data_type || symbol_node->symbol->identifier_type != IT_VECTOR)
        return DT_INVALID_IDENTIFIER_TYPE;

    // This means there is an error when acessing it
    if (!is_integer(expression_dt))
        return DT_WRONG_INDEX_TYPE;

    // Valid type
    return symbol_node->symbol->local_data_type
               ? symbol_node->symbol->local_data_type
               : symbol_node->symbol->data_type;
}

DATA_TYPE infer_type_arithmetic(AST *node)
{
    // First check if some of the early checkings already returned an error
    DATA_TYPE left_op = infer_type(node->child[0]);
    ASSERT_BASIC_DATA_TYPE(left_op, left_op);

    DATA_TYPE right_op = infer_type(node->child[1]);
    ASSERT_BASIC_DATA_TYPE(right_op, right_op);

    // Already checks for arithmetic values
    return higher_type_for_arithmetic(left_op, right_op);
}

DATA_TYPE infer_type_comparator(AST *node)
{
    // First check if some of the early checkings already returned an error
    DATA_TYPE left_op = infer_type(node->child[0]);
    ASSERT_BASIC_DATA_TYPE(left_op, left_op);

    DATA_TYPE right_op = infer_type(node->child[1]);
    ASSERT_BASIC_DATA_TYPE(right_op, right_op);

    ASSERT_ARITHMETIC(left_op, DT_NON_ARITHMETIC);
    ASSERT_ARITHMETIC(right_op, DT_NON_ARITHMETIC);

    return DT_BOOL;
}

DATA_TYPE infer_type_boolean(AST *node)
{
    // First check if some of the early checkings already returned an error
    DATA_TYPE left_op = infer_type(node->child[0]);
    ASSERT_BASIC_DATA_TYPE(left_op, left_op);

    DATA_TYPE right_op = infer_type(node->child[1]);
    ASSERT_BASIC_DATA_TYPE(right_op, right_op);

    if (is_boolean(left_op) && is_boolean(right_op))
        return DT_BOOL;

    return DT_NON_BOOLEAN;
}

DATA_TYPE infer_type_comparison(AST *node)
{
    // First check if some of the early checkings already returned an error
    DATA_TYPE left_op = infer_type(node->child[0]);
    ASSERT_BASIC_DATA_TYPE(left_op, left_op);

    DATA_TYPE right_op = infer_type(node->child[1]);
    ASSERT_BASIC_DATA_TYPE(right_op, right_op);

    return DT_BOOL;
}

DATA_TYPE infer_type_lcommand(AST *node)
{
    AST *command = node;
    while (command)
    {
        // The only problem with this, is that we will only check for the first problem inside the block
        DATA_TYPE command_dt = infer_type(command->child[0]);
        ASSERT_BASIC_DATA_TYPE(command_dt, command_dt);

        command = command->child[1];
    }

    return DT_NONE;
}

DATA_TYPE infer_type_boolean_block(AST *node)
{
    AST *expr = node->child[0];
    DATA_TYPE expr_dt = infer_type(expr);
    ASSERT_BASIC_DATA_TYPE(expr_dt, expr_dt);

    if (!is_boolean(expr_dt))
        return DT_NON_BOOLEAN;

    // The only problems with this, is that we only find the first problem of
    // the block (i.e, if we have one error each, we will only report the first one only)
    AST *commands[] = {node->child[1], node->child[2], node->child[3], node->child[4]};
    for (int i = 0; i < 4; i++)
    {
        AST *command = commands[i];
        if (!command)
            break;

        DATA_TYPE command_dt = infer_type(command);
        ASSERT_BASIC_DATA_TYPE(command_dt, command_dt);

        command = command->child[1];
    }

    return DT_NONE;
}

DATA_TYPE infer_type_add(AST *node)
{
    return infer_type_arithmetic(node);
}

DATA_TYPE infer_type_sub(AST *node)
{
    return infer_type_arithmetic(node);
}

DATA_TYPE infer_type_multiply(AST *node)
{
    return infer_type_arithmetic(node);
}

DATA_TYPE infer_type_divide(AST *node)
{
    return infer_type_arithmetic(node);
}

DATA_TYPE infer_type_lt(AST *node)
{
    return infer_type_comparator(node);
}

DATA_TYPE infer_type_gt(AST *node)
{
    return infer_type_comparator(node);
}

DATA_TYPE infer_type_or(AST *node)
{
    return infer_type_boolean(node);
}

DATA_TYPE infer_type_and(AST *node)
{
    return infer_type_boolean(node);
}

DATA_TYPE infer_type_le(AST *node)
{
    return infer_type_comparator(node);
}

DATA_TYPE infer_type_ge(AST *node)
{
    return infer_type_comparator(node);
}

DATA_TYPE infer_type_eq(AST *node)
{
    return infer_type_comparison(node);
}

DATA_TYPE infer_type_dif(AST *node)
{
    return infer_type_comparison(node);
}

DATA_TYPE infer_type_unary_minus(AST *node)
{
    // First check if some of the early checkings already returned an error
    DATA_TYPE op = infer_type(node->child[0]);
    ASSERT_BASIC_DATA_TYPE(op, op);
    ASSERT_ARITHMETIC(op, DT_NON_ARITHMETIC);

    return op;
}

DATA_TYPE infer_type_unary_negation(AST *node)
{
    // First check if some of the early checkings already returned an error
    DATA_TYPE op = infer_type(node->child[0]);
    ASSERT_BASIC_DATA_TYPE(op, op);

    if (is_boolean(op))
        return op;

    return DT_NON_BOOLEAN;
}

DATA_TYPE infer_type_func_call(AST *node)
{
    HASH_NODE *symbol = node->child[0]->symbol;

    if (!symbol->data_type)
        return DT_UNDEFINED_SYMBOL;

    // If it is local, then it is a scalar
    if (symbol->local_data_type || symbol->identifier_type != IT_FUNCTION)
        return DT_INVALID_IDENTIFIER_TYPE;

    ChainedList *params = symbol->params_data_type;
    AST *param_list = node->child[1];

    while (params && param_list)
    {
        DATA_TYPE expression_type = infer_type(param_list->child[0]);
        ASSERT_BASIC_DATA_TYPE(expression_type, expression_type);

        DATA_TYPE *param_type = (DATA_TYPE *)params->val;
        ASSERT_COMPATIBLE(*param_type, expression_type, DT_INVALID_PARAMETER_TYPE);

        params = params->next;
        param_list = param_list->child[1];
    }

    if (param_list || params)
        return DT_INVALID_PARAMETER_QUANTITY;

    return symbol->data_type;
}

DATA_TYPE infer_type_attrib(AST *node)
{
    HASH_NODE *symbol = node->child[0]->symbol;

    AST *attrib_expression = node->child[1];
    DATA_TYPE attrib_expression_type = infer_type(attrib_expression);
    ASSERT_BASIC_DATA_TYPE(attrib_expression_type, attrib_expression_type);

    // This means that this was not defined
    if (!symbol->local_data_type && !symbol->data_type)
        return DT_UNDEFINED_SYMBOL;

    // If it is local, then it is a scalar
    if (!symbol->local_data_type && symbol->identifier_type != IT_SCALAR)
        return DT_INVALID_IDENTIFIER_TYPE;

    DATA_TYPE dt = symbol->local_data_type ? symbol->local_data_type : symbol->data_type;
    if (is_compatible(attrib_expression_type, dt))
        return dt;

    return DT_INVALID_ATTRIBUTION;
}

DATA_TYPE infer_type_attrib_vec(AST *node)
{
    HASH_NODE *symbol = node->child[0]->symbol;

    AST *indexing_expression = node->child[1];
    DATA_TYPE indexing_expression_type = infer_type(indexing_expression);
    ASSERT_BASIC_DATA_TYPE(indexing_expression_type, indexing_expression_type);

    AST *attrib_expression = node->child[2];
    DATA_TYPE attrib_expression_type = infer_type(attrib_expression);
    ASSERT_BASIC_DATA_TYPE(attrib_expression_type, attrib_expression_type);

    // This means that this was not defined
    if (!symbol->local_data_type && !symbol->data_type)
        return DT_UNDEFINED_SYMBOL;

    // If it is local, then it is a scalar
    if (symbol->local_data_type || symbol->identifier_type != IT_VECTOR)
        return DT_INVALID_IDENTIFIER_TYPE;

    DATA_TYPE dt = symbol->local_data_type ? symbol->local_data_type : symbol->data_type;
    if (is_compatible(attrib_expression_type, dt))
        return dt;

    return DT_INVALID_ATTRIBUTION;
}

DATA_TYPE infer_type_return(AST *node)
{
    // It has the value of the returned expression
    return infer_type(node->child[0]);
}

DATA_TYPE infer_type_read(AST *node)
{
    // Just need to infer that we are reading to the right place
    return infer_type(node->child[0]);
}

DATA_TYPE infer_type_print(AST *node)
{
    AST *params = node->child[0];
    while (params)
    {
        AST *expr = params->child[0];
        DATA_TYPE expr_dt = infer_type(expr);
        ASSERT_BASIC_DATA_TYPE(expr_dt, expr_dt);

        params = params->child[1];
    }

    return DT_NONE;
}

DATA_TYPE infer_type_if(AST *node)
{
    return infer_type_boolean_block(node);
}

DATA_TYPE infer_type_if_else(AST *node)
{
    return infer_type_boolean_block(node);
}

DATA_TYPE infer_type_while(AST *node)
{
    return infer_type_boolean_block(node);
}

DATA_TYPE infer_type_loop(AST *node)
{
    AST *identifier = node->child[0];
    DATA_TYPE identifier_dt = infer_type(identifier);
    ASSERT_BASIC_DATA_TYPE(identifier_dt, identifier_dt);
    ASSERT_ARITHMETIC(identifier_dt, DT_NON_ARITHMETIC);

    AST *identifier_expr = node->child[1];
    DATA_TYPE identifier_expr_dt = infer_type(identifier_expr);
    ASSERT_BASIC_DATA_TYPE(identifier_expr_dt, identifier_expr_dt);
    ASSERT_ARITHMETIC(identifier_expr_dt, DT_NON_ARITHMETIC);

    AST *condition_expr = node->child[2];
    DATA_TYPE condition_expr_dt = infer_type(condition_expr);
    ASSERT_BASIC_DATA_TYPE(condition_expr_dt, condition_expr_dt);
    ASSERT_ARITHMETIC(condition_expr_dt, DT_NON_ARITHMETIC);

    AST *loop_expr = node->child[3];
    DATA_TYPE loop_expr_dt = infer_type(loop_expr);
    ASSERT_BASIC_DATA_TYPE(loop_expr_dt, loop_expr_dt);
    ASSERT_ARITHMETIC(loop_expr_dt, DT_NON_ARITHMETIC);

    AST *command = node->child[4];
    DATA_TYPE command_dt = infer_type(command);
    ASSERT_BASIC_DATA_TYPE(command_dt, command_dt);

    return DT_NONE;
}

DATA_TYPE infer_type(AST *node)
{
    DATA_TYPE dt = DT_NOT_INFERED;

    switch (node->type)
    {
    case AST_SYMBOL:
        dt = infer_type_symbol(node);
        break;
    case AST_SYMBOL_VECTOR:
        dt = infer_type_symbol_vector(node);
        break;
    case AST_ADD:
        dt = infer_type_add(node);
        break;
    case AST_SUB:
        dt = infer_type_sub(node);
        break;
    case AST_MULTIPLY:
        dt = infer_type_multiply(node);
        break;
    case AST_DIVIDE:
        dt = infer_type_divide(node);
        break;
    case AST_LT:
        dt = infer_type_lt(node);
        break;
    case AST_GT:
        dt = infer_type_gt(node);
        break;
    case AST_OR:
        dt = infer_type_or(node);
        break;
    case AST_AND:
        dt = infer_type_and(node);
        break;
    case AST_LE:
        dt = infer_type_le(node);
        break;
    case AST_GE:
        dt = infer_type_ge(node);
        break;
    case AST_EQ:
        dt = infer_type_eq(node);
        break;
    case AST_DIF:
        dt = infer_type_dif(node);
        break;
    case AST_UNARY_MINUS:
        dt = infer_type_unary_minus(node);
        break;
    case AST_UNARY_NEGATION:
        dt = infer_type_unary_negation(node);
        break;
    case AST_FUNC_CALL:
        dt = infer_type_func_call(node);
        break;
    case AST_ATTRIB:
        dt = infer_type_attrib(node);
        break;
    case AST_ATTRIB_VEC:
        dt = infer_type_attrib_vec(node);
        break;
    case AST_READ:
        dt = infer_type_read(node);
        break;
    case AST_PRINT:
        dt = infer_type_print(node);
        break;
    case AST_RETURN:
        dt = infer_type_return(node);
        break;
    case AST_IF:
        dt = infer_type_if(node);
        break;
    case AST_IF_ELSE:
        dt = infer_type_if_else(node);
        break;
    case AST_WHILE:
        dt = infer_type_while(node);
        break;
    case AST_LOOP:
        dt = infer_type_loop(node);
        break;
    case AST_LCOMMAND:
        dt = infer_type_lcommand(node);
        break;
    default:
        fprintf(stderr, "ERROR WHILE INFERING NODE AST %d\n", node->type);
        break;
    }

    node->data_type = dt;
    return dt;
}