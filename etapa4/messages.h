#ifndef MESSAGES_H
#define MESSAGES_H

// Ex: Identifier abcd at line 15 being already declared before
const char *MESSAGE_REDECLARATION = "Identifier %s at line %d already declared before";

// Ex: Param abcd at line 15 already declared in this function before
const char *MESSAGE_PARAM_REDECLARATION = "Param %s at line %d already declared in this function before";

// Ex: Identifier abcd at line 15 received incompatible initialization value .0f12c
const char *MESSAGE_INCOMPATIBLE_INITIALIZATION = "Identifier %s at line %d received incompatible initialization value %s";

// Ex: Identifier abcd at line 15 should receive 10 itens in initialization but received 4
const char *MESSAGE_WRONG_NUMBER_ITENS_VECTOR = "Identifier %s at line %d should receive %d itens in initialization but received %d";

// Ex: Could not infer type at line 15
const char *MESSAGE_NOT_INFERED = "Could not infer type (error %d) at line %d";

// Ex: This type of identifier cannot be used at line 15
const char *MESSAGE_INVALID_IDENTIFIER_TYPE = "This type of identifier cannot be used at line %d";

// Ex: This symbol is not defined at line 15
const char *MESSAGE_UNDEFINED_SYMBOL = "This symbol is not defined at line %d";

// Ex: The type of the index expression is not allowed at line %d
const char *MESSAGE_WRONG_INDEX_TYPE = "The type of the index expression is not allowed at line %d";

// Ex: The operation in line 15 requires only arithmetic operators
const char *MESSAGE_NON_ARITHMETIC = "The operation in line %d requires only arithmetic operators";

// Ex: The operation in line 15 requires only boolean operators
const char *MESSAGE_NON_BOOLEAN = "The operation in line %d requires only boolean operators";

// Ex: The quantity of parameters passed to function at line 15 is wrong
const char *MESSAGE_INVALID_PARAMETER_QUANTITY = "The quantity of parameters passed to function at line %d is wrong";

// Ex: The type of a parameter passed to function at line 15 is wrong
const char *MESSAGE_INVALID_PARAMETER_TYPE = "The type of a parameter passed to function at line %d is wrong";

// Ex: An attribution at line 15 has invalid type
const char *MESSAGE_INVALID_ATTRIBUTION = "An attribution at line %d has invalid type";

// Ex: The return type at line 15 is not compatible to the function one
const char *MESSAGE_WRONG_RETURN_TYPE = "The return type at line %d is not compatible to the function one";

#endif