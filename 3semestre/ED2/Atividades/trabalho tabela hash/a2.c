#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

typedef struct Node {
    int key;
    char value[4];
    struct Node* next;
} Node;

typedef struct HashTable {
    Node* table[TABLE_SIZE];
} HashTable;

int hashFunction1(int key) {
    return key % TABLE_SIZE;
}

int hashFunction2(int key) {
    return (key * 37) % TABLE_SIZE;
}

Node* createNode(int key, const char* value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }
    newNode->key = key;
    strcpy(newNode->value, value);
    newNode->next = NULL;
    return newNode;
}

void insert(HashTable* hashTable, int key, const char* value, int functionType) {
    int index = functionType == 1 ? hashFunction1(key) : hashFunction2(key);
    
    Node* newNode = createNode(key, value);

    if (hashTable->table[index] == NULL) {
        hashTable->table[index] = newNode;
    } else {
        Node* current = hashTable->table[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Implementar busca e outras funções conforme necessário

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Uso: %s input_filename\n", argv[0]);
        return 1;
    }

    const char* inputFilename = argv[1];
    FILE* inFile = fopen(inputFilename, "r");

    if (!inFile) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    HashTable hashTable;
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable.table[i] = NULL;
    }

    int key;
    char value[4];

    while (fscanf(inFile, "%d %s", &key, value) != EOF) {
        insert(&hashTable, key, value, 1); // Change to 2 for testing
    }

    fclose(inFile);

    // Implementar contagem de colisões e testes de diferentes funções hash

    return 0;
}
