#ifndef DATA
#include "../variables.h"
#endif

#ifndef STACK
#include "../stack.h"
#endif

typedef struct calcParameters{
    struct calcul *param;
    struct calcParameters *next;
}* CalcParameters;

typedef struct calcul{
    Variable var;
    CalcParameters params;
    Data values;
    Stack waitingResponse;
}* Calcul;

typedef struct paraResponse{
    int depth;
    char *funcName;
}*ParaResponse;

typedef struct calcLine{
    int length;
    int lastElement;
    Calcul *line;
}*CalcStorage;

Calcul VarCalc(char *name);
Calcul ConstCalcInt(int constante);
Calcul ConstCalcFloat(float constante);
Calcul FctCalc(char *name, CalcParameters parameters, int method); // method is a boolean = 1 if the function is a class method, else 0
void freeCalcul(Calcul calc);

CalcParameters newParameter(Calcul calc, CalcParameters nextParam);
void freeParameters(CalcParameters parameters);

char *getCalcCallBack(Calcul myCalc, Data myData, Data myStack);
ParaResponse getCallBack(CalcParameters params, Data myData, Data myStack, int waiting);
void getParametersValues(CalcParameters params, Data myStack, Data myData);
Variable runCalcul(Calcul myCalc, Data myData);
ParaResponse initResp(char *fctName);
void freeResp(ParaResponse resp);

CalcStorage newCalcStorage();
int storeCalcul(CalcStorage storage, Calcul calc);
Calcul getCalc(CalcStorage storage, int index);
void freeCalcStorage(CalcStorage storage);