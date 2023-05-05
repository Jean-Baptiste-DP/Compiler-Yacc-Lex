#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "calculs.h"

char *concatString2(char *str1, char *str2){
    char *value = malloc(sizeof(char)*(strlen(str1)+strlen(str2)+1));
    strcpy(value, str1);
    strcat(value, str2);
    return value;
}

char *concatString3(char *str1, char *str2, char *str3){
    char *value = malloc(sizeof(char)*(strlen(str1)+strlen(str2)+strlen(str3)+1));
    strcpy(value, str1);
    strcat(value, str2);
    strcat(value, str3);
    return value;
}

/* TODO free concatenated string */

Calcul VarCalc(char *name){
    Calcul myCalc = malloc(sizeof(struct calcul));
    Variable myVar = newVarInt(name, "__treeCalcVar__", 5);
    myCalc->var = myVar;
    myCalc->params = NULL;
    myCalc->values = NULL;
    myCalc->waitingResponse = NULL;
    return myCalc;
}

Calcul ConstCalcInt(int constante){
    Calcul myCalc = malloc(sizeof(struct calcul));
    // Variable myConstVar = newVarInt("", "int", constante);
    Variable myConstVar = newVarInt("", "int", constante);
    myCalc->var = myConstVar;
    myCalc->params = NULL;
    myCalc->values = NULL;
    myCalc->waitingResponse = NULL;
    return myCalc;
}

Calcul ConstCalcFloat(float constante){
    Calcul myCalc = malloc(sizeof(struct calcul));
    Variable myConstVar = newVarFloat("", "float", constante);
    myCalc->var = myConstVar;
    myCalc->params = NULL;
    myCalc->values = NULL;
    myCalc->waitingResponse = NULL;
    return myCalc;
}

Calcul FctCalc(char *name, CalcParameters parameters, int method){
    Calcul myCalc = malloc(sizeof(struct calcul));
    Variable myFctVar = newVarInt(name, "__treeCalcFct__", method);
    myCalc->var = myFctVar;
    myCalc->params = parameters;
    myCalc->values = newData();
    myCalc->waitingResponse = newStack();
    return myCalc;
}

void freeCalcul(Calcul calc){
    freeVar(calc->var);
    freeParameters(calc->params);
    freeData(calc->values);
    freeStack(calc->waitingResponse);
    free(calc);
}

CalcParameters newParameter(Calcul calc, CalcParameters nextParam){
    CalcParameters myParam = malloc(sizeof(struct calcParameters));
    myParam->param = calc;
    myParam->next = nextParam;
    return myParam;
}

void freeParameters(CalcParameters parameters){
    if(parameters){
        freeCalcul(parameters->param);
        freeParameters(parameters->next);
        free(parameters);
    }
}

char *getCalcCallBack(Calcul myCalc, Data myData, Data myStack){
    if(strcmp(myCalc->var->info->type, "__treeCalcVar__")==0 || strcmp(myCalc->var->info->type, "__treeCalcFct__")!=0){
        return "";
    }

    int waiting;
    if(isVarExist(myData, "return") && !StackisEmpty(myCalc->waitingResponse)){
        waiting = removeLastValue(myCalc->waitingResponse);
    }else{
        waiting = -1;
    }

    if(waiting==0){
        storeVar(myCalc->values, getVar(myData, "return"));
        removeVar(myData, "return");
        return "";
    }
    if(!myCalc->params){
        appendInt(myCalc->waitingResponse, 0);
        return concatString2(" /", myCalc->var->info->name);
    }

    ParaResponse resp = getCallBack(myCalc->params, myData, myStack, waiting);
    if(strcmp(resp->funcName, "")!=0){
        appendInt(myCalc->waitingResponse, resp->depth);
        return resp->funcName;
    }
    freeResp(resp);
    removeData(myStack);
    getParametersValues(myCalc->params, myStack, myData);
    appendInt(myCalc->waitingResponse, 0);

    if(myCalc->var->intValue==0){
        return concatString2(" /", myCalc->var->info->name);
    }else{
        return concatString3(myStack->myData->var->info->type, "/", myCalc->var->info->name);
    }
}

ParaResponse getCallBack(CalcParameters params, Data myData, Data myStack, int waiting){
    if(params){
        if(params->next && (waiting>=2 || waiting<0)){
            ParaResponse response = getCallBack(params->next, myData, myStack, waiting-1);
            if(strcmp(response->funcName, "")!=0){
                response->depth = response->depth+1;
                return response;
            }
            freeResp(response);
        }

        ParaResponse response = initResp(getCalcCallBack(params->param, myData, myStack));
        return response;
    }else{
        return initResp("");
    }
}

void getParametersValues(CalcParameters params, Data myStack, Data myData){
    if(params){
        getParametersValues(params->next, myStack, myData);
        Variable var = runCalcul(params->param, myData);
        storeVar(myStack, var);
    }
}

Variable runCalcul(Calcul myCalc, Data myData){
    if(strcmp(myCalc->var->info->type, "__treeCalcVar__")==0){
        if(isVarExist(myData, myCalc->var->info->name)){
            return copyVar(myData, myCalc->var->info->name);
        }else{
            printf("Variable %s doesn't exist\n", myCalc->var->info->name);
            return newVar("", "");
        }
    }else if(strcmp(myCalc->var->info->type, "__treeCalcFct__")==0){
        if(!isEmpty(myCalc->values)){
            return lastValue(myCalc->values);
        }else{
            printf("Problem with function %s execution\n", myCalc->var->info->name);
        }
    }else{
        return duplicateVar(myCalc->var);
    }
}

ParaResponse initResp(char *fctName){
    ParaResponse resp = malloc(sizeof(struct paraResponse));
    resp->depth = 1;
    char *myName = malloc((strlen(fctName)+1)*sizeof(char));
    strcpy(myName, fctName);
    resp->funcName = myName;
    return resp;
}

void freeResp(ParaResponse resp){
    free(resp->funcName);
    free(resp);
}

/* Calcul storage */

CalcStorage newCalcStorage(){
    int size = 4;
    CalcStorage initStor = malloc(sizeof(struct calcLine));
    initStor->length = size;
    initStor->lastElement = 0;
    initStor->line = malloc(size*sizeof(Calcul));
    return initStor;
}

int storeCalcul(CalcStorage storage, Calcul calc){
    if(storage->lastElement == storage->length){
        int size = 2* storage->length;
        Calcul *line = storage->line;
        storage->line = malloc(size*sizeof(Calcul));
        for(int i = 0; i<storage->length; i=i+1){
            storage->line[i] = line[i];
        }
        free(line);
        storage->length = size;
        storage->line[storage->lastElement] = calc;
        storage->lastElement=storage->lastElement+1;
    }else{
        storage->line[storage->lastElement] = calc;
        storage->lastElement=storage->lastElement+1;
    }
    return storage->lastElement-1;
}

Calcul getCalc(CalcStorage storage, int index){
    if(index<storage->lastElement){
        return storage->line[index];
    }else{
        printf("There is no Calcul with index %d\n", index);
        return ConstCalcInt(0);
    }
}

void freeCalcStorage(CalcStorage storage){
    for(int i=0; i<storage->lastElement; i=i+1){
        freeCalcul(storage->line[i]);
    }
    free(storage->line);
    free(storage);
}