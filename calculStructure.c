#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include "header.h"

/* Initiate a variable */

Variable newVar(char *name, char *type, int value){
    char *myName = malloc(strlen(name)*sizeof(char));
    char *myType = malloc(strlen(type)*sizeof(char));
    strcpy(myName, name);
    strcpy(myType, type);
    Variable var = malloc(sizeof(Variable));
    var->name = myName;
    var->type = myType;
    var->value = value;
    return var;
}

/* Delete a variable */

void freeVar(Variable var){
    free(var->name);
    free(var->type);
    free(var);
}


/* --- Variable storage sturcture --- */

/* Intitiat Data type */

DataStack newDataStack(){
    DataStack initVar = malloc(sizeof(DataStack));
    initVar->var = NULL;
    initVar->next = NULL;
    return initVar;
}

Data newData(){
    Data initData = malloc(sizeof(Data));
    initData->myData = newDataStack();
    return initData;
}

/* Check if any variables exist in DataStack */

bool isEmptyStack(DataStack variables){
    return variables->next==NULL;
}

bool isEmpty(Data variables){
    return isEmptyStack(variables->myData);
}

/* Check if a specific variable exist */

bool isVarExistStack(DataStack variables, char *name){
    if(isEmptyStack(variables)){
        return false;
    }
    else{
        if(strcmp(variables->var->name,name)==0){
            return true;
        }else{
            return isVarExistStack(variables->next,name);
        }
    }
}

bool isVarExist(Data variables, char *name){
    return isVarExistStack(variables->myData, name);
}

/* Return the specific variable */

Variable getVarStack(DataStack variables, char *name){
    if(isEmptyStack(variables)){
        printf("Variable %s doesn't exist\n", name);
        return 0;
    }else{
        if(strcmp(variables->var->name,name)==0){
        return variables->var;
    }else{
        return getVarStack(variables->next,name);
    }
    }
}

Variable getVar(Data variables, char *name){
    return getVarStack(variables->myData, name);
}

/* Delete the specific variable */

DataStack deleteVarStack(DataStack variables, char *name){
    if(!isEmptyStack(variables)){
        if(strcmp(variables->var->name,name)==0){
            DataStack next=variables->next;
            freeVar(variables->var);
            free(variables);
            return next;
        }else{
            variables->next = deleteVarStack(variables->next,name);
            return variables;
        }
    }
}

void deleteVar(Data variables, char *name){
    variables->myData = deleteVarStack(variables->myData, name);
}

/* Add new variable */

DataStack storeVarStack(DataStack variables, Variable var){
    DataStack newDict = malloc(sizeof(DataStack));
    newDict->var = var;
    newDict->next = variables;
    return newDict;
}

void storeVar(Data variables, Variable var){
    variables->myData = storeVarStack(variables->myData, var);
}

/* Delete the entire DataStack */

void freeDataStack(DataStack variables){
    if(!isEmptyStack(variables)){
        freeDataStack(variables->next);
        free(variables->var->name);
        free(variables->var->type);
        free(variables->var);
        free(variables);
    }
}

void freeData(Data variables){
    freeDataStack(variables->myData);
    free(variables);
}

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
        myFct->executed = false;
        printf("From execution of %s, get value %d\n", myFct->name, myFct->value);
        return myFct->value;
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
    if(tree->symbole->type == 2){
        tree->symbole->value = tree->symbole->value + num;
    }else if(tree->symbole->type == 3){
        incrementFctIndex(tree->leftChild, num);
        incrementFctIndex(tree->rightChild, num);
    }
}


/* Init empty stack */

FctParameters addParameter(int calc, FctParameters nextPara){
    FctParameters newPara = malloc(sizeof(FctParameters));
    newPara->calc = calc;
    newPara->executed = false;
    newPara->value = 0;
    newPara->nextParameter = nextPara;
}

void freeParameter(FctParameters parameter){
    if(parameter){
        freeParameter(parameter->nextParameter);
        free(parameter);
    }
}

char *getCallBack(FctParameters parameter, Data myData, CalcStorage myCalculs){
    char *response;
    if(parameter->nextParameter && !parameter->nextParameter->executed){
        printf("CallBack of the next parameter (from %d), ", parameter->calc);
        response = getCallBack(parameter->nextParameter, myData, myCalculs);
        printf("Response got %s\n", response);
        if(strcmp(response, "")!=0){
            return response;
        }
    }
    
    printf("CallBack myself %d, ",parameter->calc);
    response = getCalcCallBack(getCalc(myCalculs, parameter->calc), myData, myCalculs);
    printf("Response got %s\n", response);
    if(strcmp(response, "")!=0){
        return response;
    }

    parameter->value = runCalcul(getCalc(myCalculs, parameter->calc), myData);
    parameter->executed = true;
    return "";
}

IntStack getParametersValues(FctParameters parameter){
    if(parameter){
        IntStack nextValues = getParametersValues(parameter->nextParameter);
        IntStack head = malloc(sizeof(IntStack));
        head->value = parameter->value;
        head->next = nextValues;
        printf("Add new parameter : %d\n", parameter->value);
        return head;
    }else{
        return NULL;
    }
}

void parameterExecutionFalse(FctParameters parameter){
    if(parameter){
        parameterExecutionFalse(parameter->nextParameter);
        parameter->executed = false;
    }
}

FctRegister initFct(char *name, FctParameters parameters){
    FctRegister myFct = malloc(sizeof(FctRegister));
    char *myname = malloc(strlen(name));
    strcpy(myname, name);
    myFct->name = myname;
    myFct->parameters = parameters;
    myFct->executed = false;
    myFct->value = 0;
    return myFct;
}

void freeFctRegistered(FctRegister fct){
    free(fct->name);
    freeParameter(fct->parameters);
    free(fct);
}

char *getFctCallBack(FctRegister fct, Data myData, CalcStorage myCalc){
    if(isVarExist(myData, "return") && (!fct->parameters || fct->parameters->executed)){
        printf("%s get return value %d\n", fct->name ,getVar(myData, "return")->value);
        fct->value = getVar(myData, "return")->value;
        deleteVar(myData, "return");
        fct->executed = true;
        if(fct->parameters){
            parameterExecutionFalse(fct->parameters);
        }
        return "";
    }

    if(!fct->parameters){
        printf("No parameters in function\n");
        return fct->name;
    }else{
        char *response;
        printf("CallBack in parameter of function %s\n", fct->name);
        response = getCallBack(fct->parameters, myData, myCalc);
        printf("Response received %s\n", response);
        if(strcmp(response,"")!=0){
            return response;
        }
    }

    /* TODO stocker la pile des paramètres dans la pile */
    getParametersValues(fct->parameters);

    return fct->name;
}

/* Init line of fct type */

AllCalcFct noFctinCalc(){
    int size = 4;
    AllCalcFct initPgrm = malloc(sizeof(AllCalcFct)); 
    initPgrm->length = size;
    initPgrm->lastElement = 0;
    initPgrm->line = malloc(size*sizeof(FctRegister));
    return initPgrm;
}

/* Store action in AllCalcFct */

void storeFctCalc(AllCalcFct allFct, FctRegister fct){
    if(allFct->lastElement == allFct->length){
        int size = 2* allFct->length;
        FctRegister *line = allFct->line; //mettre type
        allFct->line = malloc(size*sizeof(FctRegister));  //mettre type
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

char *getCallBackInAll(AllCalcFct allFct, Data myData, CalcStorage myCalc){
    char *response;
    for(int i=0;i<allFct->lastElement; i=i+1){
        if(!allFct->line[i]->executed){
            response = getFctCallBack(allFct->line[i], myData, myCalc);
            if(strcmp(response, "")!=0){
                return response;
            }
        }
    }
    return "";
}

void freeAllCalcFct(AllCalcFct allFct){
    for(int i=0; i<allFct->lastElement; i=i+1){
        freeFctRegistered(allFct->line[i]);
    }
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

char *getCalcCallBack(Calcul myCalc, Data myData, CalcStorage myCalcStorage){
    return getCallBackInAll(myCalc->fct, myData, myCalcStorage);
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

int main(){
    Data myData = newData();
    storeVar(myData, newVar("myInt", "int", 15));
    CalcStorage calculs = newCalcStorage();
    Calcul myCalcul1 = OpeCalc(3, VarCalc("myInt"), ConstCalc(2));
    storeCalcul(calculs, myCalcul1); /* calc[0] = myInt - 2 */
    Calcul myCalcul2 = OpeCalc(2, ConstCalc(4), ConstCalc(2));
    storeCalcul(calculs, myCalcul2); /* cal[1] = 4+2 */ 
    Calcul myCalcul3 = OpeCalc(0, ConstCalc(3), FctCalc("fct1", addParameter(0,NULL)));/* fct1(calc[0], calc[1]) */
    storeCalcul(calculs, myCalcul3); /* calc[2] = 4 * fct1 */
    Calcul myCalcul4 = OpeCalc(0, ConstCalc(3), FctCalc("fct2", addParameter(1,NULL)));/* fct1(calc[0], calc[1]) */
    printf("should be 3 : %d\n",storeCalcul(calculs, myCalcul4)); /* calc[3] = fct2 */
    Calcul myCalcul5 = OpeCalc(2, FctCalc("fct3", addParameter(2, NULL)), FctCalc("fct4", addParameter(3, NULL)));
    storeCalcul(calculs, myCalcul5);

    char *response;
    int i = 0;

    response = getCalcCallBack(getCalc(calculs,4),myData, calculs);
    while(strcmp(response, "")!=0){
        printf("CallBack the function : %s\n", response);
        storeVar(myData, newVar("return", "int", i));
        i = i+1;
        response = getCalcCallBack(getCalc(calculs,4),myData, calculs);
    }
    
    printf("Calcul %d\n", runCalcul(getCalc(calculs, 4), myData));

    /*getVar(myData, "myInt")->value = 2;

    printf("\nSecond calcul \n\n");

    response = getCalcCallBack(getCalc(calculs,4),myData, calculs);
    while(strcmp(response, "")!=0){
        printf("CallBack the function : %s\n", response);
        storeVar(myData, newVar("return", "int", 4));
        response = getCalcCallBack(getCalc(calculs,3),myData, calculs);
    }
    
    printf("Calcul %d\n", runCalcul(getCalc(calculs, 4), myData));*/

    freeCalcStorage(calculs);
    freeData(myData);
}