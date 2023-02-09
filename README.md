# Test de compilateur avec Yacc et Lex

## Commandes à executer pour faire fonctionner le projet

(l'ordre d'execution est important)

**bison -d calc.y**

produit les fichiers :

*calc.tab.c* qui contient le code en c de l’analyseur

*calc.tab.h* qui contient la d ́efinition des codes des unités lexicales, afin qu’elles puissent être partagées par l’analyseur syntaxique et l’analyseur lexical.

**flex calc.l**

produit le fichier :

*lex.yy.c* qui contient le code en c de l’analyseur lexical.

**gcc -o calc calc.tab.c lex.yy.c**

produit l’executable :

*calc* qui permet d’analyser des expressions arithmétiques.


Il est ensuite possible de faire tourner le compilateur de 2 manières différentes :

- **./calc**

permet d'avoir un executeur ligne par ligne

- **./calc < testCalc.txt**

permet d'executer les commandes contenues dans un fichier

## Yacc et Lex 

### Lex

Lex est un générateur d’analyseur lexical, c'est à dire qu'il reconnait les expressions régulières et peut leurs associer des actions.

exemple : 

[0-9]+          {yylval.num = atoi(yytext); return NUMBER;}

[0-9] est une expression régulière permettant de reconnaitre les caractères '0', '1', '2', '3', '4', '5', '6', '7', '8' et '9'
'+' permet de trouver toutes expression respectant au moins une fois ou plus l'expression juste avant.
[0-9]+ reconnait tous les nombres, ayant 1 ou plusieurs chiffres.

Donc l'exemple va compiler tous les nombres comme de type NOMBRE

### Yacc

Yacc est un générateur d’analyseur syntaxique, c'est à dire qu'il va reconstruire une syntaxe à partir de plusieurs éléments de cette syntaxe.

exemple : 

calcul:             calcul ’+’ calcul
|                   calcul ’-’ calcul
|                   NOMBRE
;

on définit qu'un calcul peut être l'addition de 2 calculs, la différence entre 2 calculs ou un nombre. Ce qui permet de reconstuire tous les calculs possibles entre des nombres et en faisant des additions et des soustractions.

## Support

[Cours 1](https://pageperso.lis-lab.fr/alexis.nasr/Ens/Compilation/cmX_lex_yacc.pdf)

[Cours 2](https://lafibre.info/images/doc/201705_lex_yacc_tutorial.pdf)

[Header Yacc](https://www.ibm.com/docs/en/aix/7.2?topic=information-yacc-grammar-file-declarations)

[Complément Header Yacc](https://www.ibm.com/docs/en/zos/2.4.0?topic=section-precedence-associativity)