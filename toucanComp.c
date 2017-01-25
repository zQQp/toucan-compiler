/*******************************************
 * Part of Toucan Compiler
 * by Jens Nierhaus
 * Compilerbau WS 12/13
 * C MIPS COMPILER FILE
 *******************************************/

#include "toucanHeader.h"
#include "strmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


/* out file descriptor */
int out;

/* global hashmap for function param */
StrMap* sm;

/* prototypes */
int writeToOutput(char* o);
int makeLabel(void);
int generateMIPS(mNode n);


/* generate mips executable from node to file */
int compileToMIPS(mNode n, char* file){

    /* open and clear or create destination file 
		with user-read/write-access */
	out = open(file, O_WRONLY|O_TRUNC);
	if (out == (-1)){
		out = open(file, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
		if (out == (-1)){
			yyerror("file creation error");
			return 0;
		}
	}
    
    /* write header */
    writeToOutput("### MIPS EXECUTABLE BY TOUCAN COMPILER ###\n\n");
	writeToOutput("\t.data\n");
    writeToOutput("stra: .asciiz \"TEXE calculated result: \"\n");
	writeToOutput("\t.text\n\n");

    /* generate mips code */
    generateMIPS(n);
    
    /* close file */
    close(out);
    
    return 0;
}


int generateBinOpExp(mNode n){

	// visit left and right side and put on stack
    generateMIPS(n->content.cBinOpExp.lhs);
    generateMIPS(n->content.cBinOpExp.rhs);

    // get left and right side from stack in registers t0, t1
    writeToOutput("\tlw   $t1, 0($sp) \t\t\t# pop in t1\n");
    writeToOutput("\taddi $sp, $sp, 4\n");
    writeToOutput("\tlw   $t0, 0($sp) \t\t\t# pop in t0\n");
    writeToOutput("\taddi $sp, $sp, 4\n");            
        
	/* calculate result with desired operation */
	switch((opType)n->content.cBinOpExp.op){
    
        case _add:
			if(ERDEBUG) yyerror("BinOpExp[+]");
			writeToOutput("\tadd  $t2, $t0, $t1 \t\t\t# [+] t0, t1 -> t2\n");
			break;

		case _sub:
            if(ERDEBUG) yyerror("BinOpExp[-]");
            writeToOutput("\tsub  $t2, $t0, $t1 \t\t\t# [-] t0, t1 -> t2\n");
			break;

		case _mult:
            if(ERDEBUG) yyerror("BinOpExp[*]");
            writeToOutput("\tmul  $t2, $t0, $t1 \t\t\t# [*] t0, t1 -> t2\n");
			break;

		case _div:
            if(ERDEBUG) yyerror("BinOpExp[/]");
            writeToOutput("\tdiv  $t2, $t0, $t1 \t\t\t# [/] t0, t1 -> t2\n");
			break;

		case _eq:
            if(ERDEBUG) yyerror("BinOpExp[==]");
            writeToOutput("\tseq  $t2, $t0, $t1 \t\t\t# [==] t0, t1 -> t2\n");
			break;

		case _neq:
            if(ERDEBUG) yyerror("BinOpExp[!=]");
            writeToOutput("\tsne  $t2, $t0, $t1 \t\t\t# [!=] t0, t1 -> t2\n");
			break;

		case _lt:
            if(ERDEBUG) yyerror("BinOpExp[<]");
            writeToOutput("\tslt  $t2, $t0, $t1 \t\t\t# [<] t0, t1 -> t2\n");
			break;

		case _le:
            if(ERDEBUG) yyerror("BinOpExp[<=]");
            writeToOutput("\tsle  $t2, $t0, $t1 \t\t\t# [<=] t0, t1 -> t2\n");
			break;

		case _gt:
            if(ERDEBUG) yyerror("BinOpExp[>]");
            writeToOutput("\tsgt  $t2, $t0, $t1 \t\t\t# [>] t0, t1 -> t2\n");
			break;
			
		case _ge:
            if(ERDEBUG) yyerror("BinOpExp[>=]");
            writeToOutput("\tsge  $t2, $t0, $t1 \t\t\t# [>=] t0, t1 -> t2\n");
			break;
			
		default: yyerror("strange op");
    }
   
    // push result (t2) to stack
    writeToOutput("\taddi $sp, $sp, -4 \t\t\t# push t2\n");
    writeToOutput("\tsw   $t2, 0($sp)\n\n");

    return 0;
}

int generateFunCall(mNode n){
        
    if(ERDEBUG) yyerror("FunCall");     
    writeToOutput("\t\t\t\t\t\t\t\t# funcall start\n");

	/* put args on stack */
	Li fargs = n->content.cFunCall.args;
	while (fargs->isEmpty != true){
		generateMIPS(fargs->head);
		fargs = fargs->tail;
	}

	// call function
	char* fname = n->content.cFunCall.name;
	char jumpFun[64];
	sprintf(jumpFun, "\tjal %s \t\t\t\t\t# jump to fun\n", fname);  
	writeToOutput(jumpFun);

	//Copy result from stack to $t1
	writeToOutput("\tlw   $t1, 0($sp) \t\t\t# pop in t1\n"); 
	writeToOutput("\taddi $sp, $sp, 4\n");

	/* free stack from args */
	fargs = n->content.cFunCall.args;   
	while (fargs->isEmpty != true){
		writeToOutput("\taddi $sp, $sp, 4\n");
	    fargs = fargs->tail;
	}

	// write result (t1) to stack
	writeToOutput("\taddi $sp, $sp, -4 \t\t\t# push t1\n");
    writeToOutput("\tsw   $t1, 0($sp)\n");
    writeToOutput("\t\t\t\t\t\t\t\t# funcall end\n\n");
  
    return 0; 
}

int generateIfExp(mNode n){

    if(ERDEBUG) yyerror("IfExp"); 

	// get unique labels for branch/jump
	int label1 = makeLabel();
	int label2 = makeLabel();

	// calc condition
	generateMIPS(n->content.cIfExp.cond);
	writeToOutput("\tlw   $t0, 0($sp) \t\t\t# pop in t0\n");
    writeToOutput("\taddi $sp, $sp, 4\n");

	// jump on greater than zero to label1
	char ll1[64];
	sprintf(ll1, "\tbgtz  $t0, IFLABL%d \t\t# jump if t0>0\n", label1);
	writeToOutput(ll1);

	//alt 2 (else)
	generateMIPS(n->content.cIfExp.alt2);

	// jump label2
	char ll2[64];
	sprintf(ll2, "\tj IFLABL%d\n\n", label2);  
	writeToOutput(ll2);

	// if part (label1)
	char ll3[64];
	sprintf(ll3, "IFLABL%d: \t\t\t\t\t\t# else part\n", label1);  
	writeToOutput(ll3);

	// alt 1
	generateMIPS(n->content.cIfExp.alt1);

	// label 2
	char ll4[64];
	sprintf(ll4, "IFLABL%d: \t\t\t\t\t\t# endif\n", label2);  
	writeToOutput(ll4);

	return 0;	
}

int generateIntLiteral(mNode n){

    if(ERDEBUG) yyerror("IntLiteral");  

    // write IntLiteral to t0
    char intlitval[64];
    int tmpval = n->content.cIntLiteral.n;
    sprintf(intlitval, "\taddi $t0, $zero, %d \t\t\t# intLiteral -> t0\n", tmpval);  
    writeToOutput(intlitval);

    // push t0 to stack
    writeToOutput("\taddi $sp, $sp, -4 \t\t\t# push t0\n");
    writeToOutput("\tsw   $t0, 0($sp)\n\n");

    return 0;        
}

int generateProgram(mNode n){

    if(ERDEBUG) yyerror("Program");

    // main header
	writeToOutput("#START_MAIN#\n");
	writeToOutput("main:\n");
    
	// prolog
	writeToOutput("\taddi $sp, $sp, -4 \t\t\t# push ra\n");
	writeToOutput("\tsw   $ra, 0($sp)\n\n");
    
	// display string
	writeToOutput("\tli   $v0, 4 \t\t\t\t# display string\n");
	writeToOutput("\tla   $a0, stra\n");
	writeToOutput("\tsyscall\n\n");

	// generate body
    generateMIPS(n->content.cProgram.body);

	// display result
	writeToOutput("\tlw   $a0, 0($sp) \t\t\t# pop in a0\n");
	writeToOutput("\taddi $sp, $sp, 4\n");
	writeToOutput("\tli   $v0, 1 \t\t\t\t# display a0\n");
	writeToOutput("\tsyscall\n\n");
    
	// epilog
	writeToOutput("\tlw	  $ra, 0($sp) \t\t\t# pop in ra\n");
	writeToOutput("\taddi $sp, $sp, 4\n");
	writeToOutput("\tjr	  $ra \t\t\t\t\t# jump ra\n\n");

	// generate functions
	writeToOutput("\n#FUNCTIONS#\n\n");
	
	Li tmp2 = n->content.cProgram.funs;
	while (tmp2->isEmpty != true){
        generateMIPS(tmp2->head);
		writeToOutput("\n");
	    tmp2 = tmp2->tail;
	}

    return 0; 
}

int generateVariable(mNode n){

    if(ERDEBUG) yyerror("Variable");  
	
	// calc where variable is on stack
    char buf[11];
    int hashcheck = sm_get(sm, n->content.cVariable.name, buf, sizeof(buf));
    if (hashcheck == 0) {
        yyerror("variable not found");
        yyerror(n->content.cVariable.name);
    }
    int variableStack = ((atoi(buf))*4);
	
	// get variable from stack and push on top of stack
	char getVS[64];
    sprintf(getVS, "\tlw   $t0, %d($t7) \t\t\t# getVariable(t7) in t0\n", variableStack);  
    writeToOutput(getVS);
    writeToOutput("\taddi $sp, $sp, -4 \t\t\t# push t0\n");
    writeToOutput("\tsw   $t0, 0($sp)\n\n");
   
    if(ERDEBUG) {  
		char vrdebug[42];
		sprintf(vrdebug, "Var var=%s; val=%d($t7)", n->content.cVariable.name, variableStack);
		yyerror(vrdebug);
	}

    return 0;  
}

int generateFunDef(mNode n){

    if(ERDEBUG) yyerror("FunDef");

	/* create label */
    char* fundname = n->content.cFunDef.name;
    char funlabel[32];
    sprintf(funlabel, "%s:\n", fundname);  
    writeToOutput(funlabel);
  
	/* copy params in enviroment hashmap */
	sm = sm_new(VHASHMAP_SIZE);    // enviroment hashmap
	Li fparam = reverse(n->content.cFunDef.params); // reverse the list
	int paramCount = 0;
	while (fparam->isEmpty != true){
		char* pcb = (fparam->head)->content.cVariable.name;
        char pcc[11];
        sprintf(pcc, "%d", paramCount);
        paramCount = paramCount + 1;
        sm_put(sm, pcb, pcc);
		fparam = fparam->tail;

		if(ERDEBUG) {  
			char vhdebug[64];
			sprintf(vhdebug, "fundef var=%s; val=%s", pcb, pcc);
			yyerror(vhdebug);
		}
	}

    // prolog (callee save etc)
    writeToOutput("\tmove $t6, $t7 \t\t\t\t# calleeSave t7 in t6\n");
	writeToOutput("\tmove $t7, $sp \t\t\t\t# save varPointer in t7\n");
	writeToOutput("\taddi $sp, $sp, -4 \t\t\t# push ra\n");
	writeToOutput("\tsw	  $ra, 0($sp)\n\n");
	writeToOutput("\taddi $sp, $sp, -4 \t\t\t# push t6 (calleeSave t7)\n");
	writeToOutput("\tsw	  $t6, 0($sp)\n\n");
 
    // generate body
    generateMIPS(n->content.cFunDef.body);

    // save result in t0
    writeToOutput("\tlw	  $t0, 0($sp) \t\t\t# pop in t0\n");
	writeToOutput("\taddi $sp, $sp, 4\n");

    // epilog
	writeToOutput("\tlw	  $t7, 0($sp) \t\t\t# pop in t7\n");
	writeToOutput("\taddi $sp, $sp, 4\n");
	writeToOutput("\tlw	  $ra, 0($sp) \t\t\t# pop in ra\n");
	writeToOutput("\taddi $sp, $sp, 4\n");

	// push result on top of stack
	writeToOutput("\taddi $sp, $sp, -4 \t\t\t# push t0\n");
	writeToOutput("\tsw	  $t0, 0($sp)\n\n");

    // jump back
	writeToOutput("\tjr   $ra \t\t\t\t\t# jump ra\n\n");

	// clear hashmap
	sm_delete(sm);
    sm = NULL;
  
    return 0;
}

/* generate code for mips mashines from node */
int generateMIPS(mNode n){

    // node null?
    if (n == NULL){
        yyerror("node null error");
        return -5;
    }

    /* act for specific node type */
    switch(n->nodeType){
    
        case BinOpExp:
			return generateBinOpExp(n);

        case FunCall:
			return generateFunCall(n);
			
        case IfExp:
			return generateIfExp(n);
    
        case IntLiteral:
			return generateIntLiteral(n);
 
        case Program:
			return generateProgram(n);
            
        case Variable:
			return generateVariable(n);
		
        case FunDef:
			return generateFunDef(n); 
    }

    /* should not reach this */
	yyerror("dont know this node err0r");
    return -3;
}

/* write string to global filediscriptor */
int writeToOutput(char* o){
    write(out, o, strlen(o));
    return 0;
}


/* label number generator */
int makeLabel(void){
	static int lseed = 100;
	return lseed++;
}
