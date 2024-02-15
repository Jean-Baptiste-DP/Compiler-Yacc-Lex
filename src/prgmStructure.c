#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "prgmStructure.h"
#include "stack.h"

Program newPrgm(){
    int size = 4;
    Program initPgrm = malloc(sizeof(struct prgmLine)); 
    initPgrm->length = size;
    initPgrm->lastElement = 0;
    initPgrm->line = malloc(size*sizeof(Action));
    return initPgrm;
}

/* Store action in program */

int storeAction(Program myPrgm, Action action){
    if(myPrgm->lastElement == myPrgm->length){
        int size = 2* myPrgm->length; 
        Action *line = myPrgm->line;
        myPrgm->line = malloc(size*sizeof(Action));
        for(int i = 0; i<myPrgm->length; i=i+1){
            myPrgm->line[i] = line[i];
        }
        free(line);
        myPrgm->length = size;
        myPrgm->line[myPrgm->lastElement] = action;
        myPrgm->lastElement=myPrgm->lastElement+1;
    }else{
        myPrgm->line[myPrgm->lastElement] = action;
        myPrgm->lastElement=myPrgm->lastElement+1;
    }
    return myPrgm->lastElement-1;
}

/* give the given action */

Action getAction(Program myPrgm, int index){
    if(index<myPrgm->lastElement){
        return myPrgm->line[index];
    }else{
        printf("Index out of bound\n");
        exit(1);
    }
}

void freeProgram(Program myPgrm){
    free(myPgrm->line);
    free(myPgrm);
}

void displayVariable(Variable myVar){
    if(myVar.dataType == INTEGER){
        // printf("int : %d\n", myVar.integer);
        printf("%d\n", myVar.integer);
    }else if(myVar.dataType == FLOAT){
        // printf("float : %f\n", myVar.floattant);
        printf("%f\n", myVar.floattant);
    }else if(myVar.dataType == LINE){
        // printf("line : %d\n", myVar.line);
        printf("%d\n", myVar.line);
    }else if(myVar.dataType == POINTER){
        // printf("pointer : %p\n", myVar.pointer);
        printf("%p\n", myVar.pointer);
    }else if(myVar.dataType == OPERATOR){
        // printf("operator : %d\n", myVar.operator);
        printf("%d\n", myVar.operator);
    }else if(myVar.dataType == BOOLEAN){
        // printf("boolean : %d\n", myVar.boolean);
        printf("%d\n", myVar.boolean);
    }
}

void displayAction(Action myAct){
    if(myAct.actionType == STACK_VALUE){
        printf("[Stack the value] ");
        displayVariable(myAct.var);
    }else if(myAct.actionType == CALCUL){
        printf("[Calcul] ");
        displayVariable(myAct.var);
    }else if(myAct.actionType == PRINT){
        printf("[Print] \n");
    }else{
        printf("[ ~ ] ");
        displayVariable(myAct.var);
    }
}

void displayPrgm(Program myPrgm){
    int maxNumberLength = snprintf(NULL, 0,"%d",myPrgm->lastElement-1);
    for(int i=0; i<myPrgm->lastElement;i = i+1){
        printf("(%*d) ", maxNumberLength, i);
        displayAction(myPrgm->line[i]);
    }
}

Variable runCalcul(Variable operator, Stack stack){
    if(operator.dataType != OPERATOR){
        printf("Type error, expected OPERATOR\n");
        exit(1);
    }

    if(operator.operator == UMOINS){ // UMOINS -> -6 
        Variable var = removeLastValue(stack);
        if(var.dataType == INTEGER){
            var.integer = - var.integer;
        }else if(var.dataType == FLOAT){
            var.floattant = - var.floattant;
        }
        return var;
    }else if(operator.operator == NOT){ // NOT -> !False
        Variable var = removeLastValue(stack);
        if(var.dataType == BOOLEAN){
            var.boolean = !var.boolean;
        }
        return var;
    }

    Variable var2 = removeLastValue(stack);
    Variable var1 = removeLastValue(stack);

    if(var1.dataType != var2.dataType){
        printf("Trying to make operation between %d and %d without a cast\n", (int)var1.dataType, (int)var2.dataType);
    }
    if(operator.operator == MOINS){ // MOINS 4-5
        if(var1.dataType == INTEGER){
            var1.integer -= var2.integer;
        }else if(var1.dataType == FLOAT){
            var1.floattant -= var2.floattant;
        }
    }else if(operator.operator == FOIS){ // FOIS 4*5
        if(var1.dataType == INTEGER){
            var1.integer *= var2.integer;
        }else if(var1.dataType == FLOAT){
            var1.floattant *= var2.floattant;
        }
    }else if(operator.operator == PLUS){ // PLUS 4+5
        if(var1.dataType == INTEGER){
            var1.integer += var2.integer;
        }else if(var1.dataType == FLOAT){
            var1.floattant += var2.floattant;
        }
    }else if(operator.operator == DIVISE){ // DIVISE 4/5
        if(var1.dataType == INTEGER){
            var1.integer /= var2.integer;
        }else if(var1.dataType == FLOAT){
            var1.floattant /= var2.floattant;
        }
    }else if(operator.operator == MODULO){ // MODULO 4%5
        if(var1.dataType == INTEGER){
            var1.integer %= var2.integer;
        }
    }else if(operator.operator == EQ){ // EQ 4==5
        if(var1.dataType == INTEGER){
            var1.boolean = var1.integer == var2.integer;
        }else if(var1.dataType == FLOAT){
            var1.boolean = var1.floattant == var2.floattant;
        }
        var1.dataType = BOOLEAN;
    }else if(operator.operator == NEQ){ // NEQ 4!=5
        if(var1.dataType == INTEGER){
            var1.boolean = var1.integer != var2.integer;
        }else if(var1.dataType == FLOAT){
            var1.boolean = var1.floattant != var2.floattant;
        }
        var1.dataType = BOOLEAN;
    }else if(operator.operator == LT){ // LT 4<5
        if(var1.dataType == INTEGER){
            var1.boolean = var1.integer < var2.integer;
        }else if(var1.dataType == FLOAT){
            var1.boolean = var1.floattant < var2.floattant;
        }
        var1.dataType = BOOLEAN;
    }else if(operator.operator == GT){ // GT 4>5
        if(var1.dataType == INTEGER){
            var1.boolean = var1.integer > var2.integer;
        }else if(var1.dataType == FLOAT){
            var1.boolean = var1.floattant > var2.floattant;
        }
        var1.dataType = BOOLEAN;
    }else if(operator.operator == LE){ // LE 4<=5
        if(var1.dataType == INTEGER){
            var1.boolean = var1.integer <= var2.integer;
        }else if(var1.dataType == FLOAT){
            var1.boolean = var1.floattant <= var2.floattant;
        }
        var1.dataType = BOOLEAN;
    }else if(operator.operator == GE){ // GE 4>=5
        if(var1.dataType == INTEGER){
            var1.boolean = var1.integer >= var2.integer;
        }else if(var1.dataType == FLOAT){
            var1.boolean = var1.floattant >= var2.floattant;
        }
        var1.dataType = BOOLEAN;
    }else if(operator.operator == GE){ // AND True && False
        if(var1.dataType == BOOLEAN){
            var1.boolean = var1.boolean && var2.boolean;
        }
    }else if(operator.operator == GE){ // OR True || False
        if(var1.dataType == BOOLEAN){
            var1.boolean = var1.integer || var2.integer;
        }
    }
    return var1;
}

void runProgram(Program myPrgm){
    Stack calculStore = newStack();
    int exectutedLine = 0;
    while (exectutedLine<myPrgm->lastElement){
        // Loop throw program actions
        Action currentAction = getAction(myPrgm, exectutedLine);

        if(currentAction.actionType == STACK_VALUE){ // Add value to stack
            appendVar(calculStore, currentAction.var);
            exectutedLine+=1;
        }else if(currentAction.actionType == CALCUL){ // execut operation
            Variable response = runCalcul(currentAction.var, calculStore);
            appendVar(calculStore, response);
            exectutedLine+=1;
        }else if(currentAction.actionType == PRINT){ // execut operation
            Variable printedValue = removeLastValue(calculStore);
            displayVariable(printedValue);
            exectutedLine+=1;
        }else{
            printf("Other Actions\n");
        }
    }
    freeStack(calculStore);
}

// int main(){
//     Program myProgm = newPrgm();
//     storeAction(myProgm, (Action){ .actionType = STACK_VALUE, .var = { .dataType = INTEGER, .integer = 2 } });
//     storeAction(myProgm, (Action){ .actionType = STACK_VALUE, .var = { .dataType = INTEGER, .integer = 6 } });
//     storeAction(myProgm, (Action){ .actionType = STACK_VALUE, .var = { .dataType = INTEGER, .integer = 5 } });
//     storeAction(myProgm, (Action){ .actionType = CALCUL, .var = { .dataType = OPERATOR, .operator = PLUS } });
//     storeAction(myProgm, (Action){ .actionType = STACK_VALUE, .var = { .dataType = INTEGER, .integer = 4 } });
//     storeAction(myProgm, (Action){ .actionType = STACK_VALUE, .var = { .dataType = INTEGER, .integer = 3 } });
//     storeAction(myProgm, (Action){ .actionType = CALCUL, .var = { .dataType = OPERATOR, .operator = PLUS } });
//     storeAction(myProgm, (Action){ .actionType = CALCUL, .var = { .dataType = OPERATOR, .operator = FOIS } });
//     storeAction(myProgm, (Action){ .actionType = CALCUL, .var = { .dataType = OPERATOR, .operator = MOINS } });
//     storeAction(myProgm, (Action){ .actionType = PRINT});
//     storeAction(myProgm, (Action){ .actionType = STACK_VALUE, .var = { .dataType = INTEGER, .integer = 3 } });
//     storeAction(myProgm, (Action){ .actionType = PRINT});
//     runProgram(myProgm);
//     freeProgram(myProgm);
// }