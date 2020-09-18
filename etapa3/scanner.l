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
"char"      {
    yylval.symbol = hashInsert(yytext, KW_CHAR);
    return KW_CHAR;
}
"int"       {
    yylval.symbol = hashInsert(yytext, KW_INT);
    return KW_INT;
}
"float"     {
    yylval.symbol = hashInsert(yytext, KW_FLOAT);
    return KW_FLOAT;
}
"bool"      {
    yylval.symbol = hashInsert(yytext, KW_BOOL);
    return KW_BOOL;
}

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
"TRUE"                  {
    yylval.symbol = hashInsert(yytext, LIT_TRUE);
    return LIT_TRUE;
}
"FALSE"                 {
    yylval.symbol = hashInsert(yytext, LIT_FALSE);
    return LIT_FALSE;
}

 /* Identifiers */
[a-zA-Z@_][0-9a-zA-Z@_]*    {
    yylval.symbol = hashInsert(yytext, TK_IDENTIFIER);
    return TK_IDENTIFIER;
}

 /* Integer and float literals (hexadecimal) */
[A-F0-9]+               {
    yylval.symbol = hashInsert(yytext, LIT_INTEGER);
    return LIT_INTEGER;
}
[A-F0-9]*\.[A-F0-9]+    {
    yylval.symbol = hashInsert(yytext, LIT_FLOAT);
    return LIT_FLOAT;
}

 /* Chars and Strings */
'(\\.|[^'\n\t])'        { 
    yylval.symbol = hashInsert(yytext, LIT_CHAR);
    return LIT_CHAR;
}
\"([^"\\\n]|\\.)*\"     {
    yylval.symbol = hashInsert(yytext, LIT_STRING);
    return LIT_STRING;
}

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