%{
/* Definition section */
#include <stdio.h>     /* C declarations used in actions */
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
int flag=0;
void yyerror();
int yylex();
%}


%union {
    int num; 
    int boolean;
}
%start line
%token <num> NUMBER
%token <boolean> TRUE FALSE
%type <boolean> Condition
%type <num> Expression
%token print
%token exit_command
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
%left '&' '|' '!' 


/* Rule Section */

%%

line : action ';' {;}
| line action ';' {;}
;

action : print'('Expression')'  {printf("Integer : %d\n", $3);}
| print'(' Condition ')'        {printf("Boolean : %d\n", $3);}
| exit_command                  {exit(EXIT_SUCCESS);}
| ifFunction                    {;}
| '{' line '}'                  {;}
;

Expression:Expression'*'Expression  {$$=$1*$3;}
|Expression'/'Expression            {$$=$1/$3;}
|Expression'+'Expression            {$$=$1+$3;}
|Expression'-'Expression            {$$=$1-$3;}
|Expression'%'Expression            {$$=$1%$3;}
|'('Expression')'                   {$$=$2;}
|'-' Expression %prec UMINUS        {$$=-$2;}
| NUMBER                            {$$=$1;}
;

Condition : NOT Condition               {$$= !$2;}
| Condition OR Condition                {$$=$1 || $3;}
| Condition AND Condition               {$$=$1 && $3;}
| '('Condition')'                       {$$ = $2;}
| TRUE                                  {$$ = true;}
| FALSE                                 {$$ = false;}
| Expression EQ Expression              {$$ = $1 == $3;}
| Expression NEQ Expression             {$$ = $1 != $3;}
| Expression GEQ Expression             {$$ = $1 >= $3;}
| Expression LEQ Expression             {$$ = $1 <= $3;}
| Expression GNEQ Expression            {$$ = $1 > $3;}
| Expression LNEQ Expression            {$$ = $1 < $3;}

/* ifFunction : IF Condition action ELSE action    {if($2){printf("Condition passée \n");}else{printf("Posibilité par défaut \n");};}
| IF Condition action                           {if($2){printf("Condition passée \n");};} */

ifFunction : IF Condition action ELSE action    {if($2){;}else{;};}
| IF Condition action                           {if($2){;};}

%%

void yyerror()
{
    printf("\nEntered arithmetic expression is Invalid\n\n");
    flag=1;
}