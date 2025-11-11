%{
	#include <string.h>
	#include <stdio.h>
	#include "sintatico.tab.h"
	#include "symbolTable.h"
	void yyerror(char *); 
%}

ws      [ \t\r]+
letter  [A-Za-z]
digit   [0-9]

%%

"//".* { /* Comentarios de uma linha sao ignorados */ }

"var"	{
	strcpy(yylval.c.str, yytext);
	return VAR;
}

"int"	{
	strcpy(yylval.c.str, yytext);
	return INT;
}

"float" {
    strcpy(yylval.c.str, yytext);
    return FLOAT;
}

{digit}+\.{digit}+  {
	strcpy(yylval.c.str, yytext);
	return NUM_REAL;
}

{digit}+  {
	strcpy(yylval.c.str, yytext);
	return NUM;
}

{letter}({letter}|{digit}|_)*	{
	strcpy(yylval.c.str, yytext);
	return ID;
}

[-;:={}+*()/] { /* Removido o '%' para ser tratado pela regra abaixo se necessario */
	return *yytext;
}

"%" {
    return *yytext;
}

{ws}           { /* Ignora espacos em branco, tabs e carriage return */ }

\n             { cont_lines++; }

. { yyerror("Caractere invalido"); }

%%

int yywrap (void)
{
 	return 1;
}