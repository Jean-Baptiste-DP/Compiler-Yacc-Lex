%{
/* Definition section */
#include <stdbool.h>
#include "./src/prgmStructure.h"
#include <stdio.h>     /* C declarations used in actions */
#include <stdlib.h>
#include <ctype.h>
int flag=0;
void yyerror();
int yylex();
extern FILE* yyin;

Data variables = NULL;
Stack myStack = NULL;
Data temporaryStorage = NULL;
Program myPrgm = NULL;
CalcStorage myCalc = NULL;
%}

%union {
    int num; 
    int boolean;
    struct calcul *calc;
    char *varName;
    struct calcParameters *param;
    float floatVal;
}

%start line
%token <num> NUMBER
%token <floatVal> FLOAT
%token <boolean> TRUE FALSE

%type <calc> Condition
%type <calc> Expression
%type <calc> Calcul
%type <param> callParameter
%token print
%nonassoc IF
%nonassoc ELSE
%nonassoc FOR
%nonassoc WHILE
%nonassoc FUNCT
%nonassoc RETURN
%nonassoc ASSIGN
%token <varName> VARNAME
%left AND
%left OR /*The order of the token show the priority of computation NOT in priority in front of AND and OR*/
%left NOT
%token EQ NEQ GE LE GT LT
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS
%left '(' ')' '{' '}'
%left '&' '|'
%left '!'


/* Rule Section */

%%

line :          {;}
| line action   {;}
;

action : print '('Calcul')' ';'     {storeAction(myPrgm,newAction(2,"",0,storeCalcul(myCalc, $3), ""));}
| IF Condition                      {storeAction(myPrgm,newAction(3,"",0,storeCalcul(myCalc, $2), "")); gotoFrom(myStack, myPrgm);} endif
| forLoop                           {;}
| whileLoop                         {;}
| function                          {;}
| RETURN Calcul ';'                 {storeAction(myPrgm,newAction(5, "", 0, storeCalcul(myCalc, $2), ""));}
| VARNAME VARNAME ASSIGN Calcul ';' {storeAction(myPrgm,newAction(1, $2, 0, storeCalcul(myCalc, $4), $1)); free($2);free($1);}
| VARNAME VARNAME ';'               {storeAction(myPrgm,newAction(1, $2, 0, -1, $1));free($2);free($1);}
| VARNAME ASSIGN Calcul ';'         {storeAction(myPrgm,newAction(0, $1, 0, storeCalcul(myCalc, $3), "")); free($1);}
| '{' line '}'                      {;}
;

Calcul : Expression     {$$ = $1;}
| Condition             {$$ = $1;}

Expression:Expression'*'Expression  {$$ = FctCalc("__mul__",newParameter($1, newParameter($3, NULL)), 1);}
|Expression'/'Expression            {$$ = FctCalc("__div__",newParameter($1, newParameter($3, NULL)), 1);}
|Expression'+'Expression            {$$ = FctCalc("__add__",newParameter($1, newParameter($3, NULL)), 1);}
|Expression'-'Expression            {$$ = FctCalc("__sub__",newParameter($1, newParameter($3, NULL)), 1);}
|Expression'%'Expression            {$$ = FctCalc("__mod__",newParameter($1, newParameter($3, NULL)), 1);}
|'('Expression')'                   {$$ = $2;}
|'-' Expression %prec UMINUS        {$$ = FctCalc("__sub__",newParameter($2, NULL), 1);}
| NUMBER                            {$$ = ConstCalcInt($1);}
| FLOAT                             {$$ = ConstCalcFloat($1);}
| VARNAME '(' callParameter ')'     {$$ = FctCalc($1, $3, 0); free($1);}
| VARNAME                           {$$ = VarCalc($1);free($1);}
;

Condition : NOT Condition               {$$ = FctCalc("__not__",newParameter($2, NULL), 1);}
| Condition AND Condition               {$$ = FctCalc("__and__",newParameter($1, newParameter($3, NULL)), 1);}
| Condition OR Condition                {$$ = FctCalc("__or__",newParameter($1, newParameter($3, NULL)), 1);}
| '('Condition')'                       {$$ = $2;}
| TRUE                                  {$$ = ConstCalcInt(true);}
| FALSE                                 {$$ = ConstCalcInt(false);}
| Expression EQ Expression              {$$ = FctCalc("__eq__",newParameter($1, newParameter($3, NULL)), 1);}
| Expression NEQ Expression             {$$ = FctCalc("__neq__",newParameter($1, newParameter($3, NULL)), 1);}
| Expression GE Expression              {$$ = FctCalc("__ge__",newParameter($1, newParameter($3, NULL)), 1);}
| Expression LE Expression              {$$ = FctCalc("__le__",newParameter($1, newParameter($3, NULL)), 1);}
| Expression GT Expression              {$$ = FctCalc("__gt__",newParameter($1, newParameter($3, NULL)), 1);}
| Expression LT Expression              {$$ = FctCalc("__lt__",newParameter($1, newParameter($3, NULL)), 1);}
;

endif : '{' line '}' ELSE           {gotoDest(myStack, myPrgm, 1);gotoFrom(myStack, myPrgm);} 
action                              {gotoDest(myStack, myPrgm, 0);}
| action                            {gotoDest(myStack, myPrgm, 0);}
;

forLoop : FOR '(' VARNAME VARNAME ASSIGN Calcul ';' {storeAction(myPrgm,newAction(1, $4, 0, storeCalcul(myCalc, $6), $3));}
Condition ';'                                       {storeAction(myPrgm,newAction(3,"",0,storeCalcul(myCalc, $9), ""));gotoFrom(myStack, myPrgm);}
VARNAME ASSIGN Calcul ')' '{' line '}'              {storeAction(myPrgm,newAction(0, $12, 0, storeCalcul(myCalc, $14), ""));forEndGoto(myStack, myPrgm, $4);free($3);free($4);free($12);}
;

whileLoop : WHILE Condition     {storeAction(myPrgm,newAction(3,"",0,storeCalcul(myCalc, $2), ""));gotoFrom(myStack, myPrgm);}
'{' line '}'                    {whileEndGoto(myStack, myPrgm);}
;

callParameter :                 {$$ = NULL;}
| Calcul ',' callParameter      {$$ = newParameter($1, $3);}
| Calcul                        {$$ = newParameter($1, NULL);}
;

defparameters :                                 {;}
| defparameters ',' VARNAME VARNAME ASSIGN Calcul   {storeAction(myPrgm,newAction(1, $4, 0, storeCalcul(myCalc, $6), $3));free($3);free($4);}
| defparameters ',' VARNAME VARNAME                 {storeAction(myPrgm,newAction(1, $4, 0, -1, $3));free($4);free($3);}
| VARNAME VARNAME ASSIGN Calcul                     {storeAction(myPrgm,newAction(1, $2, 0, storeCalcul(myCalc, $4), $1)); free($2);free($1);}
| VARNAME VARNAME                                   {storeAction(myPrgm,newAction(1, $2, 0, -1, $1));free($2);free($1);}
;

function : FUNCT VARNAME                   {storeVar(variables, newVarInt($2, "function", gotoFrom(myStack, myPrgm)+1)); free($2);}
'(' defparameters ')' '{' line '}'         {storeAction(myPrgm,newAction(5, "", 0, -1, ""));gotoDest(myStack, myPrgm, 0);}
;

%%

void yyerror(char *error)
{
    printf("\nEntered arithmetic expression is Invalid\n");
    printf("Error : %s\n\n", error);
    flag=1;
}

int main(int argc, char** argv){

    if(argc==2){
        yyin = fopen(argv[1], "r");
        if(!yyin){
            fprintf(stderr, "Impossible to open the file : %s\n", argv[1]);
            return 1;
        }
    }

    variables = newData();
    myStack = newStack();
    temporaryStorage = newData();
    myPrgm = newPrgm();
    myCalc = newCalcStorage();
    yyparse();
    /* displayPrgm(myPrgm); */
    runProgram(myPrgm, myCalc, variables, temporaryStorage);
    freeCalcStorage(myCalc);
    freeProgram(myPrgm);
    freeData(variables);
    freeData(temporaryStorage);
    freeStack(myStack);
}