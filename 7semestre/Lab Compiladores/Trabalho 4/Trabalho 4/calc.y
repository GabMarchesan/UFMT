%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Protótipos de funções do Flex e Bison
int yylex(void);
int yyerror(char *);

// O Flex usará 'yyin' como fonte de entrada
extern FILE *yyin;

// Função para gerar nomes de variáveis temporárias (ex: t0, t1, t2...)
void newtemp(char *label) {
  static int label_count = 0;
  sprintf(label, "t%d", label_count++);
}
%}

/* Define uma 'union' para os tipos de atributos que os símbolos da gramática podem ter. */
%union {
  struct sCode {
    char code[2048]; // Armazena o código de 3 endereços gerado
    char temp[32];   // Armazena o nome da variável (temporária ou não)
  } attribute;
}

/*
 * Define a precedência e associatividade dos operadores.
 * Operadores declarados primeiro têm menor precedência.
 * %left indica associatividade à esquerda.
 */
%left '+' '-'
%left '*' '/'

// Define os tokens (símbolos terminais) e o tipo de seus atributos
%token <attribute> VARIABLE
%token <attribute> INTEGER

// Define os símbolos não-terminais e o tipo de seus atributos
%type <attribute> program statement expr

%%

/* Regras da Gramática */

program:
    /* uma produção vazia para permitir programas vazios */
    | program statement { 
        // Imprime o código gerado para cada instrução (statement)
        printf("%s", $2.code); 
      }
    ;

statement:
    expr '\n'       
        { 
            // Se a instrução for apenas uma expressão, o código é o da expressão.
            strcpy($$.code, $1.code); 
        }
    | VARIABLE '=' expr '\n' 
        {
            /* Gera o código para uma atribuição:
             * 1. Concatena o código gerado para a expressão à direita ($3.code).
             * 2. Adiciona a instrução final de atribuição (ex: a = t2).
             */
            sprintf($$.code, "%s%s = %s\n", $3.code, $1.temp, $3.temp);
        }
    ;

expr:
    INTEGER         
        {
            newtemp($$.temp);
            // Gera código para atribuir o número a uma nova variável temporária.
            sprintf($$.code, "%s = %s\n", $$.temp, $1.temp);
        }
    | VARIABLE      
        {
            // Uma variável sozinha não gera código, apenas passa seu nome para cima na árvore.
            strcpy($$.temp, $1.temp);
            strcpy($$.code, ""); // Nenhum código é gerado
        }
    | expr '+' expr 
        {
            newtemp($$.temp);
            // Gera código para a soma: código do operando 1, código do operando 2, e instrução da soma.
            sprintf($$.code, "%s%s%s = %s + %s\n", $1.code, $3.code, $$.temp, $1.temp, $3.temp);
        }
    | expr '-' expr 
        {
            newtemp($$.temp);
            sprintf($$.code, "%s%s%s = %s - %s\n", $1.code, $3.code, $$.temp, $1.temp, $3.temp);
        }
    | expr '*' expr 
        {
            newtemp($$.temp);
            sprintf($$.code, "%s%s%s = %s * %s\n", $1.code, $3.code, $$.temp, $1.temp, $3.temp);
        }
    | expr '/' expr 
        {
            newtemp($$.temp);
            sprintf($$.code, "%s%s%s = %s / %s\n", $1.code, $3.code, $$.temp, $1.temp, $3.temp);
        }
    | '(' expr ')'  
        {
            // Para parênteses, apenas passa os atributos da expressão interna para cima.
            strcpy($$.temp, $2.temp);
            strcpy($$.code, $2.code);
        }
    ;

%%

/* Código C Adicional */

int main(int argc, char *argv[]) {
    // Verifica se um nome de arquivo foi passado como argumento
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror(argv[1]);
            return 1;
        }
        // Aponta a entrada do Flex (yyin) para o arquivo
        yyin = file;
    }
    // Se nenhum arquivo for fornecido, a entrada padrão (teclado) será usada.

    yyparse();
    return 0;
}

// Função de tratamento de erros de parsing
int yyerror(char *s) {
    fprintf(stderr, "Error: %s\n", s);
    return 0;
}
