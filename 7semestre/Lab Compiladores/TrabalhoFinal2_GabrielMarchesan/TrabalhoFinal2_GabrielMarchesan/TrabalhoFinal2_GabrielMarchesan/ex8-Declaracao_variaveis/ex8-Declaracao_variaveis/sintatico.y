// sintatico.y

%{
    #include <string.h>
    #include <stdio.h>
	#include "symbolTable.h"
	#include "codeGeneration.h"

    void yyerror(char*);
    int yylex();

    Type current_type; 
    FuncTableEntry* current_function = NULL;
%}

%union {
	struct code_t{
		char str[4096];
		int op;
        Type type;
        ParamNode* params;
        ArgNode* args;
	} c;
}

%type <c> programa declaracoes_globais declaracao_global
%type <c> tipo declaracao_variavel declaracao_funcao prototipo_funcao
%type <c> bloco comandos comando comando_escrita comando_leitura comando_atribuicao
%type <c> expressao_numerica expressao_booleana operador_relacional
%type <c> comando_retorno
%type <c> lista_parametros parametro lista_parametros_chamada

%token <c> ID NUM_INT NUM_FLOAT LITERAL_STR
%token <c> INT FLOAT CHAR STRING BOOL 
%token <c> WRITE READ IF ELSE WHILE RETURN
%token <c> LE GE EQ NE

%nonassoc IFX
%nonassoc ELSE
%left '+' '-'
%left '*' '/' '%'

%%

programa: declaracoes_globais {
            fprintf(out_file, "%s", $1.str);
            dumpStringLiterals();
            dumpCodeDeclarationEnd();
        }
;

declaracoes_globais: declaracao_global declaracoes_globais {
                        strcpy($$.str, $1.str);
                        sprintf($$.str + strlen($$.str), "%s", $2.str);
                    }
                    | %empty { $$.str[0] = '\0'; }
;

declaracao_global:  prototipo_funcao      { strcpy($$.str, $1.str); }
                    | declaracao_funcao   { strcpy($$.str, $1.str); }
                    | declaracao_variavel ';' { strcpy($$.str, $1.str); }
;

tipo:   INT     { $$ = $1; current_type = TYPE_INT; }
      | FLOAT   { $$ = $1; current_type = TYPE_FLOAT; }
      | CHAR    { $$ = $1; current_type = TYPE_CHAR; }
      | STRING  { $$ = $1; current_type = TYPE_STRING; }
      | BOOL    { $$ = $1; current_type = TYPE_BOOL; }
;

declaracao_variavel: tipo ID {
                        if (current_function == NULL) {
                            if (!addVar(scope_manager.global_scope, $2.str, current_type, VAR_GLOBAL)) {
                                yyerror("Variavel global redefinida");
                                YYABORT;
                            }
                            makeCodeDeclaration($$.str, $2.str, current_type, VAR_GLOBAL);
                        } else {
                            if (!addVar(scope_manager.local_scope, $2.str, current_type, VAR_LOCAL)) {
                                yyerror("Variavel local redefinida");
                                YYABORT;
                            }
                            $$.str[0] = '\0';
                        }
                    }
;

prototipo_funcao: tipo ID '(' lista_parametros ')' ';' {
                        addFunc($2.str, current_type, $4.params);
                        $$.str[0] = '\0';
                    }
;

lista_parametros: parametro ',' lista_parametros {
                        $1.params->next = $3.params;
                        $$.params = $1.params;
                    }
                  | parametro { $$.params = $1.params; }
                  | %empty    { $$.params = NULL; }
;

parametro: tipo ID {
                $$.params = createParam(current_type, $2.str, NULL);
           }
;

declaracao_funcao: tipo ID '(' lista_parametros ')' {
                        current_function = findFunc($2.str);
                        if (current_function == NULL) {
                            yyerror("Funcao deve ser prototipada antes da implementacao");
                            YYABORT;
                        }
                        createLocalScope();
                        ParamNode* p = $4.params;
                        int param_offset = 16;
                        while (p != NULL) {
                            addVar(scope_manager.local_scope, p->identifier, p->type, PARAM);
                            SymTableEntry* entry = findVar(p->identifier);
                            entry->offset = param_offset;
                            param_offset += 8;
                            p = p->next;
                        }
                        freeParams($4.params);
                   }
                   bloco {
                        char prologue[1024], epilogue[1024];
                        makeCodeFunctionPrologue(prologue, $2.str, scope_manager.local_scope->current_local_offset - 8);
                        makeCodeFunctionEpilogue(epilogue);
                        
                        sprintf($$.str, "%s%s%s", prologue, $7.str, epilogue);
                        
                        destroyLocalScope();
                        current_function = NULL;
                   }
;

bloco : '{' comandos '}' { strcpy($$.str, $2.str); }
;

comandos :  comando comandos  {
                strcpy($$.str, $1.str);
                sprintf($$.str + strlen($$.str), "%s", $2.str);
            }
			| %empty { $$.str[0] = '\0'; }
;

comando:    declaracao_variavel ';' { strcpy($$.str, $1.str); }
            | comando_escrita       { strcpy($$.str, $1.str); }
            | comando_leitura       { strcpy($$.str, $1.str); }
            | comando_atribuicao    { strcpy($$.str, $1.str); }
            | comando_retorno       { strcpy($$.str, $1.str); }
            | WHILE '(' expressao_booleana ')' comando { makeCodeWhile($$.str, $3.str, $3.op, $5.str); }
            | bloco                 { strcpy($$.str, $1.str); }
            | IF '(' expressao_booleana ')' comando %prec IFX { makeCodeIf($$.str, $3.str, $3.op, $5.str); }
            | IF '(' expressao_booleana ')' comando ELSE comando { makeCodeIfElse($$.str, $3.str, $3.op, $5.str, $7.str); }
;

comando_retorno: RETURN expressao_numerica ';' {
                    if (current_function == NULL) {
                        yyerror("'return' fora de uma funcao");
                        YYABORT;
                    }
                    if (current_function->return_type != $2.type) {
                        yyerror("Tipo de retorno incompativel com o prototipo da funcao");
                        YYABORT;
                    }
                    makeCodeReturn($$.str, $2.str);
                 }
;

comando_leitura: READ '('ID')' ';' {
                    if (!makeCodeRead($$.str, $3.str)) {
                        yyerror("Variavel nao declarada para leitura");
                        YYABORT;
                    }
                }
;

comando_escrita: WRITE '(' ID ')' ';' {
                    if(!makeCodeWriteID($$.str, $3.str, 1)) {
                        yyerror("Variavel nao declarada para escrita");
                        YYABORT;
                    }
                 }
               | WRITE '(' expressao_numerica ')' ';' { makeCodeWriteExpr($$.str, $3.str, 1); }
               | WRITE '(' LITERAL_STR ')' ';'        { makeCodeWriteString($$.str, $3.str); }
;

comando_atribuicao: ID '=' expressao_numerica ';' {
                        if (!makeCodeAssignment($$.str, $1.str, $3.str)) {
                            yyerror("Variavel de atribuicao nao declarada");
                            YYABORT;
                        }
                    }
                    | ID '=' LITERAL_STR ';' {
                        if (!makeCodeStringAssignment($$.str, $1.str, $3.str)) {
                            yyerror("Erro na atribuicao de string (tipo da variavel eh 'string'?)");
                            YYABORT;
                        }
                    }
;

expressao_numerica: expressao_numerica '+' expressao_numerica { 
                        makeCodeAdd($$.str, $1.str, $3.str);
                        if ($1.type == TYPE_FLOAT || $3.type == TYPE_FLOAT) $$.type = TYPE_FLOAT;
                        else $$.type = TYPE_INT;
                    }
					| expressao_numerica '-' expressao_numerica { 
                        makeCodeSub($$.str, $1.str, $3.str);
                        if ($1.type == TYPE_FLOAT || $3.type == TYPE_FLOAT) $$.type = TYPE_FLOAT;
                        else $$.type = TYPE_INT;
                    }
					| expressao_numerica '*' expressao_numerica {
                        makeCodeMul($$.str, $1.str, $3.str);
                        if ($1.type == TYPE_FLOAT || $3.type == TYPE_FLOAT) $$.type = TYPE_FLOAT;
                        else $$.type = TYPE_INT;
                    }
					| expressao_numerica '/' expressao_numerica { 
                        makeCodeDiv($$.str, $1.str, $3.str);
                        $$.type = TYPE_FLOAT;
                    }
					| expressao_numerica '%' expressao_numerica {
                        makeCodeMod($$.str, $1.str, $3.str);
                        $$.type = TYPE_INT;
                    }
					| ID  		{ 
                        if (!makeCodeLoad($$.str, $1.str, 1)) {yyerror("Variavel nao declarada"); YYABORT;}
                        SymTableEntry* entry = findVar($1.str);
                        $$.type = entry->type;
                    }
					| NUM_INT  	{ makeCodeLoad($$.str, $1.str, 0); $$.type = TYPE_INT; }
					| NUM_FLOAT { makeCodeLoad($$.str, $1.str, 0); $$.type = TYPE_FLOAT; }
					| '(' expressao_numerica ')'  {	
                        strcpy($$.str, $2.str);
                        $$.type = $2.type;
                    }
                    | ID '(' lista_parametros_chamada ')' {
                        FuncTableEntry* func = findFunc($1.str);
                        if (func == NULL) {
                            yyerror("Funcao nao declarada");
                            YYABORT;
                        }

                        int expected_params = func->param_count;
                        int actual_params = countArgs($3.args);

                        if (expected_params != actual_params) {
                            yyerror("Numero incorreto de argumentos na chamada da funcao");
                            YYABORT;
                        }

                        ArgNode* current_arg = $3.args;
                        ParamNode* current_param = func->params;
                        char all_params_code[4096] = "";

                        while(current_arg != NULL && current_param != NULL) {
                            if (current_arg->type != current_param->type) {
                                yyerror("Tipo de argumento incompativel com o prototipo da funcao");
                                YYABORT;
                            }
                            char temp_code[4096];
                            strcpy(temp_code, all_params_code);
                            sprintf(all_params_code, "%s%s", current_arg->code, temp_code);

                            current_arg = current_arg->next;
                            current_param = current_param->next;
                        }
                        
                        makeCodeFunctionCall($$.str, $1.str, all_params_code, func->param_count);
                        $$.type = func->return_type;
                        freeArgs($3.args);
                      }
;

lista_parametros_chamada: expressao_numerica ',' lista_parametros_chamada {
                              $$.args = createArg($1.type, $1.str, $3.args);
                          }
                        | expressao_numerica { 
                              $$.args = createArg($1.type, $1.str, NULL);
                          }
                        | %empty             { $$.args = NULL; }
;

expressao_booleana: expressao_numerica operador_relacional expressao_numerica  {
																		$$.op = $2.op;
																		if (!makeCodeComp($$.str, $1.str, $3.str))
																			YYABORT; }
;

operador_relacional: '<'   { $$.op = -4; }
                    | '>'   { $$.op = -3; }
                    | LE    { $$.op = 3;  }
                    | GE    { $$.op = 4;  }
                    | EQ    { $$.op = -2; }
                    | NE    { $$.op = 2;  }
;

%%

void yyerror(char *s){
   fprintf(stderr, "Erro sintatico/semantico: %s na linha %d\n", s, cont_lines);
}