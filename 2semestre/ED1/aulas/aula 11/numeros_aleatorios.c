#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 999999

num_aleatorios(){
    
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <num_valores>\n", argv[0]);
        return 1;
    }
    int num_valores = atoi(argv[1]);
    if (num_valores < 1) {
        printf("Erro: número de valores inválido\n");
        return 1;
    }

    // Inicializa o gerador de números aleatórios com a hora atual
    srand(time(NULL));

    // Gera os números aleatórios sem repetições
    int numeros[num_valores];
    for (int i = 0; i < num_valores; i++) {
        int novo_numero;
        do {
            novo_numero = rand() % MAX + 1;
        } while (i > 0 && novo_numero == numeros[i-1]);
        numeros[i] = novo_numero;
    }

    // Escreve os números no arquivo de saída
    FILE *saida = fopen("entrada.txt", "w");
    if (saida == NULL) {
        printf("Erro ao abrir arquivo de saída\n");
        return 1;
    }
    for (int i = 0; i < num_valores; i++) {
        fprintf(saida, "%d\n", numeros[i]);
    }
    fclose(saida);

    return 0;
}