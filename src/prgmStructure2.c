#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include "prgmStructure2.h"

// for sleep prgm
#include <unistd.h>

/* --- Actions --- */

Action newAction(int type,char *varName,int line,int calc, char *varType){
    /* BUG if switch line 18 or 19 -> impossible to free the pointer line 18 */
    VarInfo myVarInfo = newVarInfo(varType, varName);
    Action act = malloc(sizeof(Action));
    act->type = type;
    act->line = line;
    act->calc = calc;
    act->var = myVarInfo;
    return act;
}

void freeAction(Action act){
    freeVarInfo(act->var);
    printf("step 1\n");
    free(act);
    printf("step 2\n");
}

/* -- Program Actions --- */

Program newPrgm(){
    int size = 4;
    Program initPgrm = malloc(sizeof(Program)); 
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
    for(int i=0; i<myPgrm->lastElement; i=i+1){
        /* BUG freeing type 0 or 1 */
        freeAction(myPgrm->line[i]);
    }
    free(myPgrm->line);
    free(myPgrm);
}

int gotoFrom(Stack myStack, Program myPrgm){
    int pos = storeAction(myPrgm,newAction(4,"",-1,0, ""));
    appendInt(myStack,pos);
    return pos;
}
void gotoDest(Stack myStack, Program myPrgm, int additionalPos){
    myPrgm->line[removeLastValue(myStack)]->line = myPrgm->lastElement+additionalPos;
}

void forEndGoto(Stack myStack, Program myPrgm, char *loopVar){
    int firstPos = removeLastValue(myStack);
    myPrgm->line[firstPos]->line = storeAction(myPrgm,newAction(4,"",firstPos-1,0, ""))+1;
    storeAction(myPrgm, newAction(6,loopVar,0,0, ""));
}

void whileEndGoto(Stack myStack, Program myPrgm){
    int firstPos = removeLastValue(myStack);
    myPrgm->line[firstPos]->line = storeAction(myPrgm,newAction(4,"",firstPos-1,0, ""))+1;
}

void displayPrgm(Program myPrgm){
    for(int i=0; i<myPrgm->lastElement;i = i+1){
        printf("Line %d - ", i);
        if(myPrgm->line[i]->type==2){
            printf("Print calcul : %d\n", myPrgm->line[i]->calc);
        }else if(myPrgm->line[i]->type==3){
            printf("If calcul : %d\n", myPrgm->line[i]->calc);
        }else if(myPrgm->line[i]->type==4){
            printf("Goto line : %d\n", myPrgm->line[i]->line);
        }else if(myPrgm->line[i]->type<2){
            printf("Assignment var : %s\n", myPrgm->line[i]->var->name);
        }else if(myPrgm->line[i]->type==6){
            printf("Kill var : %s\n", myPrgm->line[i]->var->name);
        }else if(myPrgm->line[i]->type==5){
            printf("Return calc : %d\n", myPrgm->line[i]->calc);
        }else{
            printf("Action %d\n", myPrgm->line[i]->type);
        }
    }
}

void runProgram(Program myPrgm, CalcStorage calculs, Data variables, Data myStack){
    int i = 0;
    Action currentAction = getAction(myPrgm, i);
    while(i<myPrgm->lastElement){
        // printf("\x1B[32m");
        // printf("Running line %d - Action %d\n", i, currentAction->type);
        // printf("\x1B[0m");
        // sleep(1);
        // printf("\x1B[31m");
        // printAllVariables(myStack->myData);
        // printf("\x1B[0m");
        if(currentAction->type==0){ /* assigment */
            if(isEmpty(myStack)){
                if(currentAction->calc>=0){
                    char *response = getCalcCallBack(getCalc(calculs, currentAction->calc), variables, myStack);
                    if(strcmp(response, "")==0){
                        if(isVarExist(variables, currentAction->var->name)){
                            Variable gettedValue = runCalcul(getCalc(calculs, currentAction->calc), variables);
                            Variable previousVar = getVar(variables, currentAction->var->name);
                            if(strcmp(gettedValue->info->type, previousVar->info->type)==0){
                                if(strcmp(previousVar->info->type, "int")==0){
                                    previousVar->intValue = gettedValue->intValue;
                                }else if(strcmp(previousVar->info->type, "float")==0){
                                    previousVar->floatValue = gettedValue->floatValue;
                                }
                            }else{
                                printf("Cannot match types %s with %s\n", previousVar->info->type, gettedValue->info->type);
                            }
                            freeVar(gettedValue);
                        }else{
                            printf("Variable \"%s\" hasn't been declared\nlet %s; to declare it\n", currentAction->var->name, currentAction->var->name);
                        }
                        i = i+1;
                    }else{
                        int nextLine = runFunction(response, myStack, variables, i);
                        if(nextLine!=-1){
                            i = nextLine;
                        }
                    }
                }else{
                    i = i+1;
                }
            }else{
                if(isVarExist(variables, currentAction->var->name)){
                    Variable gettedValue = lastValue(myStack);
                    Variable previousVar = getVar(variables, currentAction->var->name);
                    if(strcmp(gettedValue->info->type, previousVar->info->type)==0){
                        if(strcmp(previousVar->info->type, "int")==0){
                            previousVar->intValue = gettedValue->intValue;
                        }else if(strcmp(previousVar->info->type, "float")==0){
                            previousVar->floatValue = gettedValue->floatValue;
                        }
                    }else{
                        printf("Cannot match types %s with %s\n", previousVar->info->type, gettedValue->info->type);
                    }
                    freeVar(gettedValue);
                }else{
                    Variable storedValue = lastValue(myStack);
                    changeName(storedValue, currentAction->var->name, storedValue->info->type);
                    storeVar(variables, storedValue);
                }
                i = i+1;
            }
        }else if(currentAction->type==1){/* new Variable */
            if(isEmpty(myStack)){
                if(currentAction->calc>=0){
                    char *response = getCalcCallBack(getCalc(calculs, currentAction->calc), variables, myStack); 
                    if(strcmp(response, "")==0){
                        if(isVarExistInContext(variables, currentAction->var->name)){
                            printf("Variable \"%s\" has already been declared\n", currentAction->var->name);
                        }else{
                            if(currentAction->calc>=0){
                                Variable gettedVar = runCalcul(getCalc(calculs, currentAction->calc), variables);
                                if(strcmp(gettedVar->info->type, currentAction->var->type)==0){
                                    changeName(gettedVar, currentAction->var->name, gettedVar->info->type);
                                    storeVar(variables, gettedVar);
                                }else{
                                    printf("Can't assign %s value to %s variable\n", gettedVar->info->type, currentAction->var->type);
                                }
                            }
                        }
                        i = i+1;
                    }else{
                        int nextLine = runFunction(response, myStack, variables, i);
                        if(nextLine!=-1){
                            i = nextLine;
                        }
                    }
                }else{
                    if(!isVarExistInContext(variables, currentAction->var->name)){
                        storeVar(variables, newVar(currentAction->var->name, currentAction->var->type));
                    }
                    i = i+1;
                }
            }else{
                if(isVarExistInContext(variables, currentAction->var->name)){
                    printf("Variable \"%s\" has already been declared\n", currentAction->var->name);
                }else{
                    Variable storedValue = lastValue(myStack);
                    if(strcmp(storedValue->info->type, currentAction->var->type)==0){
                        changeName(storedValue, currentAction->var->name, storedValue->info->type);
                        storeVar(variables, storedValue);
                    }else{
                        printf("Can't assign %s value to %s variable\n", storedValue->info->type, currentAction->var->type);
                    }
                }
                i = i+1;
            }
        }
        else if(currentAction->type==2){/* print calcul */
            char *response = getCalcCallBack(getCalc(calculs, currentAction->calc), variables, myStack);
            if(strcmp(response, "")==0){
                Variable gettedVar = runCalcul(getCalc(calculs, currentAction->calc), variables);
                if(strcmp(gettedVar->info->type, "int")==0){
                    printf("%d\n", gettedVar->intValue);
                }else if(strcmp(gettedVar->info->type, "float")==0){
                    printf("%f\n", gettedVar->floatValue);
                }
                freeVar(gettedVar);
                i = i+1;
            }else{
                int nextLine = runFunction(response, myStack, variables, i);
                if(nextLine!=-1){
                    i = nextLine;
                }
            }
        }else if(currentAction->type==3){/* if function */
            char *response = getCalcCallBack(getCalc(calculs, currentAction->calc), variables, myStack);
            if(strcmp(response, "")==0){
                Variable gettedValue= runCalcul(getCalc(calculs, currentAction->calc), variables);
                if(strcmp(gettedValue->info->type, "int")==0 && gettedValue->intValue){
                    i = i+2;
                }else{
                    i = i+1;
                }
                freeVar(gettedValue);
            }else{
                printf("Get function %s\n", response);
                int nextLine = runFunction(response, myStack, variables, i);
                if(nextLine!=-1){
                    i = nextLine;
                }
            }
        }else if(currentAction->type==4){/* goto line */
            if(currentAction->line>=0){
                i = currentAction->line;
            }else{
                printf("Wrong position in Goto\n");
                exit(1);
            }
        }else if(currentAction->type==5){ /* exit fct */
            if(currentAction->calc>=0){
                char *response = getCalcCallBack(getCalc(calculs, currentAction->calc), variables, myStack);
                if(strcmp(response, "")==0){
                    
                    Variable returnValue = runCalcul(getCalc(calculs, currentAction->calc), variables);
                    i = freeContext(variables);
                    changeName(returnValue, "return", returnValue->info->type);
                    storeVar(variables, returnValue);
                    
                }else{
                    int nextLine = runFunction(response, myStack, variables, i);
                    if(nextLine!=-1){
                        i = nextLine;
                    }
                }
            }else{
                i = freeContext(variables);
            }
        }else if(currentAction->type==6){ /* delete var */
            deleteVar(variables, currentAction->var->name);
            i = i+1;
        }else{
            i = i+1;
        }
        if(i<myPrgm->lastElement){
            currentAction = getAction(myPrgm, i);
        }
    }
}