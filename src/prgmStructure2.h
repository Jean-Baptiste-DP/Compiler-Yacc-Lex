#ifndef __cplusplus
#ifndef bool
#include <stdbool.h>
#endif
#endif

#include "stack.h"
#include "variables.h"
#include "calc2/calculs.h"
#include "calc2/functions.h"

/* --- Type definition --- */


typedef struct action{
    int type;
    VarInfo var;
    int line;
    int calc;
}*Action;

typedef struct prgmLine{
    int length;
    int lastElement;
    Action *line;
}*Program;


/* --- Action of program --- */

Action newAction(int type,char *var,int line,int calc, char *varType);
void freeAction(Action act);

/* --- Program Line --- */

Program newPrgm();
int storeAction(Program myPrgm, Action act);
Action getAction(Program myPrgm, int index);
void freeProgram(Program myPgrm);

/* --- Build Prgm --- */

int gotoFrom(Stack myStack, Program myPrgm);
void gotoDest(Stack myStack, Program myPrgm, int additionalPos);
void forEndGoto(Stack myStack, Program myPrgm, char *loopVar);
void whileEndGoto(Stack myStack, Program myPrgm);
void displayPrgm(Program myPrgm);

/* --- Run Prgm --- */

void runProgram(Program myPrgm, CalcStorage calculs, Data variables, Data myStack);