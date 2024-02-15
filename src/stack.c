#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

Stack newStack(){
    Stack myStack = malloc(sizeof(struct stack));
    myStack->stack = NULL;
    return myStack;
}

bool StackisEmpty(Stack stack){
    return stack->stack == NULL;
}

void freeVarStack(VarStack stack){
    if(stack){
        freeVarStack(stack->next);
        free(stack);
    }
}

void freeStack(Stack stack){
    if(stack){
        freeVarStack(stack->stack);
        free(stack);
    }
}

void removeStack(Stack stack){
    freeVarStack(stack->stack);
    stack->stack=NULL;
}

void changeStack(Stack stack, VarStack VarStack){
    removeStack(stack);
    stack->stack = VarStack;
}

void appendVar(Stack stack, Variable value){
    VarStack myStack = malloc(sizeof(struct varStack));
    myStack->next = stack->stack;
    myStack->value = value;
    stack->stack = myStack;
}

Variable removeLastValue(Stack stack){
    if(StackisEmpty(stack)){
        printf("Empty stack\n");
        exit(1);
    }
    Variable value = stack->stack->value;
    VarStack next  = stack->stack->next;
    free(stack->stack);
    stack->stack = next;
    return value;
}