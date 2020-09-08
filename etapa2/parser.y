
%token KW_CHAR
%token KW_INT
%token KW_FLOAT
%token KW_BOOL

%token KW_IF
%token KW_THEN
%token KW_ELSE
%token KW_WHILE
%token KW_LOOP
%token KW_READ
%token KW_PRINT
%token KW_RETURN

%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_DIF
%token TK_IDENTIFIER

%token LIT_INTEGER
%token LIT_FLOAT
%token LIT_TRUE
%token LIT_FALSE
%token LIT_CHAR
%token LIT_STRING

%token TOKEN_ERROR

 // Precedence rules
%left '|' '^'
%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF
%left '+' '-'
%left '*' '/'
%left '~'



%{
// Declarations to get rid of warnings about not finding function when compiling
// It will work just fine in the linking step of the compilation
int yylex();
int yyerror();
%}

%%

 /* First rule, defining a program */
programa: ldecl
    ;

 /* Always finish a declaration with a `;`, and also accept an empty ldecl */
ldecl: declv ';' ldecl
    | declf ';' ldecl
    |
    ;

 /* Helper rule to define both booleans */
lit_bool: LIT_TRUE
    | LIT_FALSE
    ;

 /* Helper rule with all type keywords */
any_kw_type: KW_CHAR
    | KW_INT
    | KW_FLOAT
    | KW_BOOL
    ;

/* Helper rule with all literals keywords, except string */
any_lit: LIT_CHAR
    | LIT_INTEGER
    | LIT_FLOAT
    | lit_bool
    ;

 /* Variable declaration */
declv: declv_not_vector
    | declv_vector
    ;

 /* Variable declaration when is not a vector */
declv_not_vector: TK_IDENTIFIER '=' any_kw_type ':' any_lit
    ;

 /* Variable declaration when is not a vector */
 /* We have the rule with initialization first, and without initialization later */
declv_vector: TK_IDENTIFIER '=' any_kw_type '[' LIT_INTEGER ']' ':' lvector
    | TK_IDENTIFIER '=' any_kw_type '[' LIT_INTEGER ']'
    ;

lvector: any_lit lvector
    | any_lit
    ;

declf: function_header block
    ;

function_header: TK_IDENTIFIER function_params '=' any_kw_type
    ;

function_params: '(' lparam ')'
    | '(' ')'
    ;

lparam: TK_IDENTIFIER '=' any_kw_type rest_params
    ;

rest_params: ',' lparam
    |
    ;

block: '{' lcommand '}'
    ;

lcommand: command lcommand
    | command
    ;

command: attrib
    | flow_command
    | read_command
    | print_command
    | return_command
    | block
    |
    ;

attrib: TK_IDENTIFIER '=' expression
    | TK_IDENTIFIER '[' expression ']' '=' expression
    ;

read_command: KW_READ TK_IDENTIFIER
    ;

print_command: KW_PRINT lprint
    ;

lprint: LIT_STRING ',' lprint
    | LIT_STRING
    | expression ',' lprint
    | expression
    ;

return_command: KW_RETURN expression
    ;


flow_command: KW_IF '(' expression ')' KW_THEN command
    | KW_IF '(' expression ')' KW_THEN command KW_ELSE command
    | KW_WHILE '(' expression ')' command
    | KW_LOOP '(' TK_IDENTIFIER ':' expression ',' expression ',' expression ')' command
    ;

expression: TK_IDENTIFIER
    | TK_IDENTIFIER '[' expression ']'
    | any_lit
    | '(' expression ')'
    | '-' expression
    | '~' expression
    | expression '+' expression
    | expression '-' expression
    | expression '*' expression
    | expression '/' expression
    | expression '<' expression
    | expression '>' expression
    | expression '|' expression
    | expression '^' expression
    | expression OPERATOR_LE expression
    | expression OPERATOR_GE expression
    | expression OPERATOR_EQ expression
    | expression OPERATOR_DIF expression
    | func_call
    ;

func_call: TK_IDENTIFIER '(' lparameter_func_call ')'
    | TK_IDENTIFIER  '(' ')'
    ;

lparameter_func_call: expression rest_params_func_call
    ;

rest_params_func_call: ',' lparameter_func_call
    |
    ;


%%

#include <stdio.h>
#include <stdlib.h>

// Referencing the function available at scanner.l
extern int getLineNumber();

int yyerror()
{
    fprintf(stderr, "Syntax error at line %d\n", getLineNumber());

    exit(3);
}