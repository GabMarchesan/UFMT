#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_KEY 1024  // Valor máximo da chave (exclusivo)
#define NUM_LETTERS 3 // Número de letras no valor

// Função para gerar um valor aleatório composto por letras
void generate_random_value(char *value) {
    for (int i = 0; i < NUM_LETTERS; i++) {
        value[i] = 'a' + rand() % 26;
    }
    value[NUM_LETTERS] = '\0';
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <num_elementos> <tipo_arquivo> <nome_arquivo>\n", argv[0]);
        return 1;
    }

    int num_elements = atoi(argv[1]);
    int type = atoi(argv[2]);
    char *filename = argv[3];

    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erro ao criar o arquivo.\n");
        return 1;
    }

    for (int i = 0; i < num_elements; i++) {
        int key = (type == 1) ? 2 * i : rand() % MAX_KEY;
        char value[NUM_LETTERS + 1];
        generate_random_value(value);

        fprintf(file, "%d %s\n", key, value);
    }

    fclose(file);

    printf("Arquivo gerado com sucesso: %s\n", filename);

    return 0;
}
