# Language compiler with Lex and Yacc

## Running the project

```bash
bison -d calc.y
flex calc.l
gcc -o calc calc.tab.c lex.yy.c prgmStructure.c stack.c variables.c
./calc < exFunction.txt
```

Once the *calc* file is compile, you can only run the last line to execute your program.

## Feature

Currently, the program is only supporting types **int** and **boolean**.
The program is able to do : 
- **variable** 
- **if / if else condition** (run *exIfCondition.txt* to see example)
- **for** and **while loop** (run *exForWhile.txt* to see example)
- **function** that can be **recursive** (run *exFunction.txt* to see example)