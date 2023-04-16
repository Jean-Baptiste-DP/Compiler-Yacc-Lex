#include <stdio.h>
#include <stdlib.h>
#include "calculs.h"

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

char *getCalcCallBack(Calcul myCalc, Data myData, CalcStorage myCalcStorage, Data myStack){
    return getCallBackInAll(myCalc->fct, myData, myCalcStorage, myStack);
}

Variable runCalcul(Calcul myCalc, Data myData){
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