#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "variables.h"

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

bool isVarExistInContextStack(DataStack variables, char *name){
    if(isEmptyStack(variables) || strcmp(variables->var->type, "context")==0){
        return false;
    }
    else{
        if(strcmp(variables->var->name,name)==0){
            printf("Variable match with %s, type %s\n", variables->var->name, variables->var->type);
            return true;
        }else{
            return isVarExistInContextStack(variables->next,name);
        }
    }
}

bool isVarExist(Data variables, char *name){
    return isVarExistStack(variables->myData, name);
}

bool isVarExistInContext(Data variables, char *name){
    return isVarExistInContextStack(variables->myData, name);
}

void printAllVariables(DataStack variables){
    if(!isEmptyStack(variables)){
        printf("Type %s, name %s, value %d\n",variables->var->type, variables->var->name, variables->var->value);
        printAllVariables(variables->next);
    }
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

DataStack freeContextStack(DataStack variables){
    if(!isEmptyStack(variables) && strcmp(variables->var->type, "context")!=0){
        free(variables->var->name);
        free(variables->var->type);
        free(variables->var);
        DataStack next = variables->next;
        free(variables);
        freeContextStack(next);
    }else{
        return variables;
    }
}

DataStack freeOneInStack(DataStack variables){
    if(!isEmptyStack(variables)){
        free(variables->var->name);
        free(variables->var->type);
        free(variables->var);
        DataStack next = variables->next;
        free(variables);
        return next;
    }
    return variables;
}

int freeContext(Data variables){
    variables->myData = freeContextStack(variables->myData);
    int returnPosition = variables->myData->var->value;
    variables->myData = freeOneInStack(variables->myData);
    return returnPosition;
}