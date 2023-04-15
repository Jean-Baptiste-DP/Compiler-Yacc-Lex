#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef CALCULS
#include "calculs.h"
#endif

#include "tree.h"

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