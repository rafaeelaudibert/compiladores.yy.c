#include "codegen.h"
#include "lex.yy.h"

#define RES_OR_NULL(tac) (tac ? tac->res : NULL)
#define GENERATE_TAC_LABEL(label) tac_create(TAC_LABEL, label, NULL, NULL)
#define GENERATE_INCOND_JUMP(label) tac_create(TAC_JUMP, label ? label->res : NULL, NULL, NULL)
#define GENERATE_COND_JUMP(label, expr) tac_create(TAC_JUMPFALSE, label ? label->res : NULL, expr ? expr->res : NULL, NULL)

TAC *generate_attribution(TAC *name, TAC *expr)
{
    TAC *copy = tac_create(TAC_COPY, RES_OR_NULL(name), RES_OR_NULL(expr), NULL);

    return tac_join(tac_join(name, expr), copy);
}

TAC *generate_attribution_vector(TAC *name, TAC *offset, TAC *expr)
{
    TAC *copy = tac_create(TAC_COPY_IDX, RES_OR_NULL(name), RES_OR_NULL(expr), RES_OR_NULL(offset));

    return tac_join(tac_join(tac_join(name, offset), expr), copy);
}

TAC *generate_function(HASH_NODE *name, TAC *body)
{
    TAC *beginfun = tac_create(TAC_BEGINFUN, name, NULL, NULL),
        *endfun = tac_create(TAC_ENDFUN, NULL, NULL, NULL);

    return tac_join(tac_join(beginfun, body), endfun);
}

TAC *generate_vector_access(TAC *name, TAC *expr)
{
    TAC *tac_symbol_vector = tac_create(TAC_VECTOR_ACCESS, make_temp(), RES_OR_NULL(name), RES_OR_NULL(expr));

    return tac_join(tac_join(name, expr), tac_symbol_vector);
}

TAC *generate_boolean_arithmetic_operator(TAC_TYPE type, TAC *op1, TAC *op2)
{
    TAC *operations = tac_join(op1, op2),
        *operation = tac_create(type, make_temp(), RES_OR_NULL(op1), RES_OR_NULL(op2));

    return tac_join(operations, operation);
}

TAC *generate_unary_operator(TAC_TYPE type, TAC *op)
{
    TAC *operation = tac_create(type, make_temp(), RES_OR_NULL(op), NULL);

    return tac_join(op, operation);
}

TAC *generate_func_call(TAC *name, TAC *body)
{
    TAC *func_call = tac_create(TAC_FUNC_CALL, make_temp(), RES_OR_NULL(name), NULL);

    return tac_join(body, func_call);
}

TAC *generate_push(TAC *expression, TAC *other_push)
{

    TAC *push = tac_create(TAC_PUSH, NULL, RES_OR_NULL(expression), NULL),
        *expr_and_push = tac_join(expression, push);

    return tac_join(expr_and_push, other_push);
}

TAC *generate_read(TAC *children)
{
    TAC *tac_read = tac_create(TAC_READ, RES_OR_NULL(children), NULL, NULL);

    return tac_join(children, tac_read);
}

TAC *generate_print(TAC *expr, TAC *children)
{
    TAC *tac_print = tac_create(TAC_PRINT, NULL, RES_OR_NULL(expr), NULL);

    return tac_join(tac_join(expr, tac_print), children);
}

TAC *generate_return(TAC *expr)
{
    TAC *tac_return = tac_create(TAC_RETURN, NULL, RES_OR_NULL(expr), NULL);

    return tac_join(expr, tac_return);
}

TAC *generate_if_else(TAC *expr, TAC *body_if, TAC *body_else)
{
    HASH_NODE *label_else = make_label();
    HASH_NODE *label_after_else = make_label();

    // TACs for labels
    TAC *tac_label_else = GENERATE_TAC_LABEL(label_else);
    TAC *tac_label_after_else = GENERATE_TAC_LABEL(label_after_else);

    // We jump to the Else, if expression is false
    TAC *tac_jumpfalse = GENERATE_COND_JUMP(tac_label_else, expr);

    // Placed at the end of the IF block, to jump to after the else block
    TAC *tac_jump = GENERATE_INCOND_JUMP(tac_label_after_else);

    TAC *expr_and_jump_to_else = tac_join(expr, tac_jumpfalse);
    TAC *if_block = tac_join(body_if, tac_jump);
    TAC *else_block = tac_join(tac_join(tac_label_else, body_else), tac_label_after_else);

    TAC *full_if_else = tac_join(tac_join(expr_and_jump_to_else, if_block), else_block);

    return full_if_else;
}

TAC *generate_while(TAC *expr, TAC *body)
{
    HASH_NODE *label_before = make_label();
    HASH_NODE *label_after = make_label();

    // TACs for labels
    TAC *tac_label_before = GENERATE_TAC_LABEL(label_before);
    TAC *tac_label_after = GENERATE_TAC_LABEL(label_after);

    // We jump to after, if expression false
    TAC *tac_jumpfalse = GENERATE_COND_JUMP(tac_label_after, expr);

    // We jump inconditionally to start at the end of while
    TAC *tac_jump_before = GENERATE_INCOND_JUMP(tac_label_before);

    TAC *label_expr_and_jump = tac_join(tac_label_before, tac_join(expr, tac_jumpfalse));
    TAC *block_with_jump_and_label = tac_join(tac_join(body, tac_jump_before), tac_label_after);

    TAC *full_while = tac_join(label_expr_and_jump, block_with_jump_and_label);

    return full_while;
}

TAC *generate_loop(TAC *symbol, TAC *initializer, TAC *expr, TAC *step, TAC *body)
{
    HASH_NODE *label_cmp = make_label();
    HASH_NODE *label_after = make_label();

    // Label TACs
    TAC *tac_label_cmp = GENERATE_TAC_LABEL(label_cmp);
    TAC *tac_label_after = GENERATE_TAC_LABEL(label_after);

    // TACs for the initialization
    TAC *tac_initialization = tac_join(
        tac_join(symbol, initializer),
        tac_create(TAC_COPY, symbol->res, initializer->res, NULL));

    // TACs for the comparation, and its jump if false. We don't use the
    // `generate_boolean_arithmetic_operator` because it will add symbol to the
    // TAC again, causing an infinite loop
    TAC *tac_comparator = tac_create(TAC_LT, RES_OR_NULL(symbol), RES_OR_NULL(expr), NULL);

    TAC *tac_comparator_with_jump = tac_join(
        tac_label_cmp,
        tac_join(
            tac_comparator,
            GENERATE_COND_JUMP(tac_label_after, tac_comparator)));

    // TACs for the body with incrementation, and jump to the beginning
    // Note that we dont use `generate_boolean_arithmetic_operator` here, as we add the value
    // directly to the symbol
    TAC *tac_increment = tac_join(
        step,
        tac_create(TAC_ADD, RES_OR_NULL(symbol), RES_OR_NULL(symbol), RES_OR_NULL(step)));

    TAC *tac_jump_comparator = GENERATE_INCOND_JUMP(tac_label_cmp);

    TAC *tac_body_with_increment_and_jump = tac_join(
        tac_join(
            tac_join(body, tac_increment),
            tac_jump_comparator),
        tac_label_after);

    // TAC with everything connected: initialization, comparation, body

    TAC *initialization_comparation_body = tac_join(
        tac_join(
            tac_initialization,
            tac_comparator_with_jump),
        tac_body_with_increment_and_jump);

    return initialization_comparation_body;
}

TAC *generate_code(AST *node)
{
    if (!node)
        return NULL;

    TAC *children[MAX_CHILDREN];
    for (int i = 0; i < MAX_CHILDREN; i++)
        children[i] = generate_code(node->child[i]);

    TAC *result = NULL;

    switch (node->type)
    {
    case AST_SYMBOL:
        result = tac_create(TAC_SYMBOL, node->symbol, NULL, NULL);
        break;
    case AST_SYMBOL_VECTOR:
        result = generate_vector_access(children[0], children[1]);
        break;
    case AST_FUNC:
        result = generate_function(node->child[0]->child[0]->symbol, children[1]);
        break;
    case AST_ADD:
        result = generate_boolean_arithmetic_operator(TAC_ADD, children[0], children[1]);
        break;
    case AST_SUB:
        result = generate_boolean_arithmetic_operator(TAC_SUB, children[0], children[1]);
        break;
    case AST_MULTIPLY:
        result = generate_boolean_arithmetic_operator(TAC_MULTIPLY, children[0], children[1]);
        break;
    case AST_DIVIDE:
        result = generate_boolean_arithmetic_operator(TAC_DIVIDE, children[0], children[1]);
        break;
    case AST_LT:
        result = generate_boolean_arithmetic_operator(TAC_LT, children[0], children[1]);
        break;
    case AST_GT:
        result = generate_boolean_arithmetic_operator(TAC_GT, children[0], children[1]);
        break;
    case AST_OR:
        result = generate_boolean_arithmetic_operator(TAC_OR, children[0], children[1]);
        break;
    case AST_AND:
        result = generate_boolean_arithmetic_operator(TAC_AND, children[0], children[1]);
        break;
    case AST_LE:
        result = generate_boolean_arithmetic_operator(TAC_LE, children[0], children[1]);
        break;
    case AST_GE:
        result = generate_boolean_arithmetic_operator(TAC_GE, children[0], children[1]);
        break;
    case AST_EQ:
        result = generate_boolean_arithmetic_operator(TAC_EQ, children[0], children[1]);
        break;
    case AST_DIF:
        result = generate_boolean_arithmetic_operator(TAC_DIF, children[0], children[1]);
        break;
    case AST_UNARY_MINUS:
        result = generate_unary_operator(TAC_UNARY_MINUS, children[0]);
        break;
    case AST_UNARY_NEGATION:
        result = generate_unary_operator(TAC_UNARY_NEGATION, children[0]);
        break;
    case AST_FUNC_CALL:
        result = generate_func_call(children[0], children[1]);
        break;
    case AST_LPARAMETER:
        result = generate_push(children[0], children[1]);
        break;

    case AST_ATTRIB:
        result = generate_attribution(children[0], children[1]);
        break;
    case AST_ATTRIB_VEC:
        result = generate_attribution_vector(children[0], children[1], children[2]);
        break;
    case AST_READ:
        result = generate_read(children[0]);
        break;
    case AST_PRINT_PARAMS:
        result = generate_print(children[0], children[1]);
        break;
    case AST_RETURN:
        result = generate_return(children[0]);
        break;
    case AST_IF:
    case AST_IF_ELSE:
        // We can treat them the same way, imagining there is an empty else block
        result = generate_if_else(children[0], children[1], children[2]);
        break;
    case AST_WHILE:
        result = generate_while(children[0], children[1]);
        break;
    case AST_LOOP:
        result = generate_loop(children[0], children[1], children[2], children[3], children[4]);
        break;

    default:
        for (int i = 0; i < MAX_CHILDREN; i++)
            result = tac_join(result, children[i]);
    }

    return result;
}