/**
 *
 * UFMT - Ciencia da Computacao
 * 
 * ED1 - Prof. Ivairton
 * 
 * Aluno: Herrison Batista de Meneses Junior // Gabriel Gomes Marchesan
 * 
 * Data: 21/05/2023
 * 
 * 
 *  Trabalho 8 do código binário
 * 
 **/

//Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Estrutura de nó da árvore
typedef struct Node {
    char symbol;
    struct Node* dot;
    struct Node* dash;
} Node;

// Função para criar um novo nó da árvore
Node* createNode(char symbol) {
    Node* node = malloc(sizeof(Node));
    if (node == NULL) {
        printf("Falha na alocação de memória.\n");
        exit(1);
    }
    node->symbol = symbol;
    node->dot = NULL;
    node->dash = NULL;
    return node;
}

// Função para inserir um símbolo na árvore
void insertSymbol(Node* root, const char* morse, char symbol) {
    Node* current = root;
    while (*morse != '\0') {
        if (*morse == '.') {
            if (current->dot == NULL)
                current->dot = createNode('\0');
            current = current->dot;
        } else if (*morse == '-') {
            if (current->dash == NULL)
                current->dash = createNode('\0');
            current = current->dash;
        }
        morse++;
    }
    current->symbol = symbol;
}

// Função para construir a árvore de acordo com a tabela do Código Morse
Node* buildTree() {
    Node* root = createNode('\0');
    insertSymbol(root, ".-", 'A');
    insertSymbol(root, "-...", 'B');
    insertSymbol(root, "-.-.", 'C');
    insertSymbol(root, "-..", 'D');
    insertSymbol(root, ".", 'E');
    insertSymbol(root, "..-.", 'F');
    insertSymbol(root, "--.", 'G');
    insertSymbol(root, "....", 'H');
    insertSymbol(root, "..", 'I');
    insertSymbol(root, ".---", 'J');
    insertSymbol(root, "-.-", 'K');
    insertSymbol(root, ".-..", 'L');
    insertSymbol(root, "--", 'M');
    insertSymbol(root, "-.", 'N');
    insertSymbol(root, "---", 'O');
    insertSymbol(root, ".--.", 'P');
    insertSymbol(root, "--.-", 'Q');
    insertSymbol(root, ".-.", 'R');
    insertSymbol(root, "...", 'S');
    insertSymbol(root, "-", 'T');
    insertSymbol(root, "..-", 'U');
    insertSymbol(root, "...-", 'V');
    insertSymbol(root, ".--", 'W');
    insertSymbol(root, "-..-", 'X');
    insertSymbol(root, "-.--", 'Y');
    insertSymbol(root, "--..", 'Z');
    insertSymbol(root, "-----", '0');
    insertSymbol(root, ".----", '1');
    insertSymbol(root, "..---", '2');
    insertSymbol(root, "...--", '3');
    insertSymbol(root, "....-", '4');
    insertSymbol(root, ".....", '5');
    insertSymbol(root, "-....", '6');
    insertSymbol(root, "--...", '7');
    insertSymbol(root, "---..", '8');
    insertSymbol(root, "----.", '9');
    return root;
}

// Função auxiliar para imprimir a árvore em pré-ordem
void printTree(Node* root, char* code, int level) {
    if (root == NULL)
        return;
    for (int i = 0; i < level; i++)
        printf("  ");
    printf("%s %c\n", code, root->symbol);
    code[level] = '.';
    printTree(root->dot, code, level + 1);
    code[level] = '-';
    printTree(root->dash, code, level + 1);
    code[level] = '\0';
}

// Função para encontrar o nó correspondente a um símbolo na árvore
Node* findSymbol(Node* root, char symbol) {
    if (root == NULL)
        return NULL;
    if (root->symbol == symbol)
        return root;
    Node* node = findSymbol(root->dot, symbol);
    if (node == NULL)
        node = findSymbol(root->dash, symbol);
    return node;
}

// Função para traduzir a mensagem em Código Morse
void translateMessage(Node* root, const char* morse) {
    char* token = strtok(morse, " ");
    while (token != NULL) {
        if (strcmp(token, "/") == 0)
            printf(" ");
        else {
            Node* current = root;
            for (int i = 0; i < strlen(token); i++) {
                if (token[i] == '.')
                    current = current->dot;
                else if (token[i] == '-')
                    current = current->dash;
            }
            printf("%c", current->symbol);
        }
        token = strtok(NULL, " ");
    }
    printf("\n");
}

void freeTree(Node* root) {
    if (root == NULL)
        return;
    freeTree(root->dot);
    freeTree(root->dash);
    free(root);
}

//int main
int main() {
    Node* root = buildTree();
    printf("Árvore em pré-ordem:\n");
    char code[100] = "";
    printTree(root, code, 0);
    printf("\n");

    char morse[100];
    printf("Digite a mensagem em Código Morse: ");
    fgets(morse, sizeof(morse), stdin);
    morse[strcspn(morse, "\n")] = '\0';

    printf("Tradução: ");
    translateMessage(root, morse);

    freeTree(root);
    return 0;
}