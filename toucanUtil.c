/*******************************************
 * Part of Toucan Compiler
 * by Jens Nierhaus
 * Compilerbau WS 12/13
 * C UTILITY FILE
 *******************************************/

#include "toucanHeader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* constructor function for binop nodes */
mNode fBinOpExp(mNode lhs, opType op, mNode rhs) {
	mNode result;
	if ((result = (mNode)malloc(sizeof(struct multiNode))) == NULL) {
        	yyerror("out of memory");
	}
	result->nodeType = BinOpExp;
	result->content.cBinOpExp.lhs = lhs;
	result->content.cBinOpExp.op=op;
	result->content.cBinOpExp.rhs = rhs;
	return result;
}

/* constructor function for funcall nodes */
mNode fFunCall(char* name, Li args) {
	mNode result;
	if ((result = (mNode)malloc(sizeof(struct multiNode))) == NULL) {
        	yyerror("out of memory");
	}
	result->nodeType = FunCall;
	result->content.cFunCall.name = name;
	result->content.cFunCall.args = args;
	return result;
}

/* constructor function for fundef nodes */
mNode fFunDef(char* name, Li params, mNode body) {
	mNode result;
	if ((result = (mNode)malloc(sizeof(struct multiNode))) == NULL) {
        	yyerror("out of memory");
	}
	result->nodeType = FunDef;
	result->content.cFunDef.name = name;
	result->content.cFunDef.params = params;
	result->content.cFunDef.body = body;
	return result;
}

/* constructor function for ifexpression nodes */
mNode fIfExp(mNode cond, mNode alt1, mNode alt2) {
	mNode result;
	if ((result = (mNode)malloc(sizeof(struct multiNode))) == NULL) {
        	yyerror("out of memory");
	}
	result->nodeType = IfExp;
	result->content.cIfExp.cond=cond;
	result->content.cIfExp.alt1=alt1;
	result->content.cIfExp.alt2=alt2;
	return result;
}

/* constructor function for int literal nodes */
mNode fIntLiteral(int n) {
	mNode result;
	if ((result = (mNode)malloc(sizeof(struct multiNode))) == NULL) {
        	yyerror("out of memory");
	}
	result->nodeType=IntLiteral;
	result->content.cIntLiteral.n=n;
	return result;
}

/* constructor function for program nodes */
mNode fProgram(Li funs, mNode body) {
	mNode result;
	if ((result = (mNode)malloc(sizeof(struct multiNode))) == NULL) {
        	yyerror("out of memory");
	}
	result->nodeType=Program;
	result->content.cProgram.funs=funs;
	result->content.cProgram.body=body;
	return result;
}

/* constructor function for variable nodes */
mNode fVariable(char* name) {
	mNode result;
	if ((result = (mNode)malloc(sizeof(struct multiNode))) == NULL) {
        	yyerror("out of memory");
	}
	result->nodeType=Variable;
	result->content.cVariable.name=name;
	return result;
}

/* destructor function for all mNodes */
void freeNode(mNode n) {

	if (n==NULL) return;

	if (n->nodeType == BinOpExp) {
		freeNode(n->content.cBinOpExp.lhs);
		freeNode(n->content.cBinOpExp.rhs);
		free(n);
		return;
	}

	if (n->nodeType == FunCall) {
		Li tmp = n->content.cFunCall.args;
		while (tmp->isEmpty != true){
			freeNode(tmp->head);
			Li doomed = tmp;
			tmp = tmp->tail;
			free(doomed);
		}	
		free(n);
		return;
	}

	if (n->nodeType == FunDef) {
		Li tmp = n->content.cFunDef.params;
		while (tmp->isEmpty != true){
			freeNode(tmp->head);
			Li doomed = tmp;
			tmp = tmp->tail;
			free(doomed);
		}
		free(n->content.cFunDef.name);
		freeNode(n->content.cFunDef.body);
		free(n);
		return;
	}

	if (n->nodeType == IfExp) {
		freeNode(n->content.cIfExp.cond);
		freeNode(n->content.cIfExp.alt1);
		freeNode(n->content.cIfExp.alt2);
		free(n);
		return;
	}

	if (n->nodeType == Program) {
		Li tmp = n->content.cProgram.funs;
		while (tmp->isEmpty != true){
			freeNode(tmp->head);
			Li doomed = tmp;
			tmp = tmp->tail;
			free(doomed);
		}
		freeNode(n->content.cProgram.body);
		free(n);
		return;
	}

	if (n->nodeType == Variable) {
		free(n->content.cVariable.name);
		free(n);
		return;
	}

	/* nodes with no malloc content */
	free(n);
}

/* empty list constructor */
Li newNil(){
	Li this = (Li)malloc(sizeof(struct Lixx));
	this->head = NULL;
	this->isEmpty = true;
	return this;
}

/* list piece constructor */
Li newCons(mNode hd, Li tl){
	Li this = (Li)malloc(sizeof(struct Lixx));
	this->isEmpty = false;
	this->head = hd;
	this->tail = tl;
	return this;
}

/* calculate length of list */
int length(Li this){
	if (this->isEmpty) return 0;
	return 1+length(this->tail);	
}

/* retrieve last list piece */
mNode last(Li this){
	if (this->isEmpty) return NULL;
	if (this->tail->isEmpty) return this->head;
	return last(this->tail);
}

/* reverse list */
Li reverse(Li this){
	int i;
	Li result = newNil();
	if (this->isEmpty){
		return result;
	}

	Li tmpLi = this;
	for (i = length(this); i >= 1; i--){
		result = newCons(tmpLi->head, result);
		tmpLi = tmpLi->tail;
	}
	
	return result;
}

/* terminal toucan splash */
void toucanSplash(){

	printf("### TOUCAN COMPILER - COMPILERBAU WS12/13 ###\n");     

	printf("            .7NNNMM??+??+~,:...\n");     
	printf("         ...MO?+==IM?7II+?$?$-,:.\n");
	printf("         ..7++??$=N?IIIIII?++I??~~\n");
	printf("         .D++++=?7M??+?IZ7I7+???Z8O.\n");
	printf("       ..:+~===+?+$N$$7?+II7$77Z88ZO.\n");
	printf("       ZD8+=+==+++?Z?...?O7?7+$OD88D8\n");
	printf("    .MONMM+++=++++$==.      .O$?8DDDD\n");
	printf("   .NONMMM?+?++?77$7?M          .8DDN\n");
	printf("  .:DMMMMMO?++?I7$ZZ8+            .,D\n");
	printf("  :NMMMDNM8O7?I7$ZZNM.\n");
	printf("  8MMMMN8DN8O8D88DMM8.\n");
	printf("  $.MMMMMMNMN88MMN..\n");
	printf("  .MMMMD88NNNNM8..   nierhaus@live.com\n");
	printf(" .NMMMMMM,.,MM...\n");
	printf(" .MMMMM=M.  .O,..\n");
	printf(".$MM$MMII=. .~8NIIOO.\n");

}
