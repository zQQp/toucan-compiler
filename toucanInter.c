/*******************************************
 * Part of Toucan Compiler
 * by Jens Nierhaus
 * Compilerbau WS 12/13
 * C INTERPRETER FILE
 *******************************************/

#include "toucanHeader.h"
#include "strmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* environment for funcalls */
mNode progNode;
StrMap* funcarr[INFUNCALL];
int funcallCounter = 0;

/* prototype */
int interpretNode(mNode n, int env); 

/* no func overloading :> */
int interpret(mNode n){
    return interpretNode(n,0);
}

/* interpret binopexp node*/
int interpretBinOpExp(mNode n, int env){

    /* act for specific operator */
	switch((opType)n->content.cBinOpExp.op){
    
        case _add:
			if(ERDEBUG) yyerror("BinOpExp[+]");
            return interpretNode(n->content.cBinOpExp.lhs,env) + 
                interpretNode(n->content.cBinOpExp.rhs,env);

		case _sub:
            if(ERDEBUG) yyerror("BinOpExp[-]");
            return interpretNode(n->content.cBinOpExp.lhs,env) - 
                interpretNode(n->content.cBinOpExp.rhs,env);

		case _mult:
            if(ERDEBUG) yyerror("BinOpExp[*]");
            return interpretNode(n->content.cBinOpExp.lhs,env) * 
                interpretNode(n->content.cBinOpExp.rhs,env);

		case _div:
            if(ERDEBUG) yyerror("BinOpExp[/]");
            return interpretNode(n->content.cBinOpExp.lhs,env) / 
                interpretNode(n->content.cBinOpExp.rhs,env);

		case _eq:
            if(ERDEBUG) yyerror("BinOpExp[==]");
            return interpretNode(n->content.cBinOpExp.lhs,env) == 
                interpretNode(n->content.cBinOpExp.rhs,env);

		case _neq:
            if(ERDEBUG) yyerror("BinOpExp[!=]");
            return interpretNode(n->content.cBinOpExp.lhs,env) != 
                interpretNode(n->content.cBinOpExp.rhs,env);

		case _lt:
            if(ERDEBUG) yyerror("BinOpExp[<]");
            return interpretNode(n->content.cBinOpExp.lhs,env) < 
                interpretNode(n->content.cBinOpExp.rhs,env);

		case _le:
            if(ERDEBUG) yyerror("BinOpExp[<=]");
            return interpretNode(n->content.cBinOpExp.lhs,env) <= 
                interpretNode(n->content.cBinOpExp.rhs,env);

		case _gt:
            if(ERDEBUG) yyerror("BinOpExp[>]");
            return interpretNode(n->content.cBinOpExp.lhs,env) > 
                interpretNode(n->content.cBinOpExp.rhs,env);
			
		case _ge:
            if(ERDEBUG) yyerror("BinOpExp[>=]");
            return interpretNode(n->content.cBinOpExp.lhs,env) >= 
                interpretNode(n->content.cBinOpExp.rhs,env);
			
		default: yyerror("strange op");
    }
                   
    return -1;
}

/* interpret funcall node */
int interpretFunCall(mNode n, int env){

    if(ERDEBUG) yyerror("FunCall");
    
    // get name
    char* fname = n->content.cFunCall.name;
    if(ERDEBUG) yyerror(fname);
    
    // funcall counter for own environment setup
    int fcn = funcallCounter;
    funcallCounter++;
    funcarr[fcn] = sm_new(VHASHMAP_SIZE);    // enviroment hashmap     
            
    /* search for function body */
    mNode fun = NULL;
	Li tmp = progNode->content.cProgram.funs;
	while (tmp->isEmpty != true){
        if (0 == strcmp(((tmp->head)->content.cFunDef.name), fname)){
            fun = tmp->head;
            break;
        }
		tmp = tmp->tail;
	}
		    
	/* did not find functionbody? */
	if (fun == NULL){
		yyerror("undefined function");
	}
		    
	/* get args */
	Li fargs = n->content.cFunCall.args;
	Li fparam = fun->content.cFunDef.params;
		    
	if (length(fargs) != length(fparam)){
		yyerror("wrong number of args");
	}
		    
	/* copy args in param-arg-enviroment hashmap */
	while (fparam->isEmpty != true){

        char argval[11];
        sprintf(argval, "%d", interpretNode(fargs->head, env));

        if(ERDEBUG) yyerror((fparam->head)->content.cVariable.name);
        if(ERDEBUG) yyerror(argval);

        sm_put(funcarr[fcn], ((fparam->head)->content.cVariable.name), argval);

		fparam = fparam->tail;
		fargs = fargs->tail;
	}
		    
    return interpretNode(fun->content.cFunDef.body, fcn);
}

/* interpret if exp node */
int interpretIfExp(mNode n, int env){
       
    if(ERDEBUG) yyerror("IfExp"); 
    
    /* conition true ? */
    if (interpretNode(n->content.cIfExp.cond,env)){
        return interpretNode(n->content.cIfExp.alt1,env);
    }   
    return interpretNode(n->content.cIfExp.alt2,env);
}

/* interpret int literal node */
int interpretIntLiteral(mNode n, int env){

    if(ERDEBUG) yyerror("IntLiteral");    
    return n->content.cIntLiteral.n;
}

/* interpret program node */
int interpretProgram(mNode n, int env){

    if(ERDEBUG) yyerror("Program");  
    progNode = n;
    if (progNode == NULL) {
        yyerror("static node alloc error");
    }
            
    int programResult = interpretNode(n->content.cProgram.body,env);
    
    /* cleanup environments */
    while (funcallCounter>=0){   
        sm_delete(funcarr[funcallCounter]);
        funcallCounter--;
    }
            
    return programResult;

}

/* interpret variable node */
int interpretVariable(mNode n, int env){

    if(ERDEBUG) yyerror("Variable");  
    
    // get value from environment
    char buf[11];
    int hashcheck = sm_get(funcarr[env], n->content.cVariable.name, buf, sizeof(buf));
    if (hashcheck == 0) {
        yyerror("variable not found");
        yyerror(n->content.cVariable.name);
    }
    int variableValue = atoi(buf);
            
    return variableValue;        
}


int interpretFunDef(mNode n, int env){

    // dont need this in intrpreter
    yyerror("fundef nodes should not interpreted");            
    return -2;
}


/* calculate result of mNode n */
int interpretNode(mNode n, int env){

    if (n == NULL){
        yyerror("node null error");
        return -5;
    }

    /* act for specific node type */
    switch(n->nodeType){
    
        /* is bin op node? */
        case BinOpExp:
            return interpretBinOpExp(n,env);
              
        /* is funcall node? */    
        case FunCall:
            return interpretFunCall(n,env);
        
        /* is if expression node? */                         
        case IfExp:
            return interpretIfExp(n,env);
           
        /* is int literal node? */     
        case IntLiteral:
            return interpretIntLiteral(n,env);
            
        /* is Program node? */     
        case Program:
            return interpretProgram(n,env);

        /* is variable node? */     
        case Variable:
            return interpretVariable(n,env);        
  
        /* is fundef node? */     
        case FunDef:
            return interpretFunDef(n,env);
            
        default: yyerror("err0r");
    }

    /* should not reach this */
    yyerror("unknown nodeType");
    return -3;
}
