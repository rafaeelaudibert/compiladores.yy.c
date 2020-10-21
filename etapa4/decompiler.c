#include "decompiler.h"

// Private functions headers
void decompile_AST_SYMBOL(AST *, FILE *);
void decompile_AST_SYMBOL_VECTOR(AST *, FILE *);
void decompile_AST_ADD(AST *, FILE *);
void decompile_AST_SUB(AST *, FILE *);
void decompile_AST_MULTIPLY(AST *, FILE *);
void decompile_AST_DIVIDE(AST *, FILE *);
void decompile_AST_LT(AST *, FILE *);
void decompile_AST_GT(AST *, FILE *);
void decompile_AST_OR(AST *, FILE *);
void decompile_AST_AND(AST *, FILE *);
void decompile_AST_LE(AST *, FILE *);
void decompile_AST_GE(AST *, FILE *);
void decompile_AST_EQ(AST *, FILE *);
void decompile_AST_DIF(AST *, FILE *);
void decompile_AST_UNARY_MINUS(AST *, FILE *);
void decompile_AST_UNARY_NEGATION(AST *, FILE *);
void decompile_AST_FUNC_CALL(AST *, FILE *);
void decompile_AST_LPARAMETER(AST *, FILE *);
void decompile_AST_LDECL(AST *, FILE *);
void decompile_AST_DECLV_NOT_VECTOR(AST *, FILE *);
void decompile_AST_DECLV_VECTOR(AST *, FILE *);
void decompile_AST_LVECTOR(AST *, FILE *);
void decompile_AST_FUNC(AST *, FILE *);
void decompile_AST_FUNC_HEADER(AST *, FILE *);
void decompile_AST_FUNC_PARAMS(AST *, FILE *);
void decompile_AST_LCOMMAND(AST *, FILE *);
void decompile_AST_ATTRIB(AST *, FILE *);
void decompile_AST_ATTRIB_VEC(AST *, FILE *);
void decompile_AST_READ(AST *, FILE *);
void decompile_AST_PRINT(AST *, FILE *);
void decompile_AST_PRINT_PARAMS(AST *, FILE *);
void decompile_AST_RETURN(AST *, FILE *);
void decompile_AST_IF(AST *, FILE *);
void decompile_AST_IF_ELSE(AST *, FILE *);
void decompile_AST_WHILE(AST *, FILE *);
void decompile_AST_LOOP(AST *, FILE *);

void decompile(AST *node, FILE *out)
{
    if (!node)
    {
        return;
    }

    switch (node->type)
    {
    case AST_SYMBOL:
        decompile_AST_SYMBOL(node, out);
        break;
    case AST_SYMBOL_VECTOR:
        decompile_AST_SYMBOL_VECTOR(node, out);
        break;
    case AST_ADD:
        decompile_AST_ADD(node, out);
        break;
    case AST_SUB:
        decompile_AST_SUB(node, out);
        break;
    case AST_MULTIPLY:
        decompile_AST_MULTIPLY(node, out);
        break;
    case AST_DIVIDE:
        decompile_AST_DIVIDE(node, out);
        break;
    case AST_LT:
        decompile_AST_LT(node, out);
        break;
    case AST_GT:
        decompile_AST_GT(node, out);
        break;
    case AST_OR:
        decompile_AST_OR(node, out);
        break;
    case AST_AND:
        decompile_AST_AND(node, out);
        break;
    case AST_LE:
        decompile_AST_LE(node, out);
        break;
    case AST_GE:
        decompile_AST_GE(node, out);
        break;
    case AST_EQ:
        decompile_AST_EQ(node, out);
        break;
    case AST_DIF:
        decompile_AST_DIF(node, out);
        break;
    case AST_UNARY_MINUS:
        decompile_AST_UNARY_MINUS(node, out);
        break;
    case AST_UNARY_NEGATION:
        decompile_AST_UNARY_NEGATION(node, out);
        break;
    case AST_FUNC_CALL:
        decompile_AST_FUNC_CALL(node, out);
        break;
    case AST_LPARAMETER:
        decompile_AST_LPARAMETER(node, out);
        break;
    case AST_LDECL:
        decompile_AST_LDECL(node, out);
        break;
    case AST_DECLV_NOT_VECTOR:
        decompile_AST_DECLV_NOT_VECTOR(node, out);
        break;
    case AST_DECLV_VECTOR:
        decompile_AST_DECLV_VECTOR(node, out);
        break;
    case AST_LVECTOR:
        decompile_AST_LVECTOR(node, out);
        break;
    case AST_FUNC:
        decompile_AST_FUNC(node, out);
        break;
    case AST_FUNC_HEADER:
        decompile_AST_FUNC_HEADER(node, out);
        break;
    case AST_FUNC_PARAMS:
        decompile_AST_FUNC_PARAMS(node, out);
        break;
    case AST_LCOMMAND:
        decompile_AST_LCOMMAND(node, out);
        break;
    case AST_ATTRIB:
        decompile_AST_ATTRIB(node, out);
        break;
    case AST_ATTRIB_VEC:
        decompile_AST_ATTRIB_VEC(node, out);
        break;
    case AST_READ:
        decompile_AST_READ(node, out);
        break;
    case AST_PRINT:
        decompile_AST_PRINT(node, out);
        break;
    case AST_PRINT_PARAMS:
        decompile_AST_PRINT_PARAMS(node, out);
        break;
    case AST_RETURN:
        decompile_AST_RETURN(node, out);
        break;
    case AST_IF:
        decompile_AST_IF(node, out);
        break;
    case AST_IF_ELSE:
        decompile_AST_IF_ELSE(node, out);
        break;
    case AST_WHILE:
        decompile_AST_WHILE(node, out);
        break;
    case AST_LOOP:
        decompile_AST_LOOP(node, out);
        break;
    default:
        fprintf(stderr, "Found unknown AST Node while decompiling, exiting...\n");
        exit(1);
        break;
    }
}

void decompile_AST_SYMBOL(AST *node, FILE *out)
{
    fprintf(out, node->symbol->text);
    return;
}

// Example:
// [abcde, 15, NULL, NULL, NULL]
// abcde[(15)]
void decompile_AST_SYMBOL_VECTOR(AST *node, FILE *out)
{
    decompile(node->child[0], out);
    fprintf(out, "[(");
    decompile(node->child[1], out);
    fprintf(out, ")]");
    return;
}

// Example:
// [18, 19, NULL, NULL, NULL]
// (18) + (19)
void decompile_AST_ADD(AST *node, FILE *out)
{
    fprintf(out, "(");
    decompile(node->child[0], out);
    fprintf(out, ") + (");
    decompile(node->child[1], out);
    fprintf(out, ")");
    return;
}

// Example:
// [18, 19, NULL, NULL, NULL]
// (18) - (19)
void decompile_AST_SUB(AST *node, FILE *out)
{
    fprintf(out, "(");
    decompile(node->child[0], out);
    fprintf(out, ") - (");
    decompile(node->child[1], out);
    fprintf(out, ")");
    return;
}

// Example:
// [18, 19, NULL, NULL, NULL]
// (18) * (19)
void decompile_AST_MULTIPLY(AST *node, FILE *out)
{
    fprintf(out, "(");
    decompile(node->child[0], out);
    fprintf(out, ") * (");
    decompile(node->child[1], out);
    fprintf(out, ")");
    return;
}

// Example:
// [18, 19, NULL, NULL, NULL]
// (18) / (19)
void decompile_AST_DIVIDE(AST *node, FILE *out)
{
    fprintf(out, "(");
    decompile(node->child[0], out);
    fprintf(out, ") / (");
    decompile(node->child[1], out);
    fprintf(out, ")");
    return;
}

// Example:
// [18, 19, NULL, NULL, NULL]
// (18) < (19)
void decompile_AST_LT(AST *node, FILE *out)
{
    fprintf(out, "(");
    decompile(node->child[0], out);
    fprintf(out, ") < (");
    decompile(node->child[1], out);
    fprintf(out, ")");
    return;
}

// Example:
// [18, 19, NULL, NULL, NULL]
// (18) - (19)
void decompile_AST_GT(AST *node, FILE *out)
{
    fprintf(out, "(");
    decompile(node->child[0], out);
    fprintf(out, ") > (");
    decompile(node->child[1], out);
    fprintf(out, ")");
    return;
}

// Example:
// [18, 19, NULL, NULL, NULL]
// (18) | (19)
void decompile_AST_OR(AST *node, FILE *out)
{
    fprintf(out, "(");
    decompile(node->child[0], out);
    fprintf(out, ") | (");
    decompile(node->child[1], out);
    fprintf(out, ")");
    return;
}

// Example:
// [18, 19, NULL, NULL, NULL]
// (18) ^ (19)
void decompile_AST_AND(AST *node, FILE *out)
{
    fprintf(out, "(");
    decompile(node->child[0], out);
    fprintf(out, ") ^ (");
    decompile(node->child[1], out);
    fprintf(out, ")");
    return;
}

// Example:
// [18, 19, NULL, NULL, NULL]
// (18) <= (19)
void decompile_AST_LE(AST *node, FILE *out)
{
    fprintf(out, "(");
    decompile(node->child[0], out);
    fprintf(out, ") <= (");
    decompile(node->child[1], out);
    fprintf(out, ")");
    return;
}

// Example:
// [18, 19, NULL, NULL, NULL]
// (18) >= (19)
void decompile_AST_GE(AST *node, FILE *out)
{
    fprintf(out, "(");
    decompile(node->child[0], out);
    fprintf(out, ") >= (");
    decompile(node->child[1], out);
    fprintf(out, ")");
    return;
}

// Example:
// [18, 19, NULL, NULL, NULL]
// (18) == (19)
void decompile_AST_EQ(AST *node, FILE *out)
{
    fprintf(out, "(");
    decompile(node->child[0], out);
    fprintf(out, ") == (");
    decompile(node->child[1], out);
    fprintf(out, ")");
    return;
}

// Example:
// [18, 19, NULL, NULL, NULL]
// (18) != (19)
void decompile_AST_DIF(AST *node, FILE *out)
{
    fprintf(out, "(");
    decompile(node->child[0], out);
    fprintf(out, ") != (");
    decompile(node->child[1], out);
    fprintf(out, ")");
    return;
}

// Example:
// [18, NULL, NULL, NULL, NULL]
// -(18)
void decompile_AST_UNARY_MINUS(AST *node, FILE *out)
{
    fprintf(out, "-(");
    decompile(node->child[0], out);
    fprintf(out, ")");
    return;
}

// Example:
// [18, NULL, NULL, NULL, NULL]
// ~(18)
void decompile_AST_UNARY_NEGATION(AST *node, FILE *out)
{
    fprintf(out, "~(");
    decompile(node->child[0], out);
    fprintf(out, ")");
    return;
}

// Examples:
// [func_name, lparam, NULL, NULL, NULL]
// func_name(lparam)
//
// [func_name, NULL, NULL, NULL, NULL]
// func_name()
void decompile_AST_FUNC_CALL(AST *node, FILE *out)
{
    decompile(node->child[0], out);
    fprintf(out, "(");
    decompile(node->child[1], out);
    fprintf(out, ")");
    return;
}

// Examples:
// [param1, lparam, NULL, NULL, NULL]
// (param1), lparam
// [param1, NULL, NULL, NULL, NULL]
// (param1)
void decompile_AST_LPARAMETER(AST *node, FILE *out)
{
    fprintf(out, "(");
    decompile(node->child[0], out);
    fprintf(out, ")");

    if (node->child[1])
    {
        fprintf(out, ",");
        decompile(node->child[1], out);
    }
    return;
}

// Examples:
// [decl, other_decl, NULL, NULL, NULL]
// decl; other_decl
// [decl, NULL, NULL, NULL, NULL]
// decl;
void decompile_AST_LDECL(AST *node, FILE *out)
{

    decompile(node->child[0], out);
    fprintf(out, ";\n");
    decompile(node->child[1], out);
    return;
}

// Examples:
// [symbol, int, 6, NULL, NULL]
// symbol = int : 6
void decompile_AST_DECLV_NOT_VECTOR(AST *node, FILE *out)
{
    decompile(node->child[0], out);
    fprintf(out, "=");
    decompile(node->child[1], out);
    fprintf(out, ":");
    decompile(node->child[2], out);
    return;
}

// Examples:
// [symbol, int, 6, list_vector, NULL]
// symbol = int[6] : list_vector
// [symbol, int, 6, NULL, NULL]
// symbol = int[6]
void decompile_AST_DECLV_VECTOR(AST *node, FILE *out)
{
    decompile(node->child[0], out);
    fprintf(out, "= ");
    decompile(node->child[1], out);
    fprintf(out, "[");
    decompile(node->child[2], out);
    fprintf(out, "]");

    if (node->child[3])
    {
        fprintf(out, ":");
        decompile(node->child[3], out);
    }
    return;
}

// Examples:
// [12, list_vector, NULL, NULL, NULL]
// 12 list_vector
// [12, NULL, NULL, NULL, NULL]
// 12
void decompile_AST_LVECTOR(AST *node, FILE *out)
{
    decompile(node->child[0], out);

    if (node->child[1])
    {
        fprintf(out, " ");
        decompile(node->child[1], out);
    }
    return;
}

// Examples:
// [header, body, NULL, NULL, NULL]
// header{body}
void decompile_AST_FUNC(AST *node, FILE *out)
{
    decompile(node->child[0], out);
    fprintf(out, "{");
    decompile(node->child[1], out);
    fprintf(out, "}");
    return;
}

// Examples:
// [func_name, params, int, NULL, NULL]
// func_name(params) = int
void decompile_AST_FUNC_HEADER(AST *node, FILE *out)
{
    decompile(node->child[0], out);
    fprintf(out, "(");
    decompile(node->child[1], out);
    fprintf(out, ")=");
    decompile(node->child[2], out);
    return;
}

// Examples:
// [func_name, params_with_parenthesis, int, NULL, NULL]
// func_name params_with_parenthesis = int
void decompile_AST_FUNC_PARAMS(AST *node, FILE *out)
{
    decompile(node->child[0], out);
    fprintf(out, "=");
    decompile(node->child[1], out);

    if (node->child[2])
    {
        fprintf(out, ",");
        decompile(node->child[2], out);
    }
    return;
}

// Examples:
// [command, lcommand, NULL, NULL, NULL]
// command lcommand
void decompile_AST_LCOMMAND(AST *node, FILE *out)
{
    decompile(node->child[0], out);
    if (node->child[1])
    {
        fprintf(out, " ");
        decompile(node->child[1], out);
    }
    return;
}

// Examples:
// [symbol, expr, NULL, NULL, NULL]
// symbol=(expr)
void decompile_AST_ATTRIB(AST *node, FILE *out)
{
    decompile(node->child[0], out);
    fprintf(out, "=(");
    decompile(node->child[1], out);
    fprintf(out, ")");
    return;
}

// Examples:
// [symbol, expr1, expr2, NULL, NULL]
// symbol[(expr1)]=(expr2)
void decompile_AST_ATTRIB_VEC(AST *node, FILE *out)
{
    decompile(node->child[0], out);
    fprintf(out, "[(");
    decompile(node->child[1], out);
    fprintf(out, ")]=(");
    decompile(node->child[2], out);
    fprintf(out, ")");
    return;
}

// Examples:
// [symbol, NULL, NULL, NULL, NULL]
// read symbol
void decompile_AST_READ(AST *node, FILE *out)
{
    fprintf(out, "read ");
    decompile(node->child[0], out);
    return;
}

// Examples:
// [print_params, NULL, NULL, NULL, NULL]
// read print_params
void decompile_AST_PRINT(AST *node, FILE *out)
{
    fprintf(out, "print ");
    decompile(node->child[0], out);
    return;
}

// Examples:
// ["abcde", NULL, NULL, NULL, NULL]
// "abcde"
// ["abcde", other_params, NULL, NULL, NULL]
// "abcde", other_params
// [expr, NULL, NULL, NULL, NULL]
// expr
// [expr, other_params, NULL, NULL, NULL]
// expr, other_params
void decompile_AST_PRINT_PARAMS(AST *node, FILE *out)
{
    decompile(node->child[0], out);

    if (node->child[1])
    {
        fprintf(out, ",");
        decompile(node->child[1], out);
    }
    return;
}

// Examples:
// [expr, NULL, NULL, NULL, NULL]
// return expr
void decompile_AST_RETURN(AST *node, FILE *out)
{
    fprintf(out, "return (");
    decompile(node->child[0], out);
    fprintf(out, ")");
    return;
}

// Examples:
// [expr, commands, NULL, NULL, NULL]
// if (expr) then {commands}
void decompile_AST_IF(AST *node, FILE *out)
{
    fprintf(out, "if (");
    decompile(node->child[0], out);
    fprintf(out, ") then {");
    decompile(node->child[1], out);
    fprintf(out, "}");
    return;
}

// Examples:
// [expr, commands, more_commands, NULL, NULL]
// if (expr) then {commands} else {more_commands}
void decompile_AST_IF_ELSE(AST *node, FILE *out)
{
    fprintf(out, "if (");
    decompile(node->child[0], out);
    fprintf(out, ") then {");
    decompile(node->child[1], out);
    fprintf(out, "} else {");
    decompile(node->child[2], out);
    fprintf(out, "}");
    return;
}

// Examples:
// [expr, commands, NULL, NULL, NULL]
// while (expr) {commands}
void decompile_AST_WHILE(AST *node, FILE *out)
{
    fprintf(out, "while (");
    decompile(node->child[0], out);
    fprintf(out, ") {");
    decompile(node->child[1], out);
    fprintf(out, "}");
    return;
}

// Examples:
// [id, expr1, expr2, expr3, commands]
// loop (id:(expr1),(expr2),(expr3)) {commands}
void decompile_AST_LOOP(AST *node, FILE *out)
{
    fprintf(out, "loop (");
    decompile(node->child[0], out);
    fprintf(out, ":(");
    decompile(node->child[1], out);
    fprintf(out, "),(");
    decompile(node->child[2], out);
    fprintf(out, "),(");
    decompile(node->child[3], out);
    fprintf(out, ")) {");
    decompile(node->child[4], out);
    fprintf(out, "}");
    return;
}