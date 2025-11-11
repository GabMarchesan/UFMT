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

void imprime(TNoA *nodo, int tab)
{
    if (nodo != NULL)
    {
        imprime(nodo->dir, tab + 2);
        printf("%*s%d\n", tab, "", nodo->chave);
        imprime(nodo->esq, tab + 2);
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

TNoA *insereBalanceado(TNoA *no, int chave)
{
    if (no == NULL)
    {
        return cria(chave);
    }

    if (chave < no->chave)
    {
        no->esq = insereBalanceado(no->esq, chave);

        if (altura(no->esq) - altura(no->dir) == 2)
        {
            if (chave < no->esq->chave)
            {
                no = rotacaoDireita(no);
            }
            else
            {
                no->esq = rotacaoEsquerda(no->esq);
                no = rotacaoDireita(no);
            }
        }
    }
    else
    {
        no->dir = insereBalanceado(no->dir, chave);

        if (altura(no->dir) - altura(no->esq) == 2)
        {
            if (chave > no->dir->chave)
            {
                no = rotacaoEsquerda(no);
            }
            else
            {
                no->dir = rotacaoDireita(no->dir);
                no = rotacaoEsquerda(no);
            }
        }
    }

    return no;
}

int altura(TNoA *no)
{
    if (no == NULL)
    {
        return -1;
    }

    int esq = altura(no->esq);
    int dir = altura(no->dir);

    return 1 + (esq > dir ? esq : dir);
}

int main(void)
{
    TNoA *raiz = NULL;
    int qt, value;

    printf("Digite o número de valores a serem inseridos na árvore: ");
    scanf("%i", &qt);

    printf("Digite os valores a serem inseridos:\n");
    for (int i = 0; i < qt; i++)
    {
        scanf("%i", &value);
        raiz = insereBalanceado(raiz, value);
    }

    printf("Árvore binária balanceada após a inserção:\n");
    imprime(raiz, 0);

    return 0;
}
