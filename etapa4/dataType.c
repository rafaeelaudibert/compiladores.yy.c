#include "dataType.h"

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
        return 0;
    }
}

inline int is_basic_data_type(DATA_TYPE dt)
{
    return dt < DT_ERRORS;
}