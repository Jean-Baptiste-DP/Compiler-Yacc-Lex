# Language compiler with Lex and Yacc

## Running the project

```bash
bison -d yaccLex/compiler.y
flex yaccLex/compiler.l
gcc -o compiler compiler.tab.c lex.yy.c src/*.c src/*/*.c
./compiler < examples/exFunction.txt
```

Once the *compiler* file is compile, you can only run the last line to execute your program.

## Feature

Currently, the program is only supporting types **int** and **boolean**.
The program is able to do : 
- **variable** 
- **if / if else condition** (run *examples/exIfCondition.txt* to see example)
- **for** and **while loop** (run *examples/exForWhile.txt* to see example)
- **function** that can be **recursive** (run *examples/exFunction.txt* to see example)