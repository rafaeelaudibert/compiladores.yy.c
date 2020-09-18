#ifndef DECOMPILER_H
#define DECOMPILER_H

#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

void decompile(AST *node, FILE *out);

#endif