%{
/* Definition section */
#include <stdbool.h>
#include "prgmStructure.h"
#include <stdio.h>     /* C declarations used in actions */
#include <stdlib.h>
#include <ctype.h>
int flag=0;
void yyerror();
int yylex();

/* Data variables = newData(); 
struct AllVariables allVar = {.var = NULL, .next = NULL};
struct data dat = {.myData = &allVar};
Data variables = &dat;*/

/* Stack myStack = newStack(); 
struct stack stack = {.stack = NULL};
Stack myStack = &stack;*/

/* Program myPrgm = newPrgm(); 
Action listePrgm[4];
struct prgmLine prgm = {.length = 4, .lastElement = 0, .line = listePrgm};
Program myPrgm = &prgm;*/

/* CalcStorage myCalc = newCalcStorage(); 
Calcul listeCalc[4];
struct calcLine calc = {.length = 4, .lastElement = 0, .line = listeCalc};
CalcStorage myCalc = &calc;*/

Data variables = NULL;
Stack myStack = NULL;
Program myPrgm = NULL;
CalcStorage myCalc = NULL;
%}

%union {
    int num; 
    int boolean;
    struct calcul *calc;
}

%start line
%token <num> NUMBER
%token <boolean> TRUE FALSE
%type <calc> Condition
%type <calc> Expression
%token print
%nonassoc IF
%nonassoc ELSE
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

line : action ';' {;}
| line action ';' {;}
;

action : print '('Expression')'  {storeAction(myPrgm,newAction(1,"",0,storeCalcul(myCalc, $3)));}
| print'(' Condition ')'        {storeAction(myPrgm,newAction(1,"",0,storeCalcul(myCalc, $3)));}
| ifFunction                    {;}
| '{' line '}'                  {;}
;

Expression:Expression'*'Expression  {$$=OpeCalc(0,$1,$3);}
|Expression'/'Expression            {$$=OpeCalc(1,$1,$3);}
|Expression'+'Expression            {$$=OpeCalc(2,$1,$3);}
|Expression'-'Expression            {$$=OpeCalc(3,$1,$3);}
|Expression'%'Expression            {$$=OpeCalc(4,$1,$3);}
|'('Expression')'                   {$$=$2;}
|'-' Expression %prec UMINUS        {$$=OpeCalc(5,$2,newCalc(NULL, noFctinCalc()));}
| NUMBER                            {$$=ConstCalc($1);}
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


ifFunction : IF Condition action ELSE action    {if($2){;}else{;};}
| IF Condition action                           {if($2){;};}
;
%%

void yyerror()
{
    printf("\nEntered arithmetic expression is Invalid\n\n");
    flag=1;
}

int main(){
    variables = newData();
    myStack = newStack();
    myPrgm = newPrgm();
    myCalc = newCalcStorage();
    yyparse();
    runProgram(myPrgm, myCalc, variables, myStack);
    freeProgram(myPrgm);
    freeCalcStorage(myCalc);
    freeData(variables);
    freeStack(myStack);
}