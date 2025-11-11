// Bibliotecas
#include <stdio.h>
#include <stdlib.h>

// Estrutura tipo lista
typedef struct lista {
    int info;
    struct lista* prox;
} Tlista;

// Cria uma lista
Tlista* cria_lista(void) {
    return NULL;
}

// Insere no início da lista
Tlista* insere_inicio(Tlista* lst, int i) {
    Tlista* novo = (Tlista*) malloc(sizeof(Tlista));
    novo->info = i;
    novo->prox = lst;
    return novo;
}

// Insere no fim da lista
Tlista* insere_fim(Tlista* lst, int i) {
    Tlista* novo = (Tlista*) malloc(sizeof(Tlista));
    novo->info = i;
    novo->prox = NULL;
    Tlista* p = lst;
    if (p == NULL) { // Se a lista estiver vazia
        lst = novo;
    } else {
        while(p->prox != NULL) { // Encontra o último elemento
            p = p->prox;
        }
        p->prox = novo;
    }
    return lst;
}

// Insere ordenado na lista
Tlista* insere_ordenado(Tlista* lst, int i) {
    Tlista* novo = (Tlista*) malloc(sizeof(Tlista));
    novo->info = i;
    if (lst == NULL || i < lst->info) { // Insere no início da lista se ela estiver vazia ou se o elemento for menor que o primeiro
        novo->prox = lst;
        return novo;
    } else {
        Tlista* p = lst;
        while (p->prox != NULL && p->prox->info < i) { // Encontra a posição de inserção
            p = p->prox;
        }
        novo->prox = p->prox;
        p->prox = novo;
    }
    return lst;
}

// Imprime elementos da lista
void imprime_lista(Tlista* lst) {
    Tlista* p;
    for (p = lst; p != NULL; p = p->prox) {
        printf("Info = %d\n", p->info);
    }
}

// Função principal
int main() {
    Tlista* lista = cria_lista();
    int valor;
    int opcao;

    while (1) {
        printf("\n1 - Inserir no início da lista\n");
        printf("2 - Inserir no fim da lista\n");
        printf("3 - Inserir ordenado na lista\n");
        printf("4 - Imprimir lista\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o valor a ser inserido no início da lista: ");
                scanf("%d", &valor);
                lista = insere_inicio(lista, valor);
                break;
            case 2:
                printf("Digite o valor a ser inserido no fim da lista: ");
                scanf("%d", &valor);
                lista = insere_fim(lista, valor);
                break;
            case 3:
                printf("Digite o valor a ser inserido ordenado na lista: ");
                scanf("%d", &valor);
                lista = insere_ordenado(lista, valor);
                break;
            case 4:
                printf("Lista:\n");
                imprime_lista(lista);
                break;
            case 0:
                printf("Encerrando o programa.\n");
                exit(0);
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    }

    return 0;
}
