%{
#include "lex.yy.h"

int is_running = TRUE;
int line_number = 1;


int isRunning(void) {
    return is_running;
}

int getLineNumber(void) {
    return line_number;
}

%}

%x COMMENT

%%

 /* Ignore white spaces and tabs */
[ \t]

 /* Detect new lines to increase line_number counter */
"\n"        { ++line_number; }

 /* Type keywords */
"char"      { return KW_CHAR; }
"int"       { return KW_INT; }
"float"     { return KW_FLOAT; }
"bool"      { return KW_BOOL; }

 /* Flow Control keywords */
"if"        { return KW_IF; }
"then"      { return KW_THEN; }
"else"      { return KW_ELSE; }
"while"     { return KW_WHILE; }
"loop"      { return KW_LOOP; }
"return"    { return KW_RETURN; }

 /* IO keywords */
"read"      { return KW_READ; }
"print"     { return KW_PRINT; }

 /* Operators */
[\,\;\:\(\)\[\]\{\}\+\-\*\/\<\>\=\|\^\~\&\$\#]  { return yytext[0]; }

 /* Composite Operators */
"<="        { return OPERATOR_LE; }
">="        { return OPERATOR_GE; }
"=="        { return OPERATOR_EQ; }
"!="        { return OPERATOR_DIF; }

 /* Precedence for TRUE and FALSE constants instead of an identifier */
"TRUE"                  { return LIT_TRUE; }
"FALSE"                 { return LIT_FALSE; }

 /* Identifiers */
[a-zA-Z@_][0-9a-zA-Z@_]*    { hashInsert(yytext); return TK_IDENTIFIER; }

 /* Integer and float literals (hexadecimal) */
[A-F0-9]+               { hashInsert(yytext); return LIT_INTEGER; }
[A-F0-9]*\.[A-F0-9]+    { hashInsert(yytext); return LIT_FLOAT; }

 /* Chars and Strings */
'(\\.|[^'\n\t])'        { hashInsert(yytext); return LIT_CHAR; }
\"([^"\\\n]|\\.)*\"     { hashInsert(yytext); return LIT_STRING; }

 /* Comments */
"//".*
"/*"        { BEGIN(COMMENT); }

 /* On Comment mode, ignore everything until
    it reaches an end of comment token */
<COMMENT>"*/"   { BEGIN(INITIAL); }
<COMMENT>"\n"   { ++line_number; }
<COMMENT>.

.               { return TOKEN_ERROR; }

%%

#include "main.c"

int yywrap()
{
    is_running = FALSE;
    return 1;
}

void initMe(void)
{
    // Initialize hash
    hashInit();

    // Initialize variables
    is_running = TRUE;
    line_number = 1;
}