#ifndef DATA_TYPE_H
#define DATA_TYPE_H

typedef enum
{
    DT_NONE,
    DT_INT,
    DT_FLOAT,
    DT_CHAR,
    DT_BOOL,
    DT_STRING
} DATA_TYPE;

int is_arithmetic(DATA_TYPE dt);
int is_integer(DATA_TYPE dt);
int is_float(DATA_TYPE dt);
int is_boolean(DATA_TYPE dt);
int is_string(DATA_TYPE dt);
int is_compatible(DATA_TYPE dt, DATA_TYPE other);

#endif