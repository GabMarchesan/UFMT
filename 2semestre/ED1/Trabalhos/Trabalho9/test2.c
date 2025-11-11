#include <stdio.h>
#include <stdlib.h>

#define MAX_CHILDREN 3

// Definição da estrutura do nó da árvore
typedef struct TreeNode {
    int value;
    struct TreeNode** children;
    int numChildren;
} TreeNode;

// Função para criar um novo nó da árvore
TreeNode* createNode(int value) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->value = value;
    node->children = (TreeNode**)malloc(MAX_CHILDREN * sizeof(TreeNode*));
    node->numChildren = 0;
    return node;
}

// Função para inserir um nó na árvore
void insertNode(TreeNode* parent, int value) {
    if (parent->numChildren == MAX_CHILDREN) {
        printf("Número máximo de filhos excedido para o nó %d\n", parent->value);
        return;
    }

    TreeNode* node = createNode(value);
    parent->children[parent->numChildren] = node;
    parent->numChildren++;
}

// Função para calcular a altura da árvore
int calculateHeight(TreeNode* node) {
    if (node == NULL)
        return 0;

    int maxHeight = 0;

    for (int i = 0; i < node->numChildren; i++) {
        int childHeight = calculateHeight(node->children[i]);
        if (childHeight > maxHeight)
            maxHeight = childHeight;
    }

    return maxHeight + 1;
}

// Função para exibir o desenho da árvore em formato de texto
void printTree(TreeNode* node, int level) {
    if (node == NULL)
        return;

    printf("%*s%d\n", level * 4, "", node->value);

    for (int i = 0; i < node->numChildren; i++) {
        printTree(node->children[i], level + 1);
    }
}

int main() {
    int numRows, numCols;

    printf("Digite o número de linhas: ");
    scanf("%d", &numRows);

    printf("Digite o número de colunas: ");
    scanf("%d", &numCols);

    int** matrix = (int**)malloc(numRows * sizeof(int*));
    for (int i = 0; i < numRows; i++) {
        matrix[i] = (int*)malloc(numCols * sizeof(int));
        printf("Digite os valores da linha %d separados por espaço: ", i + 1);
        for (int j = 0; j < numCols; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }

    // Criando a primeira árvore percorrendo linha por linha
    TreeNode* rootRow = createNode(matrix[0][0]);

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            if (i == 0 && j == 0)
                continue;

            insertNode(rootRow, matrix[i][j]);
        }
    }

    // Criando a segunda árvore percorrendo coluna por coluna
    TreeNode* rootCol = createNode(matrix[0][0]);

    for (int j = 0; j < numCols; j++) {
        for (int i = 0; i < numRows; i++) {
            if (i == 0 && j == 0)
                continue;

            insertNode(rootCol, matrix[i][j]);
        }
    }

    printf("Árvore percorrendo linha por linha:\n");
    printTree(rootRow, 0);
    printf("Altura da árvore: %d\n", calculateHeight(rootRow));

    printf("Árvore percorrendo coluna por coluna:\n");
    printTree(rootCol, 0);
    printf("Altura da árvore: %d\n", calculateHeight(rootCol));

    // Liberar a memória alocada para a matriz
    for (int i = 0; i < numRows; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}

