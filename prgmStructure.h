#ifndef __cplusplus
#ifndef bool
#include <stdbool.h>
#endif
#endif

#include "stack.h"
#include "variables.h"

/* --- Type definition --- */

typedef struct calcSym{
    int value;
    int type;
    char *variable;
}*Symbole;

typedef struct calcTree{
    Symbole symbole;
    struct calcTree *leftChild;
    struct calcTree *rightChild;
}*CalculNb;

typedef struct fctParameters{
    int calc;
    Stack value;
    struct fctParameters *nextParameter;
}*FctParameters;

typedef struct paraResponse{
    int depth;
    char *funcName;
}*ParaResponse;

typedef struct fctStack
{
    Stack values;
    Stack waitingResponse;
}*FctStack;

typedef struct fctRegister
{
    char *name;
    FctStack stacks;
    FctParameters parameters;
}*FctRegister;

typedef struct allCalcFct{
    int length;
    int lastElement;
    Stack waitingFunctions;
    FctRegister *line;
}*AllCalcFct;

typedef struct calcul
{
    CalculNb nb;
    AllCalcFct fct;
}*Calcul;

typedef struct calcLine{
    int length;
    int lastElement;
    Calcul *line;
}*CalcStorage;

typedef struct action{
    int type;
    char *varName;
    int line;
    int calc;
}*Action;

typedef struct prgmLine{
    int length;
    int lastElement;
    Action *line;
}*Program;

/* --- Calcul Symbole --- */

Symbole newSymbole(int type, int val, char *var);
Symbole freeSymbole(Symbole mysym);

/* --- Calcul Tree --- */

CalculNb leafConst(int value);
CalculNb leafVar(char *varName);
CalculNb leafFct(int fctPosition);
CalculNb nodeOperator(int operat, CalculNb lChild, CalculNb rChild);
void freeCalculNb(CalculNb myCalc);
int runCalculNb(CalculNb myCalc, AllCalcFct fct, Data myData);
void incrementFctIndex(CalculNb tree, int num);


/* --- Parameters function storage --- */

FctParameters addParameter(int calc, FctParameters nextPara);
void freeParameter(FctParameters parameter);
ParaResponse getCallBack(FctParameters parameter, Data myData, CalcStorage myCalculs, Stack myStack, int waitingDepth);
void getParametersValues(FctParameters parameter, Stack myStack);

/* --- Response of function's parameters --- */

ParaResponse initResp(char *fctName);
void incrementDepth(ParaResponse resp);
void freeResp(ParaResponse resp);
bool isFctInPara(ParaResponse resp);

/* --- Stack for functions --- */

FctStack initFctStack();
void freeFctStack(FctStack myFctStack);

/* --- Functions --- */

FctRegister initFct(char *name, FctParameters parameters);
void freeFctRegistered(FctRegister fct);
char *getFctCallBack(FctRegister fct, Data myData, CalcStorage myCalc, Stack myStack);

/* -- Storage of calculs functions --- */

AllCalcFct noFctinCalc();
void storeFctCalc(AllCalcFct allFct, FctRegister fct);
char *getCallBackInAll(AllCalcFct allFct, Data myData, CalcStorage myCalc, Stack myStack);
void freeAllCalcFct(AllCalcFct allFct);

/* --- One calcul --- */

Calcul newCalc(CalculNb nb, AllCalcFct fct);
void freeCalcul(Calcul calc);
char *getCalcCallBack(Calcul myCalc, Data myData, CalcStorage myCalcStorage, Stack myStack);
int runCalcul(Calcul myCalc, Data myData);
Calcul ConstCalc(int constante);
Calcul VarCalc(char *name);
Calcul FctCalc(char *name, FctParameters parameters);
Calcul OpeCalc(int operat, Calcul left, Calcul right);

/* --- All calculs --- */

CalcStorage newCalcStorage();
int storeCalcul(CalcStorage storage, Calcul calc);
Calcul getCalc(CalcStorage storage, int index);
void freeCalcStorage(CalcStorage storage);

/* --- Action of program --- */

Action newAction(int type,char *var,int line,int calc);
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

void runProgram(Program myPrgm, CalcStorage calculs, Data variables, Stack myStack);