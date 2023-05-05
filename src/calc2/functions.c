#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "functions.h"
#include "integer.h"
#include "float.h"

int runFunction(char *name, Data myStack, Data myData, int currentLine){
    char *delim = "/";
    char *functionName = malloc(sizeof(char)*strlen(name));
    strcpy(functionName, name);
    char* type = strtok(functionName, delim);
    char* methode = strtok(NULL, delim);
    int nextLine;
    if(strcmp(type, "int")==0){
        nextLine = runFunctionInt(methode, myStack, myData);
    }else if(strcmp(type, "float")==0){
        nextLine = runFunctionFloat(methode, myStack, myData);
    }else if(strcmp(type, " ")==0){
        nextLine = runFunctionEmpty(methode, myData, currentLine);
    }else{
        printf("Type %s is not defined\n", type);
        nextLine = currentLine+1;
    }
    free(name);
    return nextLine;
}

int runFunctionEmpty(char *functionName, Data myData, int currentLine){
    if(isVarExist(myData, functionName) && strcmp(getVar(myData, functionName)->type, "function")==0){
        storeVar(myData, newVarInt("", "context", currentLine));
        return getVar(myData, functionName)->intValue;
    }else{
        printf("The function %s doesn't exist\n", functionName);
        return currentLine+1;
    }
}