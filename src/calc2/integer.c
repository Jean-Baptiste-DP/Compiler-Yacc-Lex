#include <stdio.h>
#include <string.h>
#include "integer.h"

/*
reuse of python operator name https://docs.python.org/3/reference/datamodel.html#emulating-numeric-types
__mul__ : *
__div__ : /
__sub__ : - UMINUS
__add__ : +
__mod__ : %
__not__ : !
__or__ : ||
__and__ : &&
__eq__ : ==
__neq__ : !=
__gt__ : >
__ge__ : >=
__lt__ : <
__le__ : <=
*/


int runFunctionInt(char *name, Data myStack, Data myData){
    int numberParameters = numberVariable(myStack);
    Variable returnedValue = NULL;
    if(numberParameters==1){
        Variable myVar = lastValue(myStack);
        if(strcmp(name, "__sub__")==0){
            myVar->intValue = -myVar->intValue;
        }else if(strcmp(name, "__not__")==0){
            myVar->intValue = !myVar->intValue;
        }
        returnedValue = myVar;
    }else if(numberParameters==2){

        /* TODO check var2 type */
        Variable var1 = lastValue(myStack);
        Variable var2 = lastValue(myStack);
        if(strcmp(name, "__mul__")==0){
            var1->intValue = var1->intValue * var2->intValue;
        }else if(strcmp(name, "__div__")==0){
            var1->intValue = var1->intValue / var2->intValue;
        }else if(strcmp(name, "__add__")==0){
            var1->intValue = var1->intValue + var2->intValue;
        }else if(strcmp(name, "__sub__")==0){
            var1->intValue = var1->intValue - var2->intValue;
        }else if(strcmp(name, "__mod__")==0){
            var1->intValue = var1->intValue % var2->intValue;
        }else if(strcmp(name, "__or__")==0){
            var1->intValue = var1->intValue || var2->intValue;
        }else if(strcmp(name, "__and__")==0){
            var1->intValue = var1->intValue && var2->intValue;
        }else if(strcmp(name, "__eq__")==0){
            var1->intValue = (var1->intValue == var2->intValue);
        }else if(strcmp(name, "__neq__")==0){
            var1->intValue = (var1->intValue != var2->intValue);
        }else if(strcmp(name, "__gt__")==0){
            var1->intValue = var1->intValue > var2->intValue;
        }else if(strcmp(name, "__ge__")==0){
            var1->intValue = var1->intValue >= var2->intValue;
        }else if(strcmp(name, "__lt__")==0){
            var1->intValue = var1->intValue < var2->intValue;
        }else if(strcmp(name, "__le__")==0){
            var1->intValue = var1->intValue <= var2->intValue;
        }

        returnedValue = var1;
        freeVar(var2);
    }else{
        printf("There no operator with more than 2 parameters for integer type\n");
        returnedValue = lastValue(myStack);
    }

    if(returnedValue){
        changeName(returnedValue, "return", returnedValue->type);
        storeVar(myData, returnedValue);
    }
    return -1;
}