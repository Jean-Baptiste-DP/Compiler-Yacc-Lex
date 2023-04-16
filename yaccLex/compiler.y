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
    struct fctParameters *param;
}

%start line
%token <num> NUMBER
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
%nonassoc LET
%nonassoc ASSIGN
%token <varName> VARNAME
%left AND
%left OR /*The order of the token show the priority of computation NOT in priority in front of AND and OR*/
%left NOT
%token EQ NEQ GEQ LEQ GNEQ LNEQ
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

action : print '('Calcul')' ';' {storeAction(myPrgm,newAction(2,"",0,storeCalcul(myCalc, $3)));}
| IF Condition                  {storeAction(myPrgm,newAction(3,"",0,storeCalcul(myCalc, $2))); gotoFrom(myStack, myPrgm);} endif
| forLoop                       {;}
| whileLoop                     {;}
| function                      {;}
| RETURN Calcul ';'             {storeAction(myPrgm,newAction(5, "", 0, storeCalcul(myCalc, $2)));}
| LET VARNAME ASSIGN Calcul ';' {storeAction(myPrgm,newAction(1, $2, 0, storeCalcul(myCalc, $4))); free($2);}
| LET VARNAME ';'               {storeAction(myPrgm,newAction(1, $2, 0, -1));free($2);}
| VARNAME ASSIGN Calcul ';'     {storeAction(myPrgm,newAction(0, $1, 0, storeCalcul(myCalc, $3))); free($1);}
| '{' line '}'                  {;}
;

Calcul : Expression     {$$=$1;}
| Condition             {$$=$1;}

Expression:Expression'*'Expression  {$$=OpeCalc(0,$1,$3);}
|Expression'/'Expression            {$$=OpeCalc(1,$1,$3);}
|Expression'+'Expression            {$$=OpeCalc(2,$1,$3);}
|Expression'-'Expression            {$$=OpeCalc(3,$1,$3);}
|Expression'%'Expression            {$$=OpeCalc(4,$1,$3);}
|'('Expression')'                   {$$=$2;}
|'-' Expression %prec UMINUS        {$$=OpeCalc(5,$2,newCalc(NULL, noFctinCalc()));}
| NUMBER                            {$$=ConstCalc($1);}
| VARNAME '(' callParameter ')'     {$$=FctCalc($1, $3); free($1);}
| VARNAME                           {$$=VarCalc($1);free($1);}
;

Condition : NOT Condition               {$$=OpeCalc(6,$2,newCalc(NULL, noFctinCalc()));}
| Condition OR Condition                {$$=OpeCalc(7,$1,$3);}
| Condition AND Condition               {$$=OpeCalc(8,$1,$3);}
| '('Condition')'                       {$$ = $2;}
| TRUE                                  {$$ = ConstCalc(true);}
| FALSE                                 {$$ = ConstCalc(false);}
| Expression EQ Expression              {$$ = OpeCalc(9,$1,$3);}
| Expression NEQ Expression             {$$ = OpeCalc(10,$1,$3);}
| Expression GEQ Expression             {$$ = OpeCalc(11,$1,$3);}
| Expression LEQ Expression             {$$ = OpeCalc(12,$1,$3);}
| Expression GNEQ Expression            {$$ = OpeCalc(13,$1,$3);}
| Expression LNEQ Expression            {$$ = OpeCalc(14,$1,$3);}
;

endif : '{' line '}' ELSE           {gotoDest(myStack, myPrgm, 1);gotoFrom(myStack, myPrgm);} 
action                              {gotoDest(myStack, myPrgm, 0);}
| action                            {gotoDest(myStack, myPrgm, 0);}
;

forLoop : FOR '(' LET VARNAME ASSIGN Calcul ';' {storeAction(myPrgm,newAction(1, $4, 0, storeCalcul(myCalc, $6)));}
Condition ';'                                   {storeAction(myPrgm,newAction(3,"",0,storeCalcul(myCalc, $9)));gotoFrom(myStack, myPrgm);}
VARNAME ASSIGN Calcul ')' '{' line '}'          {storeAction(myPrgm,newAction(0, $12, 0, storeCalcul(myCalc, $14)));forEndGoto(myStack, myPrgm, $4);free($4);free($12);}
;

whileLoop : WHILE Condition     {storeAction(myPrgm,newAction(3,"",0,storeCalcul(myCalc, $2)));gotoFrom(myStack, myPrgm);}
'{' line '}'                    {whileEndGoto(myStack, myPrgm);}
;

callParameter :                 {$$=NULL;}
| Calcul ',' callParameter      {$$=addParameter(storeCalcul(myCalc, $1), $3);}
| Calcul                        {$$=addParameter(storeCalcul(myCalc, $1), NULL);}
;

defparameters :                                 {;}
| defparameters ',' LET VARNAME ASSIGN Calcul   {storeAction(myPrgm,newAction(1, $4, 0, storeCalcul(myCalc, $6))); free($4);}
| defparameters ',' LET VARNAME                 {storeAction(myPrgm,newAction(1, $4, 0, -1));free($4);;}
| LET VARNAME ASSIGN Calcul                     {storeAction(myPrgm,newAction(1, $2, 0, storeCalcul(myCalc, $4))); free($2);}
| LET VARNAME                                   {storeAction(myPrgm,newAction(1, $2, 0, -1));free($2);}
;

function : FUNCT VARNAME                   {storeVar(variables, newVar($2, "function", gotoFrom(myStack, myPrgm)+1)); free($2);}
'(' defparameters ')' '{' line '}'         {storeAction(myPrgm,newAction(5, "", 0, -1));gotoDest(myStack, myPrgm, 0);}
;

%%

void yyerror(char *error)
{
    printf("\nEntered arithmetic expression is Invalid\n");
    printf("Erreur : %s\n\n", error);
    flag=1;
}

int main(){
    variables = newData();
    myStack = newStack();
    temporaryStorage = newData();
    myPrgm = newPrgm();
    myCalc = newCalcStorage();
    yyparse();
    displayPrgm(myPrgm);
    runProgram(myPrgm, myCalc, variables, temporaryStorage);
    freeProgram(myPrgm);
    freeCalcStorage(myCalc);
    freeData(variables);
    freeData(temporaryStorage);
    freeStack(myStack);
}