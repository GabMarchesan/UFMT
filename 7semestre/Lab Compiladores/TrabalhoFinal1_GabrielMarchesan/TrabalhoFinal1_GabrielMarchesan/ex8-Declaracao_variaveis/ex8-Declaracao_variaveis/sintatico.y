%{
    #include <string.h>
    #include <stdio.h>
	#include "symbolTable.h"
	#include "codeGeneration.h"

    void yyerror(char*);
    int yylex();
    extern SymTable table;
%}

%union {
	struct code_t{
		char str[4096];
		int op;
	} c;
}

%type <c> programa declaracoes declaracao bloco
%type <c> declaracao_inteiro declaracao_float
%type <c> comandos comando comando_escrita comando_leitura comando_atribuicao
%type <c> expressao_numerica
%type <c> expressao_booleana operador_relacional

%token <c> ID NUM_INT NUM_FLOAT INT FLOAT WRITE READ IF ELSE WHILE LITERAL_STR
%token <c> LE GE EQ NE

%nonassoc IFX
%nonassoc ELSE
%left '+' '-'
%left '*' '/' '%'

%%

programa:	 declaracoes bloco {
                fprintf(out_file, "%s", $1.str);
                dumpStringLiterals();
                dumpCodeDeclarationEnd();
                fprintf(out_file, "%s", $2.str);
                fprintf(out_file, "mov rax,0\nret\n");
            }
;

declaracoes: declaracao declaracoes 		{   strcpy($$.str, $1.str);
										sprintf($$.str + strlen($$.str), "%s", $2.str);}
			| %empty 				{ 	$$.str[0] = '\0'; }
;

declaracao:  declaracao_inteiro 			{   strcpy($$.str, $1.str); }
			| declaracao_float 				{   strcpy($$.str, $1.str); }
;

declaracao_inteiro: INT ID ';'  			{	addSymTable(&table, $2.str, INTEGER, NULL);
												makeCodeDeclaration($$.str, $2.str, INTEGER, NULL);}
;
declaracao_float: FLOAT ID ';'  			{	addSymTable(&table, $2.str, REAL, NULL);
												makeCodeDeclaration($$.str, $2.str, REAL, NULL);}
;

bloco : '{' comandos '}'  					{	strcpy($$.str, $2.str);}
;

comandos :  comando comandos  				{	strcpy($$.str, $1.str);
												sprintf($$.str + strlen($$.str), "%s", $2.str);}
			| %empty 						{ 	$$.str[0] = '\0'; }
;

comando:  comando_escrita
		| comando_leitura
		| comando_atribuicao
		| WHILE '(' expressao_booleana ')' comando { makeCodeWhile($$.str, $3.str, $3.op, $5.str); }
        | bloco
        | IF '(' expressao_booleana ')' comando %prec IFX { makeCodeIf($$.str, $3.str, $3.op, $5.str); }
        | IF '(' expressao_booleana ')' comando ELSE comando { makeCodeIfElse($$.str, $3.str, $3.op, $5.str, $7.str); }
;

comando_leitura: READ '('ID')' ';'  		{	if (!makeCodeRead($$.str, $3.str))
													YYABORT; }
;

comando_escrita: WRITE '(' ID ')' ';'                     { makeCodeWriteID($$.str, $3.str, 1); }
               | WRITE '(' expressao_numerica ')' ';'     { makeCodeWriteExpr($$.str, $3.str, 1); }
               | WRITE '(' LITERAL_STR ')' ';'            { makeCodeWriteString($$.str, $3.str); }
;

comando_atribuicao: ID '=' expressao_numerica ';'  	{	if (!makeCodeAssignment($$.str, $1.str, $3.str))
															YYABORT; }
;

expressao_numerica: expressao_numerica '+' expressao_numerica { makeCodeAdd($$.str, $1.str, $3.str); }
					| expressao_numerica '-' expressao_numerica { makeCodeSub($$.str, $1.str, $3.str); }
					| expressao_numerica '*' expressao_numerica { makeCodeMul($$.str, $1.str, $3.str); }
					| expressao_numerica '/' expressao_numerica { makeCodeDiv($$.str, $1.str, $3.str); }
					| expressao_numerica '%' expressao_numerica { makeCodeMod($$.str, $1.str, $3.str); }
					| ID  		{ if (!makeCodeLoad($$.str, $1.str, 1)) YYABORT; }
					| NUM_INT  	{ makeCodeLoad($$.str, $1.str, 0); }
					| NUM_FLOAT { makeCodeLoad($$.str, $1.str, 0); }
					| '(' expressao_numerica ')'  {	strcpy($$.str, $2.str); }
;

expressao_booleana: expressao_numerica operador_relacional expressao_numerica  {
																		$$.op = $2.op;
																		if (!makeCodeComp($$.str, $1.str, $3.str))
																			YYABORT; }
;

operador_relacional: '<'   { $$.op = -4; }
	| '>'                  { $$.op = -3; }
	| LE                   { $$.op = 3; }
	| GE                   { $$.op = 4; }
	| EQ                   { $$.op = -2; }
	| NE                   { $$.op = 2; }
;

%%

void yyerror(char *s){
   fprintf(stderr, "Error: %s at line %d", s, cont_lines);
   fprintf(stderr, "\n");
}