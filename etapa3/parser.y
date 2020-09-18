%{
    #include "ast.h"
    #include "hash.h"

    // Helper to create a symbol AST
    #define SYMBOL_AST(x) AST_create(AST_SYMBOL, x, NULL, NULL, NULL, NULL, NULL)

    AST* ASTroot = NULL;
%}


%union{
    HASH_NODE *symbol;
    AST *ast;
}

%token<symbol> KW_CHAR
%token<symbol> KW_INT
%token<symbol> KW_FLOAT
%token<symbol> KW_BOOL

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
%token<symbol> TK_IDENTIFIER

%token<symbol> LIT_INTEGER
%token<symbol> LIT_FLOAT
%token<symbol> LIT_TRUE
%token<symbol> LIT_FALSE
%token<symbol> LIT_CHAR
%token<symbol> LIT_STRING

%token TOKEN_ERROR

 // Precedence rules
%left '|' '^'
%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF
%left '+' '-'
%left '*' '/'
%left '~'

// Types for rules
%type<ast> programa;
%type<ast> ldecl;


%type<ast> any_lit
%type<ast> lit_bool
%type<ast> any_kw_type;

%type<ast> declv;
%type<ast> declv_not_vector;
%type<ast> declv_vector;

%type<ast> lvector;

%type<ast> declf;
%type<ast> function_header;
%type<ast> function_params;
%type<ast> lparam;
%type<ast> rest_params;

%type<ast> block;

%type<ast> lcommand;
%type<ast> command;
%type<ast> attrib;
%type<ast> return_command;
%type<ast> flow_command;
%type<ast> read_command;
%type<ast> print_command;
%type<ast> lprint;

%type<ast> expression

%type<ast> func_call
%type<ast> lparameter_func_call
%type<ast> rest_params_func_call

%{
// Declarations to get rid of warnings about not finding function when compiling
// It will work just fine in the linking step of the compilation
int yylex();
int yyerror();
%}

%%

 /* First rule, defining a program */
programa: ldecl         { $$ = $1; ASTroot = $$; }
    ;

 /* Always finish a declaration with a `;`, and also accept an empty ldecl */
ldecl: declv ';' ldecl  { $$ = AST_create( AST_LDECL, NULL, $1, $3, NULL, NULL, NULL); }
    | declf ';' ldecl   { $$ = AST_create( AST_LDECL, NULL, $1, $3, NULL, NULL, NULL); }
    |                   { $$ = NULL; }
    ;

 /* Helper rule to define both booleans */
lit_bool: LIT_TRUE      { $$ = SYMBOL_AST($1); }
    | LIT_FALSE         { $$ = SYMBOL_AST($1); }
    ;

 /* Helper rule with all type keywords */
any_kw_type: KW_CHAR    { $$ = SYMBOL_AST($1);  }
    | KW_INT            { $$ = SYMBOL_AST($1);  }
    | KW_FLOAT          { $$ = SYMBOL_AST($1);  }
    | KW_BOOL           { $$ = SYMBOL_AST($1);  }
    ;

/* Helper rule with all literals keywords, except string */
any_lit: LIT_CHAR       { $$ = SYMBOL_AST($1); }
    | LIT_INTEGER       { $$ = SYMBOL_AST($1); }
    | LIT_FLOAT         { $$ = SYMBOL_AST($1); }
    | lit_bool          { $$ = $1; }
    ;

 /* Variable declaration */
declv: declv_not_vector { $$ = $1; }
    | declv_vector      { $$ = $1; }
    ;

 /* Variable declaration when is not a vector */
declv_not_vector: TK_IDENTIFIER '=' any_kw_type ':' any_lit     { $$ = AST_create( AST_DECLV_NOT_VECTOR, NULL, SYMBOL_AST($1), $3, $5, NULL, NULL); }
    ;

 /* Variable declaration when is not a vector */
 /* We have the rule with initialization first, and without initialization later */
declv_vector: TK_IDENTIFIER '=' any_kw_type '[' LIT_INTEGER ']' ':' lvector { $$ = AST_create( AST_DECLV_VECTOR, NULL, SYMBOL_AST($1), $3, SYMBOL_AST($5), $8, NULL); }
    | TK_IDENTIFIER '=' any_kw_type '[' LIT_INTEGER ']'                     { $$ = AST_create( AST_DECLV_VECTOR, NULL, SYMBOL_AST($1), $3, SYMBOL_AST($5), NULL, NULL); }
    ;

lvector: any_lit lvector            { $$ = AST_create( AST_LVECTOR, NULL, $1, $2, NULL, NULL, NULL); }
    | any_lit                       { $$ = AST_create( AST_LVECTOR, NULL, $1, NULL, NULL, NULL, NULL); }
    ;

declf: function_header block        { $$ = AST_create( AST_FUNC, NULL, $1, $2, NULL, NULL, NULL); }
    ;

function_header: TK_IDENTIFIER function_params '=' any_kw_type  { $$ = AST_create( AST_FUNC_HEADER, NULL, SYMBOL_AST($1), $2, $4, NULL, NULL); }
    ;

function_params: '(' lparam ')'     { $$ = $2; }
    | '(' ')'                       { $$ = NULL; }
    ;

lparam: TK_IDENTIFIER '=' any_kw_type rest_params   { $$ = AST_create( AST_FUNC_PARAMS, NULL, SYMBOL_AST($1), $3, $4, NULL, NULL); }
    ;

rest_params: ',' lparam                             { $$ = $2; }
    |                                               { $$ = NULL; }
    ;

block: '{' lcommand '}'                             { $$ = $2; }
    ;

lcommand: command lcommand                          { $$ = AST_create( AST_LCOMMAND, NULL, $1, $2, NULL, NULL, NULL); }
    | command                                       { $$ = AST_create( AST_LCOMMAND, NULL, $1, NULL, NULL, NULL, NULL); }
    ;

command: attrib                                     { $$ = $1; }
    | flow_command                                  { $$ = $1; }
    | read_command                                  { $$ = $1; }
    | print_command                                 { $$ = $1; }
    | return_command                                { $$ = $1; }
    | block                                         { $$ = $1; }
    |                                               { $$ = NULL; }
    ;

attrib: TK_IDENTIFIER '=' expression                        { $$ = AST_create(AST_ATTRIB, NULL, SYMBOL_AST($1), $3, NULL, NULL, NULL); }
    | TK_IDENTIFIER '[' expression ']' '=' expression       { $$ = AST_create(AST_ATTRIB_VEC, NULL, SYMBOL_AST($1), $3, $6, NULL, NULL); }
    ;

read_command: KW_READ TK_IDENTIFIER                         { $$ = AST_create(AST_READ, NULL, SYMBOL_AST($2), NULL, NULL, NULL, NULL); }
    ;

print_command: KW_PRINT lprint                              { $$ = AST_create(AST_PRINT, NULL, $2, NULL, NULL, NULL, NULL); }
    ;

lprint: LIT_STRING ',' lprint                               { $$ = AST_create(AST_PRINT_PARAMS, NULL, SYMBOL_AST($1), $3, NULL, NULL, NULL); }
    | LIT_STRING                                            { $$ = AST_create(AST_PRINT_PARAMS, NULL, SYMBOL_AST($1), NULL, NULL, NULL, NULL); }
    | expression ',' lprint                                 { $$ = AST_create(AST_PRINT_PARAMS, NULL, $1, $3, NULL, NULL, NULL); }
    | expression                                            { $$ = AST_create(AST_PRINT_PARAMS, NULL, $1, NULL, NULL, NULL, NULL); }
    ;

return_command: KW_RETURN expression                        { $$ = AST_create(AST_RETURN, NULL, $2, NULL, NULL, NULL, NULL); }
    ;


flow_command: KW_IF '(' expression ')' KW_THEN command                                      { $$ = AST_create(AST_IF, NULL, $3, $6, NULL, NULL, NULL); }
    | KW_IF '(' expression ')' KW_THEN command KW_ELSE command                              { $$ = AST_create(AST_IF_ELSE, NULL, $3, $6, $8, NULL, NULL); }
    | KW_WHILE '(' expression ')' command                                                   { $$ = AST_create(AST_WHILE, NULL, $3, $5, NULL, NULL, NULL); }
    | KW_LOOP '(' TK_IDENTIFIER ':' expression ',' expression ',' expression ')' command    { $$ = AST_create(AST_LOOP, NULL, SYMBOL_AST($3), $5, $7, $9, $11); }
    ;

expression: TK_IDENTIFIER                       { $$ = SYMBOL_AST($1); }
    | TK_IDENTIFIER '[' expression ']'          { $$ = AST_create( AST_SYMBOL_VECTOR, NULL, SYMBOL_AST($1), $3, NULL, NULL, NULL); }
    | any_lit                                   { $$ = $1; }
    | '(' expression ')'                        { $$ = $2; }
    | '-' expression                            { $$ = AST_create(AST_UNARY_MINUS, NULL, $2, NULL, NULL, NULL, NULL); }
    | '~' expression                            { $$ = AST_create(AST_UNARY_NEGATION, NULL, $2, NULL, NULL, NULL, NULL); }
    | expression '+' expression                 { $$ = AST_create(AST_ADD, NULL, $1, $3, NULL, NULL, NULL); }
    | expression '-' expression                 { $$ = AST_create(AST_SUB, NULL, $1, $3, NULL, NULL, NULL); }
    | expression '*' expression                 { $$ = AST_create(AST_MULTIPLY, NULL, $1, $3, NULL, NULL, NULL); }
    | expression '/' expression                 { $$ = AST_create(AST_DIVIDE, NULL, $1, $3, NULL, NULL, NULL); }
    | expression '<' expression                 { $$ = AST_create(AST_LT, NULL, $1, $3, NULL, NULL, NULL); }
    | expression '>' expression                 { $$ = AST_create(AST_GT, NULL, $1, $3, NULL, NULL, NULL); }
    | expression '|' expression                 { $$ = AST_create(AST_OR, NULL, $1, $3, NULL, NULL, NULL); }
    | expression '^' expression                 { $$ = AST_create(AST_AND, NULL, $1, $3, NULL, NULL, NULL); }
    | expression OPERATOR_LE expression         { $$ = AST_create(AST_LE, NULL, $1, $3, NULL, NULL, NULL); }
    | expression OPERATOR_GE expression         { $$ = AST_create(AST_GE, NULL, $1, $3, NULL, NULL, NULL); }
    | expression OPERATOR_EQ expression         { $$ = AST_create(AST_EQ, NULL, $1, $3, NULL, NULL, NULL); }
    | expression OPERATOR_DIF expression        { $$ = AST_create(AST_DIF, NULL, $1, $3, NULL, NULL, NULL); }
    | func_call                                 { $$ = $1; }
    ;

func_call: TK_IDENTIFIER '(' lparameter_func_call ')'   { $$ = AST_create(AST_FUNC_CALL, NULL, SYMBOL_AST($1), $3, NULL, NULL, NULL); }
    | TK_IDENTIFIER  '(' ')'                            { $$ = AST_create(AST_FUNC_CALL, NULL, SYMBOL_AST($1), NULL, NULL, NULL, NULL); }
    ;

lparameter_func_call: expression rest_params_func_call              { $$ = AST_create(AST_LPARAMETER, NULL, $1, $2, NULL, NULL, NULL); }
    ;

rest_params_func_call: ',' lparameter_func_call                     { $$ = $2; }
    |                                                               { $$ = NULL; }
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