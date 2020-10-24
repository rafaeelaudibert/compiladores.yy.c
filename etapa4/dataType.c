#include "dataType.h"
#include "lex.yy.h"

inline int is_arithmetic(DATA_TYPE dt)
{
    return dt == DT_INT || dt == DT_FLOAT || dt == DT_CHAR;
}

inline int is_integer(DATA_TYPE dt)
{
    return dt == DT_INT || dt == DT_CHAR;
}

inline int is_float(DATA_TYPE dt)
{
    return dt == DT_FLOAT;
}

inline int is_boolean(DATA_TYPE dt)
{
    return dt == DT_BOOL;
}

inline int is_string(DATA_TYPE dt)
{
    return dt == DT_STRING;
}

inline int is_compatible(DATA_TYPE dt, DATA_TYPE other)
{
    switch (dt)
    {
    case DT_INT:
    case DT_FLOAT:
    case DT_CHAR:
        return is_arithmetic(other);
    case DT_BOOL:
        return is_boolean(other);
    case DT_STRING:
        return is_string(other);
    default:
        return FALSE;
    }
}

inline int is_basic_data_type(DATA_TYPE dt)
{
    return dt < DT_ERRORS;
}

/// @param lit An integer representing a literal type (one of LIT_INTEGER, LIT_FLOAT, LIT_CHAR, LIT_TRUE, LIT_FALSE)
///
/// @returns A datatype corresponding to the [lit] mapping
DATA_TYPE map_lit_to_dt(int lit)
{
    switch (lit)
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
    }

    return DT_NONE;
}

/// @param kw An integer representing a keyword (one of KW_INT, KW_FLOAT, KW_CHAR, KW_BOOL)
///
/// @returns A datatype corresponding to the [kw] mapping
DATA_TYPE map_kw_to_dt(int kw)
{
    switch (kw)
    {
    case KW_INT:
        return DT_INT;
    case KW_FLOAT:
        return DT_FLOAT;
    case KW_CHAR:
        return DT_CHAR;
    case KW_BOOL:
        return DT_BOOL;
    }

    return DT_NONE;
}