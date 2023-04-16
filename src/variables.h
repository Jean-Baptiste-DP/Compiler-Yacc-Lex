#define DATA

#ifndef __cplusplus
#ifndef bool
#include <stdbool.h>
#endif
#endif

typedef struct VariableStruct
{
    char *name;
    char *type;
    int value;
}*Variable;

/* 
type = 
"int" -> entier
"context" (name = "") -> delimitation de context
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

/* --- Variables --- */

Variable newVar(char *name, char *type, int value);
void freeVar(Variable var);
void changeName(Variable var, char *name, char *type);

/* --- DataStack : Variable storage --- */

DataStack newDataStack();
bool isEmptyStack(DataStack variables);
bool isVarExistStack(DataStack variables, char *name);
bool isVarExistInContextStack(DataStack variables, char *name);
Variable getVarStack(DataStack variables, char *name);
DataStack deleteVarStack(DataStack variables, char *name);
DataStack storeVarStack(DataStack variables, Variable var);
void freeDataStack(DataStack variables);
DataStack freeContextStack(DataStack variables);
DataStack freeOneInStack(DataStack variables);

/* --- Data Storage --- */

Data newData();
bool isEmpty(Data variables);
bool isVarExist(Data variables, char *name);
bool isVarExistInContext(Data variables, char *name);
Variable getVar(Data variables, char *name);
void deleteVar(Data variables, char *name);
void storeVar(Data variables, Variable var);
void freeData(Data variables);
int freeContext(Data variables);
Variable lastValue(Data variables);
void removeData(Data variables);