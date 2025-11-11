%{
    #include <string.h>
    #include <stdio.h>
	#include "symbolTable.h"
	#include "codeGeneration.h"

    void yyerror(char*);
    int yylex();
    extern SymTable table;
    extern FILE* out_file;

%}

%union {
	struct code_t
	{
		char str[2044]; 
		int op;
	} c;
}


%type <c> programa declaracoes declaracao bloco
%type <c> declaracao_inteiro declaracao_float
%type <c> comandos comando comando_atribuicao
%type <c> expressao_numerica termo fator


%token <c> VAR ID NUM NUM_REAL INT FLOAT
%left '+' '-'
%left '*' '/' '%'


%%

programa: declaracoes bloco  {
		fprintf(out_file, "%s", $1.str);
		dumpCodeDeclarationEnd();
		fprintf(out_file, "%s", $2.str);
	}
;


declaracoes: declaracao declaracoes  {
		strcpy($$.str, $1.str);
		sprintf($$.str + strlen($$.str), "%s", $2.str);
	}
	| %empty { $$.str[0] = '\0'; }
;


declaracao: declaracao_inteiro { strcpy($$.str, $1.str); }
	| declaracao_float { strcpy($$.str, $1.str); }
;


declaracao_inteiro: VAR ID ':' INT '=' NUM ';'  {
		if (findSymTable(&table, $2.str) != NULL) {
			char error_msg[256];
			char truncated_id[100]; // Buffer temporário para o nome da variável
			strncpy(truncated_id, $2.str, sizeof(truncated_id) - 1);
			truncated_id[sizeof(truncated_id) - 1] = '\0';
			snprintf(error_msg, sizeof(error_msg), "variavel '%s' ja declarada", truncated_id);
			yyerror(error_msg);
			YYABORT;
		}
		addSymTable(&table, $2.str, INTEGER, $6.str);
		makeCodeDeclaration($$.str, $2.str, INTEGER, $6.str);
	}
	| VAR ID ':' INT ';'  {
		if (findSymTable(&table, $2.str) != NULL) {
			char error_msg[256];
			char truncated_id[100];
			strncpy(truncated_id, $2.str, sizeof(truncated_id) - 1);
			truncated_id[sizeof(truncated_id) - 1] = '\0';
			snprintf(error_msg, sizeof(error_msg), "variavel '%s' ja declarada", truncated_id);
			yyerror(error_msg);
			YYABORT;
		}
		addSymTable(&table, $2.str, INTEGER, NULL);
		makeCodeDeclaration($$.str, $2.str, INTEGER, NULL);
	}
;

declaracao_float: VAR ID ':' FLOAT '=' NUM_REAL ';'  {
		if (findSymTable(&table, $2.str) != NULL) {
			char error_msg[256];
			char truncated_id[100];
			strncpy(truncated_id, $2.str, sizeof(truncated_id) - 1);
			truncated_id[sizeof(truncated_id) - 1] = '\0';
			snprintf(error_msg, sizeof(error_msg), "variavel '%s' ja declarada", truncated_id);
			yyerror(error_msg);
			YYABORT;
		}
		addSymTable(&table, $2.str, REAL, $6.str);
		makeCodeDeclaration($$.str, $2.str, REAL, $6.str);
	}
	| VAR ID ':' FLOAT ';'  {
		if (findSymTable(&table, $2.str) != NULL) {
			char error_msg[256];
			char truncated_id[100];
			strncpy(truncated_id, $2.str, sizeof(truncated_id) - 1);
			truncated_id[sizeof(truncated_id) - 1] = '\0';
			snprintf(error_msg, sizeof(error_msg), "variavel '%s' ja declarada", truncated_id);
			yyerror(error_msg);
			YYABORT;
		}
		addSymTable(&table, $2.str, REAL, NULL);
		makeCodeDeclaration($$.str, $2.str, REAL, NULL);
	}
;

bloco : '{' comandos '}'  {
		strcpy($$.str, $2.str);
	}
;


comandos : comando comandos  {
        strcpy($$.str, $1.str);
		sprintf($$.str + strlen($$.str), "%s", $2.str);
	}
	| %empty { $$.str[0] = '\0'; }
;


comando:  comando_atribuicao { strcpy($$.str, $1.str); }
;

comando_atribuicao: ID '=' expressao_numerica ';'  {
		if (findSymTable(&table, $1.str) == NULL) {
			char error_msg[256];
			char truncated_id[100];
			strncpy(truncated_id, $1.str, sizeof(truncated_id) - 1);
			truncated_id[sizeof(truncated_id) - 1] = '\0';
			snprintf(error_msg, sizeof(error_msg), "variavel '%s' nao foi declarada", truncated_id);
			yyerror(error_msg);
			YYABORT;
		}
		if (!makeCodeAssignment($$.str, $1.str, $3.str))
			YYABORT;
	}
;

expressao_numerica: expressao_numerica '+' termo {
        strcpy($$.str, $1.str);
        makeCodeAdd($$.str, $3.str);
    }
    | expressao_numerica '-' termo {
        strcpy($$.str, $1.str);
        makeCodeSub($$.str, $3.str);
    }
    | termo {
        strcpy($$.str, $1.str);
    }
;

termo: termo '*' fator {
        strcpy($$.str, $1.str);
        makeCodeMul($$.str, $3.str);
    }
    | termo '/' fator {
        strcpy($$.str, $1.str);
        makeCodeDiv($$.str, $3.str);
    }
    | termo '%' fator {
        strcpy($$.str, $1.str);
        makeCodeMod($$.str, $3.str);
    }
    | fator {
        strcpy($$.str, $1.str);
    }
;

fator: ID {
        if (findSymTable(&table, $1.str) == NULL) {
            char error_msg[256];
            char truncated_id[100];
			strncpy(truncated_id, $1.str, sizeof(truncated_id) - 1);
			truncated_id[sizeof(truncated_id) - 1] = '\0';
			snprintf(error_msg, sizeof(error_msg), "variavel '%s' nao foi declarada", truncated_id);
            yyerror(error_msg);
            YYABORT;
        }
        if (!makeCodeLoad($$.str, $1.str, 1))
            YYABORT;
    }
    | NUM {
        makeCodeLoad($$.str, $1.str, 0);
    }
    | NUM_REAL {
        makeCodeLoad($$.str, $1.str, 0); 
    }
    | '(' expressao_numerica ')' {
        strcpy($$.str, $2.str);
    }
;


%%

void yyerror(char *s) {
   fprintf(stderr, "Erro na linha %d: %s\n", cont_lines, s);
}
