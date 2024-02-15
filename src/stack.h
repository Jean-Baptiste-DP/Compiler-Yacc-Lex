#define STACK

#ifndef VARIABLE
#include "variable.h"
#endif

#ifndef __cplusplus
#ifndef bool
#include <stdbool.h>
#endif
#endif

typedef struct varStack{
    Variable value;
    struct varStack *next;
}* VarStack;

typedef struct stack{
    VarStack stack;
}*Stack;

/* --- Stack --- */

Stack newStack();
bool StackisEmpty(Stack stack);
void freeStack(Stack stack);
void changeStack(Stack stack, VarStack VarStack);
void removeStack(Stack stack);
void appendVar(Stack stack, Variable value);
Variable removeLastValue(Stack stack);