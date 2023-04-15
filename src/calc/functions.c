#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef bool
#include <stdbool.h>
#endif
#ifndef CALCULS
#include "calculs.h"
#endif

#include "functions.h"

/* Init empty stack */

FctParameters addParameter(int calc, FctParameters nextPara){
    FctParameters newPara = malloc(sizeof(FctParameters));
    newPara->calc = calc;
    newPara->value = newStack();
    newPara->nextParameter = nextPara;
}

void freeParameter(FctParameters parameter){
    if(parameter){
        freeParameter(parameter->nextParameter);
        freeStack(parameter->value);
        free(parameter);
    }
}

ParaResponse getCallBack(FctParameters parameter, Data myData, CalcStorage myCalculs, Stack myStack, int waitingDepth){
    if(parameter->nextParameter && (waitingDepth>=2 || waitingDepth<0)){
        ParaResponse response = getCallBack(parameter->nextParameter, myData, myCalculs, myStack, waitingDepth-1);
        if(isFctInPara(response)){
            incrementDepth(response);
            return response;
        }
        freeResp(response);
    }
    
    ParaResponse response = initResp(getCalcCallBack(getCalc(myCalculs, parameter->calc), myData, myCalculs, myStack));
    if(isFctInPara(response)){
        return response;
    }
    appendInt(parameter->value, runCalcul(getCalc(myCalculs, parameter->calc), myData));
    return response;
}

void getParametersValues(FctParameters parameter, Stack myStack){
    if(parameter){
        getParametersValues(parameter->nextParameter, myStack);
        appendInt(myStack, removeLastValue(parameter->value));
    }
}

ParaResponse initResp(char *fctName){
    ParaResponse resp = malloc(sizeof(ParaResponse));
    resp->depth = 1;
    char *myName = malloc(strlen(fctName)*sizeof(char));
    strcpy(myName, fctName);
    resp->funcName = myName;
    return resp;
}

void incrementDepth(ParaResponse resp){
    resp->depth = resp->depth +1;
}

void freeResp(ParaResponse resp){
    free(resp->funcName);
    free(resp);
}

bool isFctInPara(ParaResponse resp){
    return strcmp(resp->funcName, "")!=0;
}

/* --- Stack for functions --- */

FctStack initFctStack(){
    FctStack myStacks = malloc(sizeof(FctStack));
    myStacks->values = newStack();
    myStacks->waitingResponse = newStack();
    return myStacks;
}

void freeFctStack(FctStack myFctStack){
    freeStack(myFctStack->values);
    freeStack(myFctStack->waitingResponse);
    free(myFctStack);
}

/* --- Functions --- */

FctRegister initFct(char *name, FctParameters parameters){
    FctRegister myFct = malloc(sizeof(FctRegister));
    char *myname = malloc(strlen(name)*sizeof(char));
    strcpy(myname, name);
    myFct->name = myname;
    myFct->parameters = parameters;
    myFct->stacks = initFctStack();
    return myFct;
}

void freeFctRegistered(FctRegister fct){
    freeParameter(fct->parameters);
    freeFctStack(fct->stacks);
    free(fct->name);
    free(fct);
}

char *getFctCallBack(FctRegister fct, Data myData, CalcStorage myCalc, Stack myStack){
    int waiting;
    if(isVarExist(myData, "return") && !StackisEmpty(fct->stacks->waitingResponse)){
        waiting = removeLastValue(fct->stacks->waitingResponse);
    }else{
        waiting = -1;
    }

    if(waiting==0){
        appendInt(fct->stacks->values, getVar(myData, "return")->value);
        deleteVar(myData, "return");
        return "";
    }

    if(!fct->parameters){
        appendInt(fct->stacks->waitingResponse, 0);
        return fct->name;
    }

    ParaResponse resp = getCallBack(fct->parameters, myData, myCalc, myStack, waiting);
    if(isFctInPara(resp)){
        appendInt(fct->stacks->waitingResponse, resp->depth);
        return resp->funcName;
    }
    freeResp(resp);

    removeStack(myStack);
    getParametersValues(fct->parameters, myStack);
    appendInt(fct->stacks->waitingResponse, 0);
    return fct->name;
}

/* Init line of fct type */

AllCalcFct noFctinCalc(){
    int size = 4;
    AllCalcFct initPgrm = malloc(sizeof(AllCalcFct)); 
    initPgrm->length = size;
    initPgrm->lastElement = 0;
    initPgrm->line = malloc(size*sizeof(FctRegister));
    initPgrm->waitingFunctions = newStack();
    return initPgrm;
}

/* Store action in AllCalcFct */

void storeFctCalc(AllCalcFct allFct, FctRegister fct){
    if(allFct->lastElement == allFct->length){
        int size = 2* allFct->length;
        FctRegister *line = allFct->line;
        allFct->line = malloc(size*sizeof(FctRegister));
        for(int i = 0; i<allFct->length; i=i+1){
            allFct->line[i] = line[i];
        }
        free(line);
        allFct->length = size;
        allFct->line[allFct->lastElement] = fct;
        allFct->lastElement=allFct->lastElement+1;
    }else{
        allFct->line[allFct->lastElement] = fct;
        allFct->lastElement=allFct->lastElement+1;
    }
}

char *getCallBackInAll(AllCalcFct allFct, Data myData, CalcStorage myCalc, Stack myStack){
    int beginning;
    if(isVarExist(myData, "return") && !StackisEmpty(allFct->waitingFunctions)){
        beginning = removeLastValue(allFct->waitingFunctions);
    }else{
        beginning = 0;
    }
    for(int i=beginning;i<allFct->lastElement; i=i+1){
        char *response = getFctCallBack(allFct->line[i], myData, myCalc, myStack);
        if(strcmp(response, "")!=0){
            appendInt(allFct->waitingFunctions, i);
            return response;
        }
    }
    return "";
}

void freeAllCalcFct(AllCalcFct allFct){
    for(int i=0; i<allFct->lastElement; i=i+1){
        freeFctRegistered(allFct->line[i]);
    }
    freeStack(allFct->waitingFunctions);
    free(allFct->line);
    free(allFct);
}