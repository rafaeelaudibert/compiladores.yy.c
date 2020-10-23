#ifndef TYPE_INFER_H
#define TYPE_INFER_H

#include "chainedList.h"
#include "dataType.h"
#include "identifierType.h"
#include "ast.h"

DATA_TYPE infer_type(AST *ast);

#endif