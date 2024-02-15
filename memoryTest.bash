bison -d yaccLex/compiler.y
flex yaccLex/compiler.l

gcc -g -O0 -o compiler compiler.tab.c lex.yy.c src/*.c

valgrind --leak-check=full --show-leak-kinds=all ./compiler < test/calculation.test.txt