# README FILE FOR TOUCAN COMPILER

by Jens-Christoph Nierhaus / jens@nierha.us

# Language:

PROJECT COMPILERBAU HS-RM WS12/13 Allgemeine Informatik (KAT)

Toucan Compiler is based on CMaj7 by Prof. Dr. Sven Eric Panitz
until funcall/asambly gen

written completely in C (flex/yacc)

options can be set with defines in toucanHeader.h


# Files:

README.md -- this README FILE

compilecompiler.sh -- shell script to compile the compiler and run with sampleprogram.tou

toucanLexer.l -- lex/flex tokenizer file

toucanParser.y -- yacc/bison file containing grammar for parser generation

toucanHeader.h -- header file with defines, required prototypes and data structures

toucanUtil.c -- tree/list/misc utility code

toucanInter.c -- interpreter code

toucanComp.c -- mips-assembly-generator code

*.tou -- sample test programs in CMaj7

toucomp -- toucan compiler executable (created by compilecompiler.sh)

lex.yy.c / y.tab.h / y.tab.c -- generated by lex/yacc

strmap.h/.c -- library for string hashmap Copyright (c) 2009, 2011 Per Ola Kristensson.


# Tools used:

tokenizer:
flex 2.5.35

parser:
bison (GNU Bison) 2.3
(option -y for yacc)

compiler:
i686-apple-darwin10-gcc-4.2.1 (GCC) 4.2.1 (Apple Inc. build 5666) (dot 3)

editor:
Bare Bones Software | BBEdit 10

os:
Unix OSX 10.6.8 Snow Leopard
Darwin Kernel Version 10.8.0: Tue Jun  7 16:33:36 PDT 2011; 
root:xnu-1504.15.3~1/RELEASE_I386 i386

mips emulator:
QTSpim Version 9.1.7 of February 12, 2012
