#include <stdlib.h>

#ifndef DATA
#include "../variables.h"
#endif

int runFunction(char *name, Data myStack, Data myData, int currentLine);
int runFunctionEmpty(char *functionName, Data myData, int currentLine);