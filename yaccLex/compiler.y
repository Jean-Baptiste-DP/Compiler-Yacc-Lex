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
int yylex_destroy();

Program myPrgm = NULL;
%}

%union {
    int num; 
    int boolean;
    struct calcul *calc;
    char *varName;
    struct calcParameters *param;
    float floatVal;
}

%start YACCline
%token <num> YACCNUMBER
%token <floatVal> YACCFLOAT
%token <boolean> YACCTRUE YACCFALSE

%token YACCprint
%nonassoc YACCIF
%nonassoc YACCELSE
%nonassoc YACCFOR
%nonassoc YACCWHILE
%nonassoc YACCFUNCT
%nonassoc YACCRETURN
%nonassoc YACCASSIGN
%token <varName> YACCVARNAME
%left YACCAND
%left YACCOR /*The order of the token show the priority of computation NOT in priority in front of AND and OR*/
%left YACCNOT
%token YACCEQ YACCNEQ YACCGE YACCLE YACCGT YACCLT
%left '+' '-'
%left '*' '/' '%'
%nonassoc YACCUMINUS
%left '(' ')' '{' '}'
%left '&' '|'
%left '!'


/* Rule Section */

%%

YACCline :              {;}
| YACCline YACCaction   {;}
;

YACCaction : YACCprint '('YACCCalcul')' ';'         {storeAction(myPrgm, (Action){ .actionType = PRINT});}
| YACCIF YACCCondition                              {storeAction(myPrgm, (Action){ .actionType = IF});} YACCendif
| YACCforLoop                                       {;}
| YACCwhileLoop                                     {;}
| YACCfunction                                      {;}
| YACCRETURN YACCCalcul ';'                         {storeAction(myPrgm, (Action){ .actionType = ASSIGNMENT});}
| YACCVARNAME YACCVARNAME YACCASSIGN YACCCalcul ';' {storeAction(myPrgm, (Action){ .actionType = ASSIGNMENT}); free($2);free($1);}
| YACCVARNAME YACCVARNAME ';'                       {storeAction(myPrgm, (Action){ .actionType = ASSIGNMENT});free($2);free($1);}
| YACCVARNAME YACCASSIGN YACCCalcul ';'             {storeAction(myPrgm, (Action){ .actionType = ASSIGNMENT}); free($1);}
| '{' YACCline '}'                                  {;}
;

YACCCalcul : YACCExpression     {;}
| YACCCondition                 {;}

YACCExpression: YACCExpression'*'YACCExpression     {storeAction(myPrgm, (Action){ .actionType = CALCUL, .var = { .dataType = OPERATOR, .operator = FOIS } });}
|YACCExpression'/'YACCExpression                    {storeAction(myPrgm, (Action){ .actionType = CALCUL, .var = { .dataType = OPERATOR, .operator = DIVISE } });}
|YACCExpression'+'YACCExpression                    {storeAction(myPrgm, (Action){ .actionType = CALCUL, .var = { .dataType = OPERATOR, .operator = PLUS } });}
|YACCExpression'-'YACCExpression                    {storeAction(myPrgm, (Action){ .actionType = CALCUL, .var = { .dataType = OPERATOR, .operator = MOINS } });}
|YACCExpression'%'YACCExpression                    {storeAction(myPrgm, (Action){ .actionType = CALCUL, .var = { .dataType = OPERATOR, .operator = MODULO } });}
|'('YACCExpression')'                               {;}
|'-' YACCExpression %prec YACCUMINUS                {storeAction(myPrgm, (Action){ .actionType = CALCUL, .var = { .dataType = OPERATOR, .operator = UMOINS } });}
| YACCNUMBER                                        {storeAction(myPrgm, (Action){ .actionType = STACK_VALUE, .var = { .dataType = INTEGER, .integer = $1 } });}
| YACCFLOAT                                         {storeAction(myPrgm, (Action){ .actionType = STACK_VALUE, .var = { .dataType = FLOAT, .floattant = $1 } });}
| YACCVARNAME '(' YACCcallParameter ')'             {free($1);}
| YACCVARNAME                                       {free($1);}
;

YACCCondition : YACCNOT YACCCondition       {storeAction(myPrgm, (Action){ .actionType = CALCUL, .var = { .dataType = OPERATOR, .operator = NOT } });}
|YACCCondition YACCAND YACCCondition        {storeAction(myPrgm, (Action){ .actionType = CALCUL, .var = { .dataType = OPERATOR, .operator = AND } });}
|YACCCondition YACCOR YACCCondition         {storeAction(myPrgm, (Action){ .actionType = CALCUL, .var = { .dataType = OPERATOR, .operator = OR } });}
| '('YACCCondition')'                       {;}
| YACCTRUE                                  {storeAction(myPrgm, (Action){ .actionType = STACK_VALUE, .var = { .dataType = BOOLEAN, .boolean = 1 } });}
| YACCFALSE                                 {storeAction(myPrgm, (Action){ .actionType = STACK_VALUE, .var = { .dataType = BOOLEAN, .boolean = 0 } });}
|YACCExpression YACCEQ YACCExpression       {storeAction(myPrgm, (Action){ .actionType = CALCUL, .var = { .dataType = OPERATOR, .operator = EQ } });}
|YACCExpression YACCNEQ YACCExpression      {storeAction(myPrgm, (Action){ .actionType = CALCUL, .var = { .dataType = OPERATOR, .operator = NEQ } });}
|YACCExpression YACCGE YACCExpression       {storeAction(myPrgm, (Action){ .actionType = CALCUL, .var = { .dataType = OPERATOR, .operator = GE } });}
|YACCExpression YACCLE YACCExpression       {storeAction(myPrgm, (Action){ .actionType = CALCUL, .var = { .dataType = OPERATOR, .operator = LE } });}
|YACCExpression YACCGT YACCExpression       {storeAction(myPrgm, (Action){ .actionType = CALCUL, .var = { .dataType = OPERATOR, .operator = GT } });}
|YACCExpression YACCLT YACCExpression       {storeAction(myPrgm, (Action){ .actionType = CALCUL, .var = { .dataType = OPERATOR, .operator = LT } });}
;

YACCendif : '{' YACCline '}' YACCELSE           {;} 
YACCaction                                      {;}
| YACCaction                                    {;}
;

YACCforLoop : YACCFOR '(' YACCVARNAME YACCVARNAME YACCASSIGN YACCCalcul ';' {;}
YACCCondition ';'                                                           {;}
YACCVARNAME YACCASSIGN YACCCalcul ')' '{' YACCline '}'                      {free($3);free($4);free($12);}
;

YACCwhileLoop : YACCWHILE YACCCondition     {;}
'{' YACCline '}'                            {;}
;

YACCcallParameter :                     {;}
| YACCCalcul ',' YACCcallParameter      {;}
| YACCCalcul                            {;}
;

YACCdefparameters :                                                     {;}
| YACCdefparameters ',' YACCVARNAME YACCVARNAME YACCASSIGN YACCCalcul   {free($3);free($4);}
| YACCdefparameters ',' YACCVARNAME YACCVARNAME                         {free($4);free($3);}
| YACCVARNAME YACCVARNAME YACCASSIGN YACCCalcul                         {free($2);free($1);}
| YACCVARNAME YACCVARNAME                                               {free($2);free($1);}
;

YACCfunction : YACCFUNCT YACCVARNAME                    {free($2);}
'(' YACCdefparameters ')' '{' YACCline '}'              {;}
;

%%

void yyerror(char *error)
{
    printf("\nEntered arithmetic expression is Invalid\n");
    printf("Error : %s\n\n", error);
    flag=1;
}

int main(){
    myPrgm = newPrgm();
    yyparse();
    /* displayPrgm(myPrgm); */
    runProgram(myPrgm);
    freeProgram(myPrgm);
    yylex_destroy(); // to avoid memory leaks https://stackoverflow.com/questions/40227135/why-yacc-have-memory-leak-at-exit
}