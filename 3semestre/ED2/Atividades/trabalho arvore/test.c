#include <stdio.h>
#include <stdlib.h>

typedef struct sNoA
{
    int chave;
    struct sNoA *esq;
    struct sNoA *dir;
} TNoA;

TNoA *cria(void)
{
    return NULL;
}

void imprime(TNoA *nodo, int tab)
{
    if (nodo != NULL)
    {
        imprime(nodo->esq, tab + 2);
        printf("%*s%d\n", tab, "", nodo->chave);
        imprime(nodo->dir, tab + 2);
    }
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

TNoA *busca(TNoA *no, int chave)
{
    if (no == NULL || no->chave == chave)
    {
        return no;
    }

    if (chave < no->chave)
    {
        no->esq = busca(no->esq, chave);
        // Aplicar rotação para trazer o nó encontrado para a raiz
        return rotacaoDireita(no);
    }

    no->dir = busca(no->dir, chave);
    // Aplicar rotação para trazer o nó encontrado para a raiz
    return rotacaoEsquerda(no);
}

TNoA *insere(TNoA *no, int chave)
{
    if (no == NULL)
    {
        TNoA *novo = (TNoA *)malloc(sizeof(TNoA));
        novo->chave = chave;
        novo->esq = NULL;
        novo->dir = NULL;
        return novo;
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

int main(void)
{
    TNoA *raiz;
    raiz = cria();
    int qt, value, x;

    printf("Insira a quantidade de números a ser inserido:\n");
    scanf("%i", &qt);

    for (int i = 0; i < qt; i++)
    {
        scanf("%i", &value);
        raiz = insere(raiz, value);
    }
    scanf("%i", &x);
    raiz = busca(raiz, x);
    imprime(raiz, 0);

    return 0;
}
