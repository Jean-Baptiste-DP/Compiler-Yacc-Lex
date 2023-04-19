#ifndef DATA
#include "../variables.h"
#endif

/* --- Calcul Symbole --- */

// Symbole newSymbole(int type, int val, char *var);
// Symbole freeSymbole(Symbole mysym);

/* --- Calcul Tree --- */

CalculNb leafConstInt(int value);
CalculNb leafConstFloat(float value);
CalculNb leafVar(char *varName);
CalculNb leafFct(int fctPosition);
CalculNb nodeOperator(int operat, CalculNb lChild, CalculNb rChild);
void freeCalculNb(CalculNb myCalc);
Variable runCalculNb(CalculNb myCalc, AllCalcFct fct, Data myData);
void incrementFctIndex(CalculNb tree, int num);
Variable integerCalculation(Variable var1, Variable var2, int operator);
Variable floatCalculation(Variable var1, Variable var2, int operator);