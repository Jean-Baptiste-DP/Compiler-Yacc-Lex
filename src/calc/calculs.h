#define CALCULS

#include "types.h"

#include "tree.h"
#include "functions.h"

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