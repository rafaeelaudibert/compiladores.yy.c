#include "ast.h"
#include "hash.h"
#include "y.tab.h"

#define FALSE 0
#define TRUE 1

// Returns TRUE if we are already done reading the file,
// else, FALSE
int isRunning(void);

// Returns which is the current line in the file being lexed
int getLineNumber(void);

// Declare to be able to access on main.c later on
int yylex(void);

// Used to initialize any needed data
void initMe(void);