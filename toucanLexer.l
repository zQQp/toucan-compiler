%option nounput
%{
/*******************************************
 * Part of Toucan Compiler
 * by Jens Nierhaus
 * Compilerbau WS 12/13
 * LEXER FILE
 *******************************************/

#include "toucanHeader.h"
#include "y.tab.h"


%}

%%

"define"		return DEFINE;
"constructor"	return CONSTRUCTOR;
"case"			return CASE;
"of"			return OF;
"if"			return IF;
"then"			return THEN;
"else"			return ELSE;

"("				return LPAR;
")"				return RPAR;
"."				return DOT;
","				return COMMA;
"->"			return ARROW;
";"				return SEMIC;

"="				return EQ;
"+"				return ADD;
"-"				return SUB;
"*"				return MULT;
"/"				return DIV;
"=="			return DEQ;
"!="			return NEQ;
"<"				return LT;
"<="			return LE;
">"				return GT;
">="			return GE;

[a-zA-Z]+	{ 
				yylval.ident = strdup(yytext);
				return IDENTIFIER;
			}

0			{
				yylval.integer = atoi(yytext);
				return INTEGER;
			}

[1-9][0-9]*	{
				yylval.integer = atoi(yytext);
				return INTEGER;
			}

[ \t\n]+	{
				/* ignore whitespace */
			}       

.			{
				yyerror("Unknown character");
			}

%%

int yywrap(void) {
    return 1;
}
