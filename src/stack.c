#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

Stack newStack(){
    Stack myStack = malloc(sizeof(Stack));
    myStack->stack = NULL;
    return myStack;
}

bool StackisEmpty(Stack stack){
    return stack->stack == NULL;
}

void freeIntStack(IntStack stack){
    if(stack){
        freeIntStack(stack->next);
        free(stack);
    }
}

void freeStack(Stack stack){
    freeIntStack(stack->stack);
    free(stack);
}

void removeStack(Stack stack){
    freeIntStack(stack->stack);
    stack->stack=NULL;
}

void changeStack(Stack stack, IntStack intStack){
    removeStack(stack);
    stack->stack = intStack;
}

void appendInt(Stack stack, int value){
    IntStack myStack = malloc(sizeof(IntStack));
    myStack->next = stack->stack;
    myStack->value = value;
    stack->stack = myStack;
}

int removeLastValue(Stack stack){
    int value = stack->stack->value;
    IntStack next  = stack->stack->next;
    free(stack->stack);
    stack->stack = next;
    return value;
}

void displayIntStack(IntStack stack){
    if(stack){
        displayIntStack(stack->next);
        printf("Stack value : %d\n", stack->value);
    }
}

void displayStack(Stack stack){
    displayIntStack(stack->stack);
}