#include <stdio.h>
#include <stdlib.h>

typedef struct sNoA
{
    int chave;
    struct sNoA *esq;
    struct sNoA *dir;
} TNoA;

TNoA *cria(int chave)
{
    TNoA *novo = (TNoA *)malloc(sizeof(TNoA));
    novo->chave = chave;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

TNoA *rotacaoDireita(TNoA *raiz)
{
    TNoA *novoPai = raiz->esq;
    raiz->esq = novoPai->dir;
    novoPai->dir = raiz;
    return novoPai;
}

TNoA *rotacaoEsquerda(TNoA *raiz)
{
    TNoA *novoPai = raiz->dir;
    raiz->dir = novoPai->esq;
    novoPai->esq = raiz;
    return novoPai;
}

TNoA *insere(TNoA *no, int chave) {
    if (no == NULL) {
        TNoA *novo = (TNoA *)malloc(sizeof(TNoA));
        novo->chave = chave;
        novo->esq = NULL;
        novo->dir = NULL;
        return novo;
    }

    if (chave < no->chave) {
        no->esq = insere(no->esq, chave);
        if (no->esq->chave == chave) {
            return rotacaoDireita(no);
        }

        if (no->esq->esq != NULL && no->esq->esq->chave == chave) {
            return rotacaoDireita(rotacaoDireita(no));
        }

        if (no->esq->dir != NULL && no->esq->dir->chave == chave) {
            return rotacaoDireita(rotacaoEsquerda(no));
        }
    } else if (chave > no->chave) {
        no->dir = insere(no->dir, chave);
        if (no->dir->chave == chave) {
            return rotacaoEsquerda(no);
        }

        if (no->dir->dir != NULL && no->dir->dir->chave == chave) {
            return rotacaoEsquerda(rotacaoEsquerda(no));
        }

        if (no->dir->esq != NULL && no->dir->esq->chave == chave) {
            return rotacaoEsquerda(rotacaoDireita(no));
        }
    }

    return no;
}

TNoA *buscar(TNoA *raiz, int chave)
{
    if (raiz == NULL || raiz->chave == chave)
    {
        return raiz;
    }

    if (chave < raiz->chave)
    {
        raiz->esq = buscar(raiz->esq, chave);
        raiz = rotacaoDireita(raiz);
    }
    else
    {
        raiz->dir = buscar(raiz->dir, chave);
        raiz = rotacaoEsquerda(raiz);
    }

    return raiz;
}

void imprime(TNoA *nodo, int tab)
{
    if (nodo != NULL)
    {
        imprime(nodo->dir, tab + 2);
        printf("%*s%d\n", tab, "", nodo->chave);
        imprime(nodo->esq, tab + 2);
    }
}

int main(void)
{
    TNoA *raiz = NULL;
    int qt, value, x;

    printf("Digite o número de valores a serem inseridos na árvore: ");
    scanf("%i", &qt);

    printf("Digite os valores a serem inseridos:\n");
    for (int i = 0; i < qt; i++)
    {
        scanf("%i", &value);
        raiz = insere(raiz, value);
    }

    printf("Árvore após a inserção:\n");
    imprime(raiz, 0);

    printf("Digite o valor a ser buscado e movido para a raiz: ");
    scanf("%i", &x);
    raiz = buscar(raiz, x);

    printf("Árvore após a busca e movimentação:\n");
    imprime(raiz, 0);

    return 0;
}


