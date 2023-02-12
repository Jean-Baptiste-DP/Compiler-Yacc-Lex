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
"for"           {return FOR;}
"while"         {return WHILE;}
"function"      {return FUNCT;}
"return"        {return RETURN;}
"print"		    {return print;}
"let"           {return LET;}
"="             {return ASSIGN;}
[0-9]+          {yylval.num = atoi(yytext); return NUMBER;}
[a-zA-Z][a-zA-Z0-9_]*   {
                            char *myvar = malloc(strlen(yytext)*sizeof(char));
                            strcpy(myvar, yytext);
                            yylval.varName = myvar;
                            return VARNAME;
                        }
[ \t]           ; /* ignore les blancs et tabulations */
\n              ;
.               return yytext[0];


%%

int yywrap (void) {return 1;}