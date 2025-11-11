%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    int yylex(void); 
    void yyerror(char *s); 

    // Variável externa do Flex que aponta para o arquivo de entrada
    extern FILE *yyin;
%}

%union {
    char str[128];
}

%token <str> ARTIGO_A ARTIGO_O
%token <str> SUBSTANTIVO_GATO SUBSTANTIVO_CACHORRO SUBSTANTIVO_COMIDA SUBSTANTIVO_BISCOITO
%token <str> VERBO_E VERBO_COME
%token <str> ADJETIVO_BONITO ADJETIVO_FEIO

%type <str> oracao sujeito predicado artigo substantivo verbo complemento

%%

programa:
      /* pode começar vazio */
    | programa oracao '\n' { printf("=> Frase Válida: '%s'\n\n", $2); }
    | programa '\n'      { /* ignora linhas em branco */ }
    ;

oracao: 
    sujeito predicado {
        strcpy($$, $1);
        strcat($$, " ");
        strcat($$, $2);
    }
    ;

sujeito: 
    artigo substantivo {
        strcpy($$, $1);
        strcat($$, " ");
        strcat($$, $2);
        printf("Reconhecido [SUJEITO]: %s\n", $$);
    }
    ;                
                          
artigo: 
    ARTIGO_O { strcpy($$, $1); }
    | ARTIGO_A { strcpy($$, $1); }
    ;
        
substantivo: 
    SUBSTANTIVO_GATO      { strcpy($$, $1); }
    | SUBSTANTIVO_CACHORRO  { strcpy($$, $1); }
    | SUBSTANTIVO_COMIDA    { strcpy($$, $1); }
    | SUBSTANTIVO_BISCOITO  { strcpy($$, $1); }
    ;

predicado: 
    verbo complemento {
        strcpy($$, $1);
        strcat($$, " ");
        strcat($$, $2);
        printf("Reconhecido [PREDICADO]: %s\n", $$);
    }
    ;

verbo:  
    VERBO_E   { strcpy($$, $1); }
    | VERBO_COME { strcpy($$, $1); }
    ;
        
complemento: 
    ADJETIVO_BONITO { strcpy($$, $1); }
    | ADJETIVO_FEIO   { strcpy($$, $1); }
    | substantivo     { strcpy($$, $1); }
    ;

%% 

void yyerror(char *s) { 
    fprintf(stderr, "Erro de Sintaxe: %s\n", s); 
} 

int main(int argc, char *argv[]) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror(argv[1]);
            return 1;
        }
        // Conecta o arquivo de entrada ao analisador léxico
        yyin = file;
    }
    // Inicia a análise
    return yyparse();
}
