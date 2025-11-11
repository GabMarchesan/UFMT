%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct simbolo {
    char *nome;
    int valor;
    struct simbolo *prox;
};

struct simbolo *tabela = NULL;

int yylex(void);
void yyerror(const char *s);

struct simbolo* busca(char *nome) {
    struct simbolo *atual = tabela;
    while (atual) {
        if (strcmp(atual->nome, nome) == 0)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}

struct simbolo* insere(char *nome, int valor) {
    struct simbolo *novo = malloc(sizeof(struct simbolo));
    novo->nome = strdup(nome);
    novo->valor = valor;
    novo->prox = tabela;
    tabela = novo;
    return novo;
}
%}

%union {
    int valor;
    char *id;
}

%token <valor> NUM
%token <id> ID
%type <valor> expr

%%

cmds: 
      cmds cmd
    | cmd
    ;

cmd:
      ID '=' expr ';' {
            struct simbolo *s = busca($1);
            if (!s) s = insere($1, $3);
            else s->valor = $3;
            free($1);
        }
    | expr ';'   { printf("Resultado: %d\n", $1); }
    ;

expr:
      expr '+' expr { $$ = $1 + $3; }
    | expr '-' expr { $$ = $1 - $3; }
    | expr '*' expr { $$ = $1 * $3; }
    | expr '/' expr { $$ = $1 / $3; }
    | NUM           { $$ = $1; }
    | ID {
            struct simbolo *s = busca($1);
            if (!s) {
                printf("Erro: variável '%s' não definida!\n", $1);
                $$ = 0;
            } else $$ = s->valor;
            free($1);
        }
    | '(' expr ')'  { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro: %s\n", s);
}

int main() {
    yyparse();
    return 0;
}
