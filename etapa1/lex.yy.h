#include "tokens.h"

#define FALSE 0
#define TRUE 1

// Retorna TRUE caso já tenhamos finalizado de ler o arquivo,
// caso contrário, FALSE
int isRunning(void);

// Retorna em qual linha estamos atualmente ao ler o arquivo
int getLineNumber(void);

// Declaração da yylex para acessar na main depois
int yylex(void);