/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INTEGER = 258,
     IDENTIFIER = 259,
     DEFINE = 260,
     CONSTRUCTOR = 261,
     CASE = 262,
     OF = 263,
     IF = 264,
     THEN = 265,
     ELSE = 266,
     LPAR = 267,
     RPAR = 268,
     DOT = 269,
     COMMA = 270,
     ARROW = 271,
     SEMIC = 272,
     EQ = 273,
     ADD = 274,
     SUB = 275,
     MULT = 276,
     DIV = 277,
     DEQ = 278,
     NEQ = 279,
     LT = 280,
     LE = 281,
     GT = 282,
     GE = 283
   };
#endif
/* Tokens.  */
#define INTEGER 258
#define IDENTIFIER 259
#define DEFINE 260
#define CONSTRUCTOR 261
#define CASE 262
#define OF 263
#define IF 264
#define THEN 265
#define ELSE 266
#define LPAR 267
#define RPAR 268
#define DOT 269
#define COMMA 270
#define ARROW 271
#define SEMIC 272
#define EQ 273
#define ADD 274
#define SUB 275
#define MULT 276
#define DIV 277
#define DEQ 278
#define NEQ 279
#define LT 280
#define LE 281
#define GT 282
#define GE 283




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 19 "toucanParser.y"
{
    int integer;		/* integer value */
    char* ident;		/* identifier */
	mNode tmnode;       /* multi nodes */
	Li tli;             /* list */
}
/* Line 1529 of yacc.c.  */
#line 112 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

