#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

// Estrutura para cada nó da lista de encadeamento externo
typedef struct Node {
    int key;
    char value[4]; // 3 letras + '\0'
    struct Node *next;
} Node;

// Função de hash 1
int hash_function_1(int key) {
    return key % TABLE_SIZE;
}

// Função de hash 2
int hash_function_2(int key) {
    return (key * 2 + 1) % TABLE_SIZE;
}

// Função para criar um novo nó
Node *create_node(int key, const char *value) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        perror("Erro na alocação de memória");
        exit(EXIT_FAILURE);
    }
    new_node->key = key;
    strcpy(new_node->value, value);
    new_node->next = NULL;
    return new_node;
}

// Função para inserir um elemento na tabela Hash
void insert_element(Node *hash_table[], int key, const char *value, int (*hash_function)(int)) {
    int index = hash_function(key);
    Node *new_node = create_node(key, value);

    if (hash_table[index] == NULL) {
        hash_table[index] = new_node;
    } else {
        Node *current = hash_table[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

// Função para buscar um elemento na tabela Hash
Node *search_element(Node *hash_table[], int key, int (*hash_function)(int)) {
    int index = hash_function(key);
    Node *current = hash_table[index];

    while (current != NULL) {
        if (current->key == key) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

// Função principal
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <nome_arquivo>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    Node *hash_table[TABLE_SIZE] = {NULL}; // Inicialização da tabela Hash

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int key;
    char value[4];

    while (fscanf(file, "%d %3s\n", &key, value) != EOF) {
        insert_element(hash_table, key, value, hash_function_1); // Usar a função de hash 1
        // insert_element(hash_table, key, value, hash_function_2); // Usar a função de hash 2
    }

    fclose(file);

    // Exemplo de busca
    int search_key = 42; // Chave a ser buscada
    Node *result = search_element(hash_table, search_key, hash_function_1); // Usar a função de hash 1
    // Node *result = search_element(hash_table, search_key, hash_function_2); // Usar a função de hash 2

    if (result != NULL) {
        printf("Elemento encontrado: Chave = %d, Valor = %s\n", result->key, result->value);
    } else {
        printf("Elemento não encontrado.\n");
    }

    // Liberar memória alocada para a tabela Hash
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = hash_table[i];
        while (current != NULL) {
            Node *temp = current;
            current = current->next;
            free(temp);
        }
    }

    return 0;
}
