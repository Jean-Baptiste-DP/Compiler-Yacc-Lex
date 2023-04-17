#define DATA

#ifndef __cplusplus
#ifndef bool
#include <stdbool.h>
#endif
#endif

typedef struct varInfo
{
    char *type;
    char *name;
}*VarInfo;

typedef struct VariableStruct
{
    char *name;
    char *type;
    int intValue;
    float floatValue;
}*Variable;

/* 
type = 
"int" -> integer
"float" -> float value
"context" (name = "") -> context delimitation
"return" (name = "return") -> function call return
*/

typedef struct AllVariables
{
    struct VariableStruct *var;
    struct AllVariables *next;
}*DataStack;

typedef struct data{
    DataStack myData;
}*Data;

/* --- Type --- */

VarInfo newVarInfo(char *type, char *name);
void freeVarInfo(VarInfo var);

/* --- Variables --- */

Variable newVarInt(char *name, char *type, int value);
Variable newVarFloat(char *name, char *type, float value);
Variable newVar(char *name, char *type);
void freeVar(Variable var);
void changeName(Variable var, char *name, char *type);

/* --- DataStack : Variable storage --- */

DataStack newDataStack();
bool isEmptyStack(DataStack variables);
bool isVarExistStack(DataStack variables, char *name);
bool isVarExistInContextStack(DataStack variables, char *name);
Variable getVarStack(DataStack variables, char *name);
Variable copyVarStack(DataStack variables, char *name);
DataStack deleteVarStack(DataStack variables, char *name);
DataStack removeVarStack(DataStack variables, char *name);
DataStack storeVarStack(DataStack variables, Variable var);
void freeDataStack(DataStack variables);
DataStack freeContextStack(DataStack variables);
DataStack freeOneInStack(DataStack variables);
void printAllVariables(DataStack variables);

/* --- Data Storage --- */

Data newData();
bool isEmpty(Data variables);
bool isVarExist(Data variables, char *name);
bool isVarExistInContext(Data variables, char *name);
Variable getVar(Data variables, char *name);
Variable copyVar(Data variables, char *name);
void deleteVar(Data variables, char *name);
void removeVar(Data variables, char *name);
void storeVar(Data variables, Variable var);
void freeData(Data variables);
int freeContext(Data variables);
Variable lastValue(Data variables);
void removeData(Data variables);