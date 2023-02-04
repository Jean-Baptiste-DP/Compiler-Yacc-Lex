%{
/* fichier dans lequel est defini la macro constante NOMBRE */
#include "calc.tab.h"
%}

%%

"True"          {return TRUE;}
"False"         {return FALSE;}
"&&"            {return AND;}
"||"            {return OR;}
"!"             {return NOT;}
"=="            {return EQ;}
"!="            {return NEQ;}
">="            {return GEQ;}
"<="            {return LEQ;}
">"             {return GNEQ;}
"<"             {return LNEQ;}
"if"            {return IF;}
"else"          {return ELSE;}
"print"		    {return print;}
"exit"		    {return exit_command;}
[0-9]+          {yylval.num = atoi(yytext); return NUMBER;}
[ \t]           ; /* ignore les blancs et tabulations */
\n              ;
.               return yytext[0];


%%

int yywrap (void) {return 1;}