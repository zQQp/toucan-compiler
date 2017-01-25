# ******************************************
# * Part of Toucan Compiler
# * by Jens Nierhaus
# * Compilerbau WS 12/13
# * BUILD SHELLSCRIPT
# ******************************************
echo '=> ### toucan compile script ###'
rm lex.yy.*
rm y.tab.*
rm toucomp
bison -yd toucanParser.y
flex toucanLexer.l
gcc -Wall -o toucomp lex.yy.c y.tab.c strmap.c toucanUtil.c toucanInter.c toucanComp.c
printf '=> compiler compilation complete, running compiled compiler :D\n'
./toucomp < fibonacci.tou
