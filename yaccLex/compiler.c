#include <stdio.h>
#include "../compiler.tab.h"

extern int ylex();
extern int yylineno;
extern char* yytext;

int main(void)
{
    yyparse();
}