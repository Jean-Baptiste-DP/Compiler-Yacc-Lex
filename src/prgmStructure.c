#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include "prgmStructure.h"



/* --- Actions --- */

Action newAction(int type,char *var,int line,int calc){
    Action act = malloc(sizeof(Action));
    char *myVar = malloc(strlen(var)*sizeof(char));
    strcpy(myVar, var);
    act->varName = myVar;
    act->type = type;
    act->line = line;
    act->calc = calc;
    return act;
}

void freeAction(Action act){
    free(act->varName);
    free(act);
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
        return 0;
    }
}

void freeProgram(Program myPgrm){
    for(int i=0; i<myPgrm->lastElement; i=i+1){
        freeAction(myPgrm->line[i]);
    }
    free(myPgrm->line);
    free(myPgrm);
}

int gotoFrom(Stack myStack, Program myPrgm){
    int pos = storeAction(myPrgm,newAction(4,"",-1,0));
    appendInt(myStack,pos);
    return pos;
}
void gotoDest(Stack myStack, Program myPrgm, int additionalPos){
    myPrgm->line[removeLastValue(myStack)]->line = myPrgm->lastElement+additionalPos;
}

void forEndGoto(Stack myStack, Program myPrgm, char *loopVar){
    int firstPos = removeLastValue(myStack);
    myPrgm->line[firstPos]->line = storeAction(myPrgm,newAction(4,"",firstPos-1,0))+1;
    storeAction(myPrgm, newAction(6,loopVar,0,0));
}

void whileEndGoto(Stack myStack, Program myPrgm){
    int firstPos = removeLastValue(myStack);
    myPrgm->line[firstPos]->line = storeAction(myPrgm,newAction(4,"",firstPos-1,0))+1;
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
            printf("Assignment var : %s\n", myPrgm->line[i]->varName);
        }else if(myPrgm->line[i]->type==6){
            printf("Kill var : %s\n", myPrgm->line[i]->varName);
        }else if(myPrgm->line[i]->type==5){
            printf("Return calc : %d\n", myPrgm->line[i]->calc);
        }else{
            printf("Action %d\n", myPrgm->line[i]->type);
        }
    }
}

void runProgram(Program myPrgm, CalcStorage calculs, Data variables, Stack myStack){
    int i = 0;
    Action currentAction = getAction(myPrgm, i);
    while(i<myPrgm->lastElement){
        if(currentAction->type==0){ /* assigment */
            if(StackisEmpty(myStack)){
                if(currentAction->calc>=0){
                    char *response = getCalcCallBack(getCalc(calculs, currentAction->calc), variables, calculs, myStack); 
                    if(strcmp(response, "")==0){
                        if(isVarExist(variables, currentAction->varName)){
                            getVar(variables, currentAction->varName)->value = runCalcul(getCalc(calculs, currentAction->calc), variables);
                        }else{
                            printf("Variable \"%s\" hasn't been declared\nlet %s; to declare it\n", currentAction->varName, currentAction->varName);
                        }
                        i = i+1;
                    }else{
                        if(isVarExist(variables, response) && strcmp(getVar(variables, response)->type, "function")==0){
                            storeVar(variables, newVar("", "context", i));
                            i = getVar(variables, response)->value;
                        }else{
                            printf("The function %s doesn't exist\n", response);
                            i = i+1;
                        }
                    }
                }else{
                    i = i+1;
                }
            }else{
                if(isVarExist(variables, currentAction->varName)){
                    getVar(variables, currentAction->varName)->value = removeLastValue(myStack);
                }else{
                    storeVar(variables, newVar(currentAction->varName, "int", removeLastValue(myStack)));
                }
                i = i+1;
            }
        }else if(currentAction->type==1){/* new Variable */
            if(StackisEmpty(myStack)){
                if(currentAction->calc>=0){
                    char *response = getCalcCallBack(getCalc(calculs, currentAction->calc), variables, calculs, myStack); 
                    if(strcmp(response, "")==0){
                        if(isVarExistInContext(variables, currentAction->varName)){
                            printf("1 - Variable \"%s\" has already been declared\n", currentAction->varName);
                        }else{
                            if(currentAction->calc>=0){
                                storeVar(variables, newVar(currentAction->varName, "int", runCalcul(getCalc(calculs, currentAction->calc), variables)));
                            }
                        }
                        i = i+1;
                    }else{
                        if(isVarExist(variables, response) && strcmp(getVar(variables, response)->type, "function")==0){
                            storeVar(variables, newVar("", "context", i));
                            i = getVar(variables, response)->value;
                        }else{
                            printf("The function %s doesn't exist\n", response);
                            i = i+1;
                        }
                    }
                }else{
                    if(!isVarExistInContext(variables, currentAction->varName)){
                        storeVar(variables, newVar(currentAction->varName, "int", 0));
                    }
                    i = i+1;
                }
            }else{
                if(isVarExistInContext(variables, currentAction->varName)){
                    printf("2 - Variable \"%s\" has already been declared\n", currentAction->varName);
                }else{
                    storeVar(variables, newVar(currentAction->varName, "int", removeLastValue(myStack)));
                }
                i = i+1;
            }
        }
        else if(currentAction->type==2){/* print calcul */
            char *response = getCalcCallBack(getCalc(calculs, currentAction->calc), variables, calculs, myStack);
            if(strcmp(response, "")==0){
                printf("%d\n", runCalcul(getCalc(calculs, currentAction->calc), variables));
                i = i+1;
            }else{
                if(isVarExist(variables, response) && strcmp(getVar(variables, response)->type, "function")==0){
                    storeVar(variables, newVar("", "context", i));
                    i = getVar(variables, response)->value;
                }else{
                    printf("The function %s doesn't exist\n", response);
                    i = i+1;
                }
            }
        }else if(currentAction->type==3){/* if function */
            char *response = getCalcCallBack(getCalc(calculs, currentAction->calc), variables, calculs, myStack);
            if(strcmp(response, "")==0){
                if(runCalcul(getCalc(calculs, currentAction->calc), variables)){
                    i = i+2;
                }else{
                    i = i+1;
                }
            }else{
                if(isVarExist(variables, response) && strcmp(getVar(variables, response)->type, "function")==0){
                    storeVar(variables, newVar("", "context", i));
                    i = getVar(variables, response)->value;
                }else{
                    printf("The function %s doesn't exist\n", response);
                    i = i+1;
                }
            }
        }else if(currentAction->type==4){/* goto line */
            if(currentAction->line>=0){
                i = currentAction->line;
            }
        }else if(currentAction->type==5){ /* exit fct */
            if(currentAction->calc>=0){
                char *response = getCalcCallBack(getCalc(calculs, currentAction->calc), variables, calculs, myStack);
                if(strcmp(response, "")==0){
                    int returnValue = runCalcul(getCalc(calculs, currentAction->calc), variables);
                    i = freeContext(variables);
                    storeVar(variables, newVar("return", "return", returnValue));
                    
                }else{
                    if(isVarExist(variables, response) && strcmp(getVar(variables, response)->type, "function")==0){
                        storeVar(variables, newVar("", "context", i));
                        i = getVar(variables, response)->value;
                    }else{
                        printf("The function %s doesn't exist\n", response);
                        i = i+1;
                    }
                }
            }else{
                i = freeContext(variables);
            }
        }else if(currentAction->type==6){
            deleteVar(variables, currentAction->varName);
            i = i+1;
        }else{
            i = i+1;
        }
        if(i<myPrgm->lastElement){
            currentAction = getAction(myPrgm, i);
        }
    }
}