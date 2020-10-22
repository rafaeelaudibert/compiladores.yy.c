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

#endif