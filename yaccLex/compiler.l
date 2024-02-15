%{
/* fichier dans lequel est defini la macro constante NOMBRE */
#include "./compiler.tab.h"
%}

%%

"True"          {return YACCTRUE;}
"False"         {return YACCFALSE;}
"&&"            {return YACCAND;}
"||"            {return YACCOR;}
"!"             {return YACCNOT;}
"=="            {return YACCEQ;}
"!="            {return YACCNEQ;}
">="            {return YACCGE;}
"<="            {return YACCLE;}
">"             {return YACCGT;}
"<"             {return YACCLT;}
"if"            {return YACCIF;}
"else"          {return YACCELSE;}
"for"           {return YACCFOR;}
"while"         {return YACCWHILE;}
"function"      {return YACCFUNCT;}
"return"        {return YACCRETURN;}
"print"		    {return YACCprint;}
"="             {return YACCASSIGN;}
[0-9]+\.[0-9]*  {yylval.floatVal = atof(yytext); return YACCFLOAT;}
[0-9]+          {yylval.num = atoi(yytext); return YACCNUMBER;}
[a-zA-Z][a-zA-Z0-9_]*   {
                            char *myvar = malloc((strlen(yytext)+1)*sizeof(char));
                            strcpy(myvar, yytext);
                            yylval.varName = myvar;
                            return YACCVARNAME;
                        }
[ \t]           ; /* ignore les blancs et tabulations */
\n              ;
.               return yytext[0];


%%

int yywrap (void) {return 1;}