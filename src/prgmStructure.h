#ifndef VARIABLE
#include "variable.h"
#endif

/* --- Type definition --- */

enum ActionType{
    STACK_VALUE,
    CALCUL,
    ASSIGNMENT,
    NEW_VAR,
    PRINT,
    IF,
    GOTO,
    EXIT,
    DELETE_VAR
};


typedef struct{
    enum ActionType actionType;
    Variable var;
} Action;

typedef struct prgmLine{
    int length;
    int lastElement;
    Action *line;
}*Program;


// /* --- Action of program --- */

// Action newAction(int type,char *var,int line,int calc, char *varType);
// void freeAction(Action act);

// /* --- Program Line --- */

Program newPrgm();
int storeAction(Program myPrgm, Action act);
Action getAction(Program myPrgm, int index);
void freeProgram(Program myPgrm);

// /* --- Build Prgm --- */

// int gotoFrom(Stack myStack, Program myPrgm);
// void gotoDest(Stack myStack, Program myPrgm, int additionalPos);
// void forEndGoto(Stack myStack, Program myPrgm, char *loopVar);
// void whileEndGoto(Stack myStack, Program myPrgm);
void displayPrgm(Program myPrgm);

// /* --- Run Prgm --- */

void runProgram(Program myPrgm);