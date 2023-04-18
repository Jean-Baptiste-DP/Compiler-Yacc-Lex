#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef CALCULS
#include "calculs.h"
#endif

#include "tree.h"

/* --- Calcul symbole --- */

// Symbole newSymbole(int type, int val, char *var){
//     Symbole initSymb = malloc(sizeof(Symbole));
//     char *myvar = malloc(strlen(var));
//     strcpy(myvar, var);
//     initSymb->variable = myvar;
//     initSymb->value = val;
//     initSymb->type = type;
//     return initSymb;
// }

// Symbole freeSymbole(Symbole mysym){
//     free(mysym->variable);
//     free(mysym);
// }

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

CalculNb leafConstInt(int value){
    CalculNb leaf = malloc(sizeof(CalculNb));
    Variable sym = newVarInt("", "int", value);
    leaf->symbole = sym;
    leaf->leftChild=NULL;
    leaf->rightChild=NULL;
    
    return leaf;
}

CalculNb leafVar(char *varName){

    CalculNb leaf = malloc(sizeof(CalculNb));
    Variable sym = newVar(varName, "calcTreeVar");
    leaf->symbole = sym;
    leaf->leftChild=NULL;
    leaf->rightChild=NULL;

    return leaf;
}

CalculNb leafFct(int fctPosition){

    CalculNb leaf = malloc(sizeof(CalculNb));
    Variable sym = newVarInt("", "calcTreeFct", fctPosition);
    leaf->symbole=sym;
    leaf->leftChild=NULL;
    leaf->rightChild=NULL;

    return leaf;
}

/* Init node operators */

CalculNb nodeOperator(int operat, CalculNb lChild, CalculNb rChild){

    CalculNb node = malloc(sizeof(CalculNb));
    Variable sym = newVarInt("", "calcTreeCalc", operat);
    node->symbole = sym;
    node->leftChild=lChild;
    node->rightChild=rChild;

    return node;
}

/* Delete Calcul */

void freeCalculNb(CalculNb myCalc){
    if(strcmp(myCalc->symbole->type, "calcTreeCalc")==0){
        freeCalculNb(myCalc->leftChild);
        if(myCalc->symbole->intValue!=5 && myCalc->symbole->intValue!=6){
            freeCalculNb(myCalc->rightChild);
        }
        freeVar(myCalc->symbole);
        free(myCalc);
    }else
    {
        freeVar(myCalc->symbole);
        free(myCalc);
    }
}

/* Execute Calcul */

Variable runCalculNb(CalculNb myCalc, AllCalcFct fct, Data myData){
    
    if (strcmp(myCalc->symbole->type, "calcTreeVar")==0) /* variable */
    {
        if(isVarExist(myData, myCalc->symbole->name)){
            return copyVar(myData, myCalc->symbole->name);
        }else{
            printf("Variable %s doesn't exist\n", myCalc->symbole->name);
            exit(1);
        }
    }else if(strcmp(myCalc->symbole->type, "calcTreeFct")==0){ /* fct type */
        FctRegister myFct = fct->line[myCalc->symbole->intValue];
        return lastValue(myFct->stacks->values);
    }else if(strcmp(myCalc->symbole->type, "calcTreeCalc")==0){
        if(myCalc->symbole->intValue==5 || myCalc->symbole->intValue==6){
            Variable child = runCalculNb(myCalc->leftChild, fct, myData);
            
            if(myCalc->symbole->intValue==5){
                child->intValue = - child->intValue;
            }else{
                child->intValue = ! child->intValue;
            }

            return child;
        }

        /* TODO check if left type == right type */
        /* Send calculs to differents types */

        Variable left = runCalculNb(myCalc->leftChild, fct, myData);
        Variable right = runCalculNb(myCalc->rightChild, fct, myData);

        if(myCalc->symbole->intValue==0){
            left->intValue = left->intValue*right->intValue;
        }else if(myCalc->symbole->intValue==1){
            left->intValue = left->intValue/right->intValue;
        }else if(myCalc->symbole->intValue==2){
            left->intValue = left->intValue+right->intValue;
        }else if(myCalc->symbole->intValue==3){
            left->intValue = left->intValue-right->intValue;
        }else if(myCalc->symbole->intValue==4){
            left->intValue = left->intValue%right->intValue;
        }else if(myCalc->symbole->intValue==7){
            left->intValue = left->intValue||right->intValue;
        }else if(myCalc->symbole->intValue==8){
            left->intValue = left->intValue&&right->intValue;
        }else if(myCalc->symbole->intValue==9){
            left->intValue = left->intValue==right->intValue;
        }else if(myCalc->symbole->intValue==10){
            left->intValue = left->intValue!=right->intValue;
        }else if(myCalc->symbole->intValue==11){
            left->intValue = left->intValue>=right->intValue;
        }else if(myCalc->symbole->intValue==12){
            left->intValue = left->intValue<=right->intValue;
        }else if(myCalc->symbole->intValue==13){
            left->intValue = left->intValue>right->intValue;
        }else if(myCalc->symbole->intValue==14){
            left->intValue = left->intValue<right->intValue;
        }else{
            printf("Calc don't match possibilities");
            freeVar(left);
            freeVar(right);
            printf("Impossible symbole value : %d\n", myCalc->symbole->intValue);
            exit(1);
        }

        freeVar(right);
        return left;

    }else{
        return duplicateVar(myCalc->symbole);
    }
    
}

void incrementFctIndex(CalculNb tree, int num){
    if(tree && strcmp(tree->symbole->type, "calcTreeFct") == 0){
        tree->symbole->intValue = tree->symbole->intValue + num;
    }else if(tree && strcmp(tree->symbole->type, "calcTreeCalc") == 0){
        incrementFctIndex(tree->leftChild, num);
        incrementFctIndex(tree->rightChild, num);
    }
}