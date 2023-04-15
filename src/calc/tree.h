#ifndef DATA
#include "../variables.h"
#endif

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