#ifndef STACK
#include "../stack.h"
#endif

/* --- Tree --- */

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

/* --- Functions --- */

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

/* --- Calculs --- */

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