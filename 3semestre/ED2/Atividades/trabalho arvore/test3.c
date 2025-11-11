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

TNoA *insere(TNoA *no, int chave)
{
    if (no == NULL)
    {
        return cria(chave);
    }

    if (chave < no->chave)
    {
        no->esq = insere(no->esq, chave);
        // Aplicar rotação para trazer o nó inserido para a raiz
        return rotacaoDireita(no);
    }

    no->dir = insere(no->dir, chave);
    // Aplicar rotação para trazer o nó inserido para a raiz
    return rotacaoEsquerda(no);
}

TNoA *moverParaRaiz(TNoA *raiz, int chave)
{
    if (raiz == NULL || raiz->chave == chave)
    {
        return raiz;
    }

    if (chave < raiz->chave)
    {
        raiz->esq = moverParaRaiz(raiz->esq, chave);
        raiz = rotacaoDireita(raiz); // Corrigir a rotação
    }
    else
    {
        raiz->dir = moverParaRaiz(raiz->dir, chave);
        raiz = rotacaoEsquerda(raiz); // Corrigir a rotação
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

    printf("Digite o valor a ser movido para a raiz: ");
    scanf("%i", &x);
    raiz = moverParaRaiz(raiz, x);

    printf("Árvore após a inserção e movimentação:\n");
    imprime(raiz, 0);

    return 0;
}
