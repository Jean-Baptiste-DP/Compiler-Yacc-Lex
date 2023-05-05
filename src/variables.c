#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "variables.h"

/* Type */

VarInfo newVarInfo(char *type, char *name){
    char *myType = malloc((strlen(type)+1)*sizeof(char));
    strcpy(myType, type);
    char *myName = malloc((strlen(name)+1)*sizeof(char));
    strcpy(myName, name);
    VarInfo myVar = malloc(sizeof(VarInfo));
    myVar->type = myType;
    myVar->name = myName;
    return myVar;
}

void freeVarInfo(VarInfo var){
    printf("Free Var info -%s- -%s-\n", var->type, var->name);
    free(var->type);
    free(var->name);
    free(var);
}

void changeVarInfo(VarInfo var, char *name, char *type){
    char *myName = malloc((strlen(name)+1)*sizeof(char));
    char *myType = malloc((strlen(type)+1)*sizeof(char));
    strcpy(myName, name);
    strcpy(myType, type);
    free(var->name);
    free(var->type);
    var->name = myName;
    var->type = myType;
}

/* Initiate a variable */

Variable newVarInt(char *name, char *type, int value){
    Variable var = malloc(sizeof(Variable));
    var->info = newVarInfo(type, name);
    var->intValue = value;
    return var;
}

Variable newVarFloat(char *name, char *type, float value){
    Variable var = malloc(sizeof(Variable));
    var->info = newVarInfo(type, name);
    var->floatValue = value;
    return var;
}

Variable newVar(char *name, char *type){
    Variable var = malloc(sizeof(Variable));
    var->info = newVarInfo(type, name);
    return var;
}

Variable duplicateVar(Variable var){
    if(strcmp(var->info->type, "int")==0){
        return newVarInt(var->info->name, var->info->type, var->intValue);
    }else if(strcmp(var->info->type, "float")==0){
        return newVarFloat(var->info->name, var->info->type, var->floatValue);
    }
}

/* Delete a variable */

void freeVar(Variable var){
    printf("free var %s(%s)\n", var->info->name, var->info->type);
    freeVarInfo(var->info);
    printf("step 2\n");
    free(var);
    printf("End free var\n");
}

void changeName(Variable var, char *name, char *type){
    changeVarInfo(var->info, name, type);
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
        if(strcmp(variables->var->info->name,name)==0){
            return true;
        }else{
            return isVarExistStack(variables->next,name);
        }
    }
}

bool isVarExistInContextStack(DataStack variables, char *name){
    if(isEmptyStack(variables) || strcmp(variables->var->info->type, "context")==0){
        return false;
    }
    else{
        if(strcmp(variables->var->info->name,name)==0){
            printf("Variable match with %s, type %s\n", variables->var->info->name, variables->var->info->type);
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
        printf("Type %s, name %s, value %d\n",variables->var->info->type, variables->var->info->name, variables->var->intValue);
        printAllVariables(variables->next);
    }
}

/* Return the specific variable */

Variable getVarStack(DataStack variables, char *name){
    if(isEmptyStack(variables)){
        printf("Variable %s doesn't exist\n", name);
        /* change 0 to variable, without creating memory for it */
        return 0;
    }else{
        if(strcmp(variables->var->info->name,name)==0){
            return variables->var;
        }else{
            return getVarStack(variables->next,name);
        }
    }
}

Variable getVar(Data variables, char *name){
    return getVarStack(variables->myData, name);
}

Variable copyVarStack(DataStack variables, char *name){
    if(isEmptyStack(variables)){
        printf("Variable %s doesn't exist\n", name);
        return newVarInt("", "int", 0);
    }else{
        if(strcmp(variables->var->info->name,name)==0){
            if(strcmp(variables->var->info->type, "int")==0){
                return newVarInt(variables->var->info->name, variables->var->info->type, variables->var->intValue);
            }else if(strcmp(variables->var->info->type, "float")==0){
                return newVarFloat(variables->var->info->name, variables->var->info->type, variables->var->floatValue);
            }
        }else{
            return copyVarStack(variables->next,name);
        }
    }
}

Variable copyVar(Data variables, char *name){
    return copyVarStack(variables->myData, name);
}

/* Delete the specific variable */

DataStack deleteVarStack(DataStack variables, char *name){
    if(!isEmptyStack(variables)){
        if(strcmp(variables->var->info->name,name)==0){
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

DataStack removeVarStack(DataStack variables, char *name){
    if(!isEmptyStack(variables)){
        if(strcmp(variables->var->info->name,name)==0){
            DataStack next=variables->next;
            return next;
        }else{
            variables->next = removeVarStack(variables->next,name);
            return variables;
        }
    }
}

void removeVar(Data variables, char *name){
    variables->myData = removeVarStack(variables->myData, name);
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
        freeVarInfo(variables->var->info);
        free(variables->var);
        free(variables);
    }
}

void freeData(Data variables){
    if(variables){
        freeDataStack(variables->myData);
        free(variables);
    }
}

DataStack freeContextStack(DataStack variables){
    if(!isEmptyStack(variables) && strcmp(variables->var->info->type, "context")!=0){
        free(variables->var->info->name);
        free(variables->var->info->type);
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
        free(variables->var->info->name);
        free(variables->var->info->type);
        free(variables->var);
        DataStack next = variables->next;
        free(variables);
        return next;
    }
    return variables;
}

int freeContext(Data variables){
    variables->myData = freeContextStack(variables->myData);
    int returnPosition = variables->myData->var->intValue;
    variables->myData = freeOneInStack(variables->myData);
    return returnPosition;
}

Variable lastValue(Data variables){
    Variable value = variables->myData->var;
    DataStack next = variables->myData->next;
    free(variables->myData);
    variables->myData = next;
    return value;
}

void removeData(Data variables){
    freeDataStack(variables->myData);
    variables->myData=newDataStack();
}

int countVariable(DataStack variables){
    if(isEmptyStack(variables)){
        return 0;
    }else{
        return 1+countVariable(variables->next);
    }
}

int numberVariable(Data variables){
    return countVariable(variables->myData);
}