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
//     char *myvar = malloc(strlen(var)+1);
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

CalculNb leafConstFloat(float value){
    CalculNb leaf = malloc(sizeof(CalculNb));
    Variable sym = newVarFloat("", "float", value);
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

Variable integerCalculation(Variable var1, Variable var2, int operator){
    int num1 = var1->intValue;
    int num2 = var2->intValue;

    if(operator==0){
        var1->intValue = num1*num2;
    }else if(operator==1){
        var1->intValue = num1/num2;
    }else if(operator==2){
        var1->intValue = num1+num2;
    }else if(operator==3){
        var1->intValue = num1-num2;
    }else if(operator==4){
        var1->intValue = num1%num2;
    }else if(operator==5){
        var1->intValue = -num1;
    }else if(operator==6){
        var1->intValue = !num1;
    }else if(operator==7){
        var1->intValue = num1||num2;
    }else if(operator==8){
        var1->intValue = num1&&num2;
    }else if(operator==9){
        var1->intValue = num1==num2;
    }else if(operator==10){
        var1->intValue = num1!=num2;
    }else if(operator==11){
        var1->intValue = num1>=num2;
    }else if(operator==12){
        var1->intValue = num1<=num2;
    }else if(operator==13){
        var1->intValue = num1>num2;
    }else if(operator==14){
        var1->intValue = num1<num2;
    }

    freeVar(var2);
    return var1;
}

Variable floatCalculation(Variable var1, Variable var2, int operator){
    float num1 = var1->floatValue;
    float num2 = var2->floatValue;

    if(operator==0){
        var1->floatValue = num1*num2;
    }else if(operator==1){
        var1->floatValue = num1/num2;
    }else if(operator==2){
        var1->floatValue = num1+num2;
    }else if(operator==3){
        var1->floatValue = num1-num2;
    }else if(operator==4){
        printf("%% operator need integer values\n");
    }else if(operator==5){
        var1->floatValue = -num1;
    }

    if(operator>=6 && operator<=8){
        printf("Cannot convert float value to boolean\n");
    }

    if(operator>=9 && operator<=14){
        changeName(var1, var1->name, "int");
    }
    
    if(operator==9){
        var1->intValue = num1==num2;
    }else if(operator==10){
        var1->intValue = num1!=num2;
    }else if(operator==11){
        var1->intValue = num1>=num2;
    }else if(operator==12){
        var1->intValue = num1<=num2;
    }else if(operator==13){
        var1->intValue = num1>num2;
    }else if(operator==14){
        var1->intValue = num1<num2;
    }

    freeVar(var2);
    return var1;
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
            
            if(strcmp(child->type, "int")==0){
                return integerCalculation(child, newVar("", ""), myCalc->symbole->intValue);
            }else if(strcmp(child->type, "float")==0){
                return floatCalculation(child, newVar("", ""), myCalc->symbole->intValue);
            }
        }

        Variable left = runCalculNb(myCalc->leftChild, fct, myData);
        Variable right = runCalculNb(myCalc->rightChild, fct, myData);

        if(strcmp(left->type, "int")==0){
            return integerCalculation(left, right, myCalc->symbole->intValue);
        }else if(strcmp(left->type, "float")==0){
            return floatCalculation(left, right, myCalc->symbole->intValue);
        }

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