#ifndef _TOUCANHEADER_H
#define _TOUCANHEADER_H 
/*******************************************
 * Part of Toucan Compiler
 * by Jens Nierhaus
 * Compilerbau WS 12/13
 * C HEADER FILE
 *******************************************/

/* defines for options */

// interpret code
#define T_INTERPRETER 1

// compile Code
#define T_COMPILER 1

// show debug messages
#define ERDEBUG 0

// size of variable hashmap
#define VHASHMAP_SIZE 10

// allow interpreter that many funcalls
#define INFUNCALL 999999


/* data structures */

// for boolean type
typedef enum {false,true} boolean;

// types of treenodes
typedef enum {BinOpExp, FunCall, FunDef, IfExp, IntLiteral, Program, Variable} mNodeType;

// operator types
typedef enum {_add, _sub, _mult, _div, _eq, _neq, _lt, _le, _gt, _ge} opType;

// typedef for node and nodelist
typedef struct multiNode* mNode;
typedef struct Lixx* Li;

// struct of multinode (mNode)
struct multiNode{

    // determined node type
	mNodeType nodeType;

	union {

		struct {
			opType op;
			mNode lhs;
			mNode rhs;
		} cBinOpExp; // BinOpExp

		struct {
			char* name;
			Li args;
		} cFunCall; // FunCall

		struct {
			char* name;
			Li params;
			mNode body;
		} cFunDef; // FunDef

		struct {
			mNode cond;
			mNode alt1;
			mNode alt2;
		} cIfExp; // IfExp

		struct {
			int n;
		} cIntLiteral; // IntLiteral

		struct {
			Li funs;
			mNode body;
		} cProgram; // Program

		struct {
			char* name;
		} cVariable; // Variable

	} content;
};

// semester 1 lists for nodes ;D
struct Lixx{
	mNode head;
	Li tail;
	boolean isEmpty;
};


/* prototypes */

/**
 * Yylex.
 *
 * @return 0 if success
 */
int yylex(void);

/**
 * Yyerror.
 *
 * @param s the error message to display
 */
void yyerror(char* s);

/**
 * bin op exp constructor function.
 *
 * @param lhs the lhs
 * @param op the operator
 * @param rhs the rhs
 * @return the m node
 */
mNode fBinOpExp(mNode lhs, opType op, mNode rhs);

/**
 * fun call constructor function.
 *
 * @param name the function name
 * @param args the argument list
 * @return the m node
 */
mNode fFunCall(char* name, Li args);

/**
 * fun def constructor function.
 *
 * @param name the function name
 * @param params the parameter List
 * @param body the function body
 * @return the m node
 */
mNode fFunDef(char* name, Li params, mNode body);

/**
 * if exp constructor function.
 *
 * @param cond the condition
 * @param alt1 the alternative1
 * @param alt2 the alternative2
 * @return the m node
 */
mNode fIfExp(mNode cond, mNode alt1, mNode alt2);

/**
 * int literal constructor function.
 *
 * @param n the integer literal
 * @return the m node
 */
mNode fIntLiteral(int n);

/**
 * program constructor function.
 *
 * @param funs the function List
 * @param body the body expression
 * @return the m node
 */
mNode fProgram(Li funs, mNode body);

/**
 * variable constructor function.
 *
 * @param name the variable name
 * @return the m node
 */
mNode fVariable(char* name);

/**
 * Free mNode - tree node destructor.
 *
 * @param n the node to destroy
 */
void freeNode(mNode n);
	
/**
 * New nil.
 *
 * @return the empty list
 */
Li newNil();

/**
 * construct new list element.
 *
 * @param hd the head
 * @param tl the tail
 * @return the list
 */
Li newCons(mNode hd, Li tl);
	
/**
 * Length.
 *
 * @return the length of the list
 */
int length(Li this);
	
/**
 * retrieve Last list item (not the empty one).
 *
 * @param this the list
 * @return the m node
 */
mNode last(Li this);
	
/**
 * Reverse list.
 *
 * @param this the list to reverse
 * @return the reversed list
 */
Li reverse(Li this);

/**
 * Interpret node.
 *
 * @param n the node to interpret
 * @return the result
 */
int interpret(mNode n);
	
/**
 * Compile to mips.
 *
 * @param n the node to compile
 * @param file filename for executable output
 * @return 0 if success
 */
int compileToMIPS(mNode n, char* file);
	
/**
 * Display Toucan splash eyecandy by printing.
 */
void toucanSplash();
	

#endif
