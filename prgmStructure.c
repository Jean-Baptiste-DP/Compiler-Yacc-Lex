#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include "prgmStructure.h"

/* --- Calcul symbole --- */

Symbole newSymbole(int type, int val, char *var){
    Symbole initSymb = malloc(sizeof(Symbole));
    char *myvar = malloc(strlen(var));
    strcpy(myvar, var);
    initSymb->variable = myvar;
    initSymb->value = val;
    initSymb->type = type;
    return initSymb;
}

Symbole freeSymbole(Symbole mysym){
    free(mysym->variable);
    free(mysym);
}

/* --- Calcul structure --- */

/*
type = "const" = 0
-> value : real value

type = "var" = 1
-> value stored in variable

type = "fct" = 2
-> value stored in variable function

type = "ope" = 3
-> value : 
0 <-> "*"
1 <-> "/"
2 <-> "+"
3 <-> "-"
4 <-> "%"
5 <-> "UMINUS"
6 <-> "!"
7 <-> "||"
8 <-> "&&"
9 <-> "=="
10 <-> "!="
11 <-> ">="
12 <-> "<="
13 <-> ">"
14 <-> "<"
*/


/* Init constant value */

CalculNb leafConst(int value){

    CalculNb leaf = malloc(sizeof(CalculNb));
    Symbole sym = newSymbole(0, value, "");
    leaf->symbole = sym;
    leaf->leftChild=NULL;
    leaf->rightChild=NULL;
    
    return leaf;
}

CalculNb leafVar(char *varName){

    CalculNb leaf = malloc(sizeof(CalculNb));
    Symbole sym = newSymbole(1, 0, varName);
    leaf->symbole = sym;
    leaf->leftChild=NULL;
    leaf->rightChild=NULL;

    return leaf;
}

CalculNb leafFct(int fctPosition){

    CalculNb leaf = malloc(sizeof(CalculNb));
    Symbole sym = newSymbole(2, fctPosition, "");
    leaf->symbole=sym;
    leaf->leftChild=NULL;
    leaf->rightChild=NULL;

    return leaf;
}

/* Init node operators */

CalculNb nodeOperator(int operat, CalculNb lChild, CalculNb rChild){

    CalculNb node = malloc(sizeof(CalculNb));
    Symbole sym = newSymbole(3, operat, "");
    node->symbole = sym;
    node->leftChild=lChild;
    node->rightChild=rChild;

    return node;
}

/* Delete Calcul */

void freeCalculNb(CalculNb myCalc){
    if(myCalc->symbole->type==3){
        freeCalculNb(myCalc->leftChild);
        if(myCalc->symbole->value!=5 && myCalc->symbole->value!=6){
            freeCalculNb(myCalc->rightChild);
        }
        freeSymbole(myCalc->symbole);
        free(myCalc);
    }else if (myCalc->symbole->type==0 || myCalc->symbole->type==1 || myCalc->symbole->type==2)
    {
        freeSymbole(myCalc->symbole);
        free(myCalc);
    }else{
        printf("Empty Calc, type : %d\n", myCalc->symbole->type);
    }
}

/* Execute Calcul */

int runCalculNb(CalculNb myCalc, AllCalcFct fct, Data myData){
    if(myCalc->symbole->type==0){
        return myCalc->symbole->value;
    }else if (myCalc->symbole->type==1) /* variable */
    {
        if(isVarExist(myData, myCalc->symbole->variable)){
            return getVar(myData, myCalc->symbole->variable)->value;
        }else{
            return 0;
        }
    }else if(myCalc->symbole->type==2){ /* fct type */
        FctRegister myFct = fct->line[myCalc->symbole->value];
        return removeLastValue(myFct->stacks->values);
    }else if(myCalc->symbole->value==0){
        return runCalculNb(myCalc->leftChild, fct, myData)*runCalculNb(myCalc->rightChild, fct, myData);
    }else if(myCalc->symbole->value==1){
        return runCalculNb(myCalc->leftChild, fct, myData)/runCalculNb(myCalc->rightChild, fct, myData);
    }else if(myCalc->symbole->value==2){
        return runCalculNb(myCalc->leftChild, fct, myData)+runCalculNb(myCalc->rightChild, fct, myData);
    }else if(myCalc->symbole->value==3){
        return runCalculNb(myCalc->leftChild, fct, myData)-runCalculNb(myCalc->rightChild, fct, myData);
    }else if(myCalc->symbole->value==4){
        return runCalculNb(myCalc->leftChild, fct, myData)%runCalculNb(myCalc->rightChild, fct, myData);
    }else if(myCalc->symbole->value==5){
        return -runCalculNb(myCalc->leftChild, fct, myData);
    }else if(myCalc->symbole->value==6){
        return !runCalculNb(myCalc->leftChild, fct, myData);
    }else if(myCalc->symbole->value==7){
        return runCalculNb(myCalc->leftChild, fct, myData)||runCalculNb(myCalc->rightChild, fct, myData);
    }else if(myCalc->symbole->value==8){
        return runCalculNb(myCalc->leftChild, fct, myData)&&runCalculNb(myCalc->rightChild, fct, myData);
    }else if(myCalc->symbole->value==9){
        return runCalculNb(myCalc->leftChild, fct, myData)==runCalculNb(myCalc->rightChild, fct, myData);
    }else if(myCalc->symbole->value==10){
        return runCalculNb(myCalc->leftChild, fct, myData)!=runCalculNb(myCalc->rightChild, fct, myData);
    }else if(myCalc->symbole->value==11){
        return runCalculNb(myCalc->leftChild, fct, myData)>=runCalculNb(myCalc->rightChild, fct, myData);
    }else if(myCalc->symbole->value==12){
        return runCalculNb(myCalc->leftChild, fct, myData)<=runCalculNb(myCalc->rightChild, fct, myData);
    }else if(myCalc->symbole->value==13){
        return runCalculNb(myCalc->leftChild, fct, myData)>runCalculNb(myCalc->rightChild, fct, myData);
    }else if(myCalc->symbole->value==14){
        return runCalculNb(myCalc->leftChild, fct, myData)<runCalculNb(myCalc->rightChild, fct, myData);
    }else{
        printf("Calc don't match possibilities");
        return 0;
    }
}

void incrementFctIndex(CalculNb tree, int num){
    if(tree && tree->symbole->type == 2){
        tree->symbole->value = tree->symbole->value + num;
    }else if(tree && tree->symbole->type == 3){
        incrementFctIndex(tree->leftChild, num);
        incrementFctIndex(tree->rightChild, num);
    }
}


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



Calcul newCalc(CalculNb nb, AllCalcFct fct){
    Calcul myCalc = malloc(sizeof(Calcul));
    myCalc->nb = nb;
    myCalc->fct = fct;
    return myCalc;
}

void freeCalcul(Calcul calc){
    freeCalculNb(calc->nb);
    freeAllCalcFct(calc->fct);
    free(calc);
}

char *getCalcCallBack(Calcul myCalc, Data myData, CalcStorage myCalcStorage, Stack myStack){
    return getCallBackInAll(myCalc->fct, myData, myCalcStorage, myStack);
}

int runCalcul(Calcul myCalc, Data myData){
    return runCalculNb(myCalc->nb, myCalc->fct, myData);
}

Calcul ConstCalc(int constante){
    return newCalc(leafConst(constante), noFctinCalc());
}

Calcul VarCalc(char *name){
    return newCalc(leafVar(name), noFctinCalc());
}

Calcul FctCalc(char *name, FctParameters parameters){
    AllCalcFct myFct = noFctinCalc();
    storeFctCalc(myFct, initFct(name, parameters));
    return newCalc(leafFct(0), myFct);
}

Calcul OpeCalc(int operat, Calcul left, Calcul right){
    int leftLength = left->fct->lastElement;
    int rightLength = right->fct->lastElement;
    if(leftLength>rightLength){
        incrementFctIndex(right->nb, leftLength);
        for(int i=0; i<rightLength; i=i+1){
            storeFctCalc(left->fct, right->fct->line[i]);
        }
        left->nb = nodeOperator(operat, left->nb, right->nb);
        free(right->fct->line);
        free(right->fct);
        free(right);
        return left;
    }else{
        incrementFctIndex(left->nb, rightLength);
        for(int i=0; i<leftLength; i=i+1){
            storeFctCalc(right->fct, left->fct->line[i]);
        }
        right->nb = nodeOperator(operat, left->nb, right->nb);
        free(left->fct->line);
        free(left->fct);
        free(left);
        return right;
    }
}


/* --- Calcul Storage --- */

CalcStorage newCalcStorage(){
    int size = 4;
    CalcStorage initStor = malloc(sizeof(CalcStorage));
    initStor->length = size;
    initStor->lastElement = 0;
    initStor->line = (Calcul *) malloc(size*sizeof(Calcul));
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
        printf("Index out of bound\n");
        return newCalc(leafConst(0), noFctinCalc());
    }
}

void freeCalcStorage(CalcStorage storage){
    for(int i=0; i<storage->lastElement; i=i+1){
        freeCalcul(storage->line[i]);
    }
    free(storage->line);
    free(storage);
}

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