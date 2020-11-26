#ifndef DATA_TYPE_H
#define DATA_TYPE_H

// With DT_NONE being 0, it makes it easy to use local || global (DT_NONE || DT_INT)
typedef enum
{
    DT_NONE = 0,
    DT_INT,
    DT_FLOAT,
    DT_CHAR,
    DT_BOOL,
    DT_STRING,

    // Extra types to match errors
    DT_ERRORS,
    DT_NOT_INFERED,
    DT_INVALID_IDENTIFIER_TYPE,
    DT_UNDEFINED_SYMBOL,
    DT_WRONG_INDEX_TYPE,
    DT_NON_ARITHMETIC,
    DT_NON_BOOLEAN,
    DT_INVALID_PARAMETER_QUANTITY,
    DT_INVALID_PARAMETER_TYPE,
    DT_INVALID_ATTRIBUTION
} DATA_TYPE;

int is_arithmetic(DATA_TYPE dt);
int is_integer(DATA_TYPE dt);
int is_float(DATA_TYPE dt);
int is_boolean(DATA_TYPE dt);
int is_string(DATA_TYPE dt);
int is_compatible(DATA_TYPE dt, DATA_TYPE other);
int is_basic_data_type(DATA_TYPE dt);

DATA_TYPE map_lit_to_dt(int lit);
DATA_TYPE map_kw_to_dt(int kw);

#endif