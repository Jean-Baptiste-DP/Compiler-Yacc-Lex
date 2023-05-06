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
    VarInfo myVar = malloc(sizeof(struct varInfo));
    myVar->type = myType;
    myVar->name = myName;
    return myVar;
}

void freeVarInfo(VarInfo var){
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
    Variable var = malloc(sizeof(struct VariableStruct));

    char *myType = malloc((strlen(type)+1)*sizeof(char));
    strcpy(myType, type);
    char *myName = malloc((strlen(name)+1)*sizeof(char));
    strcpy(myName, name);
    var->type = myType;
    var->name = myName;

    var->intValue = value;
    return var;
}

Variable newVarFloat(char *name, char *type, float value){
    Variable var = malloc(sizeof(struct VariableStruct));

    char *myType = malloc((strlen(type)+1)*sizeof(char));
    strcpy(myType, type);
    char *myName = malloc((strlen(name)+1)*sizeof(char));
    strcpy(myName, name);
    var->type = myType;
    var->name = myName;
    
    var->floatValue = value;
    return var;
}

Variable newVar(char *name, char *type){
    Variable var = malloc(sizeof(struct VariableStruct));
    
    char *myType = malloc((strlen(type)+1)*sizeof(char));
    strcpy(myType, type);
    char *myName = malloc((strlen(name)+1)*sizeof(char));
    strcpy(myName, name);
    var->type = myType;
    var->name = myName;
    
    return var;
}

Variable duplicateVar(Variable var){
    if(strcmp(var->type, "int")==0){
        return newVarInt(var->name, var->type, var->intValue);
    }else if(strcmp(var->type, "float")==0){
        return newVarFloat(var->name, var->type, var->floatValue);
    }
}

/* Delete a variable */

void freeVar(Variable var){
    free(var->type);
    free(var->name);
    free(var);
}

void changeName(Variable var, char *name, char *type){
    char *myName = malloc((strlen(name)+1)*sizeof(char));
    char *myType = malloc((strlen(type)+1)*sizeof(char));
    strcpy(myName, name);
    strcpy(myType, type);
    free(var->name);
    free(var->type);
    var->name = myName;
    var->type = myType;
}


/* --- Variable storage sturcture --- */

/* Intitiat Data type */

DataStack newDataStack(){
    DataStack initVar = malloc(sizeof(struct AllVariables));
    initVar->var = NULL;
    initVar->next = NULL;
    return initVar;
}

Data newData(){
    Data initData = malloc(sizeof(struct data));
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
        printf("Type %s, name %s, value %d\n",variables->var->type, variables->var->name, variables->var->intValue);
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

Variable copyVarStack(DataStack variables, char *name){
    if(isEmptyStack(variables)){
        printf("Variable %s doesn't exist\n", name);
        return newVarInt("", "int", 0);
    }else{
        if(strcmp(variables->var->name,name)==0){
            if(strcmp(variables->var->type, "int")==0){
                return newVarInt(variables->var->name, variables->var->type, variables->var->intValue);
            }else if(strcmp(variables->var->type, "float")==0){
                return newVarFloat(variables->var->name, variables->var->type, variables->var->floatValue);
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

DataStack removeVarStack(DataStack variables, char *name){
    if(!isEmptyStack(variables)){
        if(strcmp(variables->var->name,name)==0){
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
    DataStack newDict = malloc(sizeof(struct AllVariables));
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
        freeVar(variables->var);
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