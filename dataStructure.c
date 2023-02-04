#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

/* --- Variable structure --- */

typedef struct VariableStruct
{
    char *name;
    char *type;
    int value;
}*Variable;

/* Initiate a variable */

Variable newVar(char *name, char *type, int value){
    Variable var = malloc(sizeof(Variable));
    char *myName = malloc(strlen(name)*sizeof(char));
    char *myType = malloc(strlen(type)*sizeof(char));
    strcpy(myName, name);
    strcpy(myType, type);
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

typedef struct AllVariables
{
    struct VariableStruct *var;
    struct AllVariables *next;
}*Data;

/* Intitiat Data type */

Data newData(){
    Data initVar = malloc(sizeof(Data));
    initVar->var = NULL;
    initVar->next = NULL;
    return initVar;
}

/* Check if any variables exist in Data */

bool isEmpty(Data variables){
    return variables->next==NULL;
}

/* Check if a specific variable exist */

bool isVarExist(Data variables, char *name){
    if(isEmpty(variables)){
        printf("Variable %s doesn't exist\n", name);
        return false;
    }
    else{
        if(strcmp(variables->var->name,name)==0){
            return true;
        }else{
            return isVarExist(variables->next,name);
        }
    }
}

/* Return the specific variable */

Variable getVar(Data variables, char *name){
    if(strcmp(variables->var->name,name)==0){
        return variables->var;
    }else{
        return getVar(variables->next,name);
    }
}

/* Delete the specific variable */

Data deleteVar(Data variables, char *name){
    if(strcmp(variables->var->name,name)==0){
        Data next=variables->next;
        freeVar(variables->var);
        free(variables);
        return next;
    }else{
        variables->next = deleteVar(variables->next,name);
        return variables;
    }
}

/* Add new variable */

Data storeVar(Data variables, Variable var){
    Data newDict = malloc(sizeof(Data));
    newDict->var = var;
    newDict->next = variables;
    return newDict;
}

/* Delete the entire data */

void freeData(Data variables){
    if(!isEmpty(variables)){
        freeData(variables->next);
        free(variables->var->name);
        free(variables->var->type);
        free(variables->var);
        free(variables);
    }
}

int main()
{
    Data dictionnaire = newData();

    dictionnaire = storeVar(dictionnaire, newVar("number", "int", 3));
    dictionnaire = storeVar(dictionnaire, newVar("nb2", "bool", 1));
    bool varExist = isVarExist(dictionnaire, "number");
    printf("The value exist(1) or not(0) : %d\n", varExist);
    Variable var = getVar(dictionnaire, "number");
    printf("The stored value is %d \n", var->value);
    var->value = 4;
    printf("The new value is %d\n", getVar(dictionnaire, "number")->value);

    dictionnaire = deleteVar(dictionnaire, "number");
    printf("The value exist(1) or not(0) : %d\n", isVarExist(dictionnaire, "number"));


    freeData(dictionnaire);
}