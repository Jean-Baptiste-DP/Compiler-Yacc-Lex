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

Variable runCalculNb(CalculNb myCalc, AllCalcFct fct, Data myData){
    if(myCalc->symbole->type==0){
        return newVar("","int", myCalc->symbole->value);
    }else if (myCalc->symbole->type==1) /* variable */
    {
        if(isVarExist(myData, myCalc->symbole->variable)){
            return copyVar(myData, myCalc->symbole->variable);
        }else{
            return newVar("", "int", 0);
        }
    }else if(myCalc->symbole->type==2){ /* fct type */
        FctRegister myFct = fct->line[myCalc->symbole->value];
        return lastValue(myFct->stacks->values);
    }

    if(myCalc->symbole->value==5 || myCalc->symbole->value==6){
        Variable child = runCalculNb(myCalc->leftChild, fct, myData);
        
        if(myCalc->symbole->value==5){
            child->intValue = - child->intValue;
        }else{
            child->intValue = ! child->intValue;
        }

        return child;
    }

    Variable left = runCalculNb(myCalc->leftChild, fct, myData);
    Variable right = runCalculNb(myCalc->rightChild, fct, myData);

    if(myCalc->symbole->value==0){
        left->intValue = left->intValue*right->intValue;
    }else if(myCalc->symbole->value==1){
        left->intValue = left->intValue/right->intValue;
    }else if(myCalc->symbole->value==2){
        left->intValue = left->intValue+right->intValue;
    }else if(myCalc->symbole->value==3){
        left->intValue = left->intValue-right->intValue;
    }else if(myCalc->symbole->value==4){
        left->intValue = left->intValue%right->intValue;
    }else if(myCalc->symbole->value==7){
        left->intValue = left->intValue||right->intValue;
    }else if(myCalc->symbole->value==8){
        left->intValue = left->intValue&&right->intValue;
    }else if(myCalc->symbole->value==9){
        left->intValue = left->intValue==right->intValue;
    }else if(myCalc->symbole->value==10){
        left->intValue = left->intValue!=right->intValue;
    }else if(myCalc->symbole->value==11){
        left->intValue = left->intValue>=right->intValue;
    }else if(myCalc->symbole->value==12){
        left->intValue = left->intValue<=right->intValue;
    }else if(myCalc->symbole->value==13){
        left->intValue = left->intValue>right->intValue;
    }else if(myCalc->symbole->value==14){
        left->intValue = left->intValue<right->intValue;
    }else{
        printf("Calc don't match possibilities");
        freeVar(left);
        freeVar(right);
        return newVar("", "int", 0);
    }
    
    freeVar(right);
    return left;
}

void incrementFctIndex(CalculNb tree, int num){
    if(tree && tree->symbole->type == 2){
        tree->symbole->value = tree->symbole->value + num;
    }else if(tree && tree->symbole->type == 3){
        incrementFctIndex(tree->leftChild, num);
        incrementFctIndex(tree->rightChild, num);
    }
}