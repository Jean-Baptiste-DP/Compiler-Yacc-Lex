#ifndef DATA
#include "../variables.h"
#endif

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