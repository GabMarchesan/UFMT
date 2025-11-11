/* lexico.l */
%{
	#include <string.h>
	#include <stdio.h>
	#include "symbolTable.h"
	#include "sintatico.tab.h"

	void yyerror(char *); 
%}

ws [ \t]
letter [A-Za-z]
digit [0-9]


%%

{ws}           { }

"//".* { /* Apenas ignora o resto da linha */ }

\"[^\"\n]*\" {
	printf("Lexic: string literal found\n");
	strncpy(yylval.c.str, yytext + 1, yyleng - 2); // Copia a string SEM as aspas
	yylval.c.str[yyleng - 2] = '\0';
	return LITERAL_STR;
}

"int"	{
	printf("Lexic: int found\n");
	strcpy(yylval.c.str, yytext);
	return INT;
}

"float"	{
	printf("Lexic: float found\n");
	strcpy(yylval.c.str, yytext);
	return FLOAT;
}

"char" {
	printf("Lexic: char found\n");
	strcpy(yylval.c.str, yytext);
	return CHAR;
}

"string" {
	printf("Lexic: string found\n");
	strcpy(yylval.c.str, yytext);
	return STRING;
}

"bool" {
	printf("Lexic: bool found\n");
	strcpy(yylval.c.str, yytext);
	return BOOL;
}

"return" {
	printf("Lexic: return found\n");
	strcpy(yylval.c.str, yytext);
	return RETURN;
}

"if"	{
	printf("Lexic: if found\n");
	strcpy(yylval.c.str, yytext);
	return IF;
}

"else"	{
	printf("Lexic: else found\n");
	strcpy(yylval.c.str, yytext);
	return ELSE;
}

"while"	{
	printf("Lexic: while found\n");
	strcpy(yylval.c.str, yytext);
	return WHILE;
}

"read"	{
	printf("Lexic: read found\n");
	strcpy(yylval.c.str, yytext);
	return READ;
}

"write"	{
	printf("Lexic: write found\n");
	strcpy(yylval.c.str, yytext);
	return WRITE;
}

{digit}+"."{digit}+  {
	printf("Lexic: num_float found\n");
	strcpy(yylval.c.str, yytext);
	return NUM_FLOAT;
}

{digit}+ {
	printf("Lexic: num_int found\n");
	strcpy(yylval.c.str, yytext);
	return NUM_INT;
}

{letter}({letter}|{digit}|_)*	{
	printf("Lexic: id found\n");
	strcpy(yylval.c.str, yytext);
	return ID;
}

"=="	{
	printf("Lexic: == found\n");
	strcpy(yylval.c.str, yytext);
	return EQ;
}

"!="	{
	printf("Lexic: != found\n");
	strcpy(yylval.c.str, yytext);
	return NE;
}

"<="	{
	printf("Lexic: <= found\n");
	strcpy(yylval.c.str, yytext);
	return LE;
}

">="	{
	printf("Lexic: >= found\n");
	strcpy(yylval.c.str, yytext);
	return GE;
}

[-;={}+*()<>/%,"] {
	printf("Lexic: %c found\n", *yytext);
	return *yytext;
}

\n|\r|\n\r|\r\n { cont_lines++; }

. { yyerror("Invalid character"); }

%%

int yywrap (void){
 	return 1;
}