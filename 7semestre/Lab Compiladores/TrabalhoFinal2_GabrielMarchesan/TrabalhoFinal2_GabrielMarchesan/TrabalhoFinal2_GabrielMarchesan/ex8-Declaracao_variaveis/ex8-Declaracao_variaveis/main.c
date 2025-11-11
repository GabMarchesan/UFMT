// main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"
#include "codeGeneration.h"
#include "sintatico.tab.h"

extern FILE* yyin;

FILE* out_file = NULL;
int cont_lines = 1;

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        printf("comp: nenhum arquivo de entrada\n");
        return 1;
    }

    int n = strlen(argv[1]);
    char s[n + 10];
    memset(s, 0, sizeof(s));
    int i;
    for (i = n - 1; i >= 0 && argv[1][i] != '.'; i--){}
    if (i == -1) i = n;
    strncpy(s, argv[1], i);
    strcat(s, ".asm");
    
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        fprintf(stderr, "[ERRO]: Nao foi possivel abrir o arquivo de entrada %s\n", argv[1]);
        return 1;
    }

    out_file = fopen(s, "w");
    if (!out_file) {
        fprintf(stderr, "[ERRO]: Nao foi possivel criar o arquivo de saida %s\n", s);
        fclose(yyin);
        return 1;
    }
    
    fprintf(out_file, "extern printf\n");
    fprintf(out_file, "extern scanf\n");
    fprintf(out_file, "section .data\n");
    fprintf(out_file, "fmt_d: db \"%%ld\", 0\n");
    fprintf(out_file, "fmt_f: db \"%%f\", 0\n");
    fprintf(out_file, "fmt_s: db \"%%s\", 0\n");
    fprintf(out_file, "fmt_dln: db \"%%ld\", 10, 0\n");
    fprintf(out_file, "fmt_fln: db \"%%f\", 10, 0\n");
    fprintf(out_file, "fmt_sln: db \"%%s\", 10, 0\n");
    
    initScopeManager();
    
    int ret = yyparse();
    
    // Verificando se a função main foi definida
    if (ret == 0) {
        if (findFunc("main") == NULL) {
            fprintf(stderr, "Erro Semantico: funcao 'main' nao foi definida.\n");
            ret = 1; 
        }
    }
    
    if (ret != 0) {
        fclose(yyin);
        fclose(out_file);
        remove(s);
        return 1;
    }

    fclose(yyin);
    fclose(out_file);

    return 0;
}