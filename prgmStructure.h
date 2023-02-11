#ifndef __cplusplus
#ifndef bool
#include <stdbool.h>
#endif
#endif

/* --- Type definition --- */

typedef struct intStack{
    int value;
    struct intStack *next;
}*IntStack;

typedef struct stack{
    IntStack stack;
}*Stack;

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
    bool executed;
    int value;
    struct fctParameters *nextParameter;
}*FctParameters;

typedef struct fctRegister
{
    char *name;
    bool executed;
    int value;
    FctParameters parameters;
}*FctRegister;

typedef struct allCalcFct{
    int length;
    int lastElement;
    FctRegister *line;
}*AllCalcFct;

typedef struct VariableStruct
{
    char *name;
    char *type;
    int value;
}*Variable;

typedef struct AllVariables
{
    struct VariableStruct *var;
    struct AllVariables *next;
}*DataStack;

typedef struct data{
    DataStack myData;
}*Data;

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

/* --- Variable --- */

Variable newVar(char *name, char *type, int value);
void freeVar(Variable var);

/* --- DataStack : Variable storage --- */

DataStack newDataStack();
bool isEmptyStack(DataStack variables);
bool isVarExistStack(DataStack variables, char *name);
bool isVarExistInContextStack(DataStack variables, char *name);
Variable getVarStack(DataStack variables, char *name);
DataStack deleteVarStack(DataStack variables, char *name);
DataStack storeVarStack(DataStack variables, Variable var);
void freeDataStack(DataStack variables);
DataStack freeContextStack(DataStack variables);
DataStack freeOneInStack(DataStack variables);

/* --- Stack --- */

Stack newStack();
bool StackisEmpty(Stack stack);
void freeIntStack(IntStack stack);
void freeStack(Stack stack);
void changeStack(Stack stack, IntStack intStack);
void removeStack(Stack stack);
void appendInt(Stack stack, int value);
int removeLastValue(Stack stack);

/* --- Data Storage --- */

Data newData();
bool isEmpty(Data variables);
bool isVarExist(Data variables, char *name);
bool isVarExistInContext(Data variables, char *name);
Variable getVar(Data variables, char *name);
void deleteVar(Data variables, char *name);
void storeVar(Data variables, Variable var);
void freeData(Data variables);
int freeContext(Data variables);

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
char *getCallBack(FctParameters parameter, Data myData, CalcStorage myCalculs, Stack myStack);
IntStack getParametersValues(FctParameters parameter);
void parameterExecutionFalse(FctParameters parameter);

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

void gotoFrom(Stack myStack, Program myPrgm);
void gotoDest(Stack myStack, Program myPrgm, int additionalPos);
void forConditionGoto(Stack myStack, Program myPrgm);
void forEndGoto(Stack myStack, Program myPrgm, char *loopVar);
void displayPrgm(Program myPrgm);

/* --- Run Prgm --- */

void runProgram(Program myPrgm, CalcStorage calculs, Data variables, Stack myStack);