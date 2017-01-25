%{
/*******************************************
 * Part of Toucan Compiler
 * by Jens Nierhaus
 * Compilerbau WS 12/13
 * YACC/BISON PARSER FILE
 *******************************************/

#include "toucanHeader.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

mNode result = NULL; // result node
%}

%error-verbose /* better errors */

%union {
    int integer;		/* integer value */
    char* ident;		/* identifier */
	mNode tmnode;       /* multi nodes */
	Li tli;             /* list */
};

%token <integer> INTEGER
%token <ident> IDENTIFIER

%token DEFINE CONSTRUCTOR CASE OF IF THEN ELSE
%token LPAR RPAR DOT COMMA ARROW SEMIC
%token EQ ADD SUB MULT DIV
%token DEQ NEQ LT LE GT GE

%type <tli> params args funDeclaration
%type <tmnode> program expression atom
%type <tmnode> ifExpression addExpression multExpression

%%

/* CMaj7 rules for bottom up parser */
/* rules contain no epsilon :) */

program:
        funDeclaration expression	
            {result = fProgram($1, $2);}
            
        | expression	
            {result = fProgram(newNil(), $1);}
            
        ;
            
funDeclaration:
        DEFINE IDENTIFIER LPAR RPAR EQ expression DOT
            {$$ = newCons(fFunDef($2,newNil(),$6),newNil());}
            
        | DEFINE IDENTIFIER LPAR params RPAR EQ expression DOT
            {$$ = newCons(fFunDef($2,$4,$7),newNil());}
            
        | funDeclaration DEFINE IDENTIFIER LPAR RPAR EQ expression DOT
            {$$ = newCons(fFunDef($3,newNil(),$7),$1);}
            
        | funDeclaration DEFINE IDENTIFIER LPAR params RPAR EQ expression DOT
            {$$ = newCons(fFunDef($3,$5,$8),$1);}
            
        ;
        
params: 
        IDENTIFIER
            {$$ = newCons(fVariable($1),newNil());}
            
        | params COMMA IDENTIFIER
            {$$ = newCons(fVariable($3), $1);}
            
        ;

expression:
        addExpression	
            {$$ = $1;}
        
        | ifExpression	
            {$$ = $1;}
            
        ;

ifExpression:
        IF expression THEN expression ELSE atom	
            {$$ = fIfExp($2,$4,$6);}
            
        ;

addExpression:
		multExpression
		    {$$ = $1;}
		    
		| addExpression ADD multExpression
		    {$$ = fBinOpExp($1,(_add),$3);}
		    
		| addExpression SUB multExpression
		    {$$ = fBinOpExp($1,(_sub),$3);}
		    
        ;
        
multExpression:
		atom    
		    {$$ = $1;}
		    
		| multExpression MULT atom
		    {$$ = fBinOpExp($1,(_mult),$3);}
		    
		| multExpression DIV atom
		    {$$ = fBinOpExp($1,(_div),$3);}
		    
		| multExpression DEQ atom
		    {$$ = fBinOpExp($1,(_eq),$3);}
		    
		| multExpression NEQ atom
		    {$$ = fBinOpExp($1,(_neq),$3);}
		    
		| multExpression LT atom
		    {$$ = fBinOpExp($1,(_lt),$3);}
		    
		| multExpression LE atom
		    {$$ = fBinOpExp($1,(_le),$3);}
		    
		| multExpression GT atom
		    {$$ = fBinOpExp($1,(_gt),$3);}
		    
		| multExpression GE atom
		    {$$ = fBinOpExp($1,(_ge),$3);}
		    
        ;
        
atom:
		INTEGER	
		    {$$ = fIntLiteral($1);}
		    
		| LPAR expression RPAR
		    {$$ = $2;}
		    
		| IDENTIFIER
		    {$$ = fVariable($1);}
		    
		| IDENTIFIER LPAR args RPAR
		    {$$ = fFunCall($1,$3);}
		    
		| IDENTIFIER LPAR RPAR
		    {$$ = fFunCall($1,newNil());}
		    
        ;

args: 
        expression
            {$$ = newCons($1,newNil());}
            
        | args COMMA expression
            {$$ = newCons($3,$1);}
            
        ;

%%

/* display errors in yacc */
void yyerror(char* s) {
    fprintf(stdout, "%s\n", s);
}

int main(void) {

    // show logo
    toucanSplash();
    
    // run message
    printf("=> Toucan Compiler running ...");
    
    // parse input
    yyparse();
    
    // parsing complete?
    if (result!=NULL) {
        printf("tree built, parsing done!\n");
    } else {
        // lol nope :(
        printf("There were parse errors ...abort!");
        return 1;
    }
    
    // interpret code?
    if (T_INTERPRETER){
        printf("=> interpeting code ...");
        int ires = interpret(result); // call interpreter
        printf("INTERPRETER [result] -> %d <- done!\n", ires);
    }
    
    // compile code?
    if (T_COMPILER) {
        printf("=> compiling to mips: \"mipsOut.s\" ...");
        compileToMIPS(result, "mipsOut.s"); // call compiler
        printf("done!\n");
    }
    
    // free tree
    freeNode(result);
    printf("=>freed tree, exiting!\n");
    
    return 0;
}
