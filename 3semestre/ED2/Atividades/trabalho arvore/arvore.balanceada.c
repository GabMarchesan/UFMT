#include <stdio.h>
#include <stdlib.h>

typedef struct sNoA {
    int chave;
    struct sNoA *esq;
    struct sNoA *dir;
    // Se necessário pode adicionar elementos na struct
} TNoA;

TNoA *rotacaoDireita(TNoA *no) {
    TNoA *aux = no->esq;
    no->esq = aux->dir;
    aux->dir = no;
    return aux;
}

TNoA *rotacaoEsquerda(TNoA *no) {
    TNoA *aux = no->dir;
    no->dir = aux->esq;
    aux->esq = no;
    return aux;
}

TNoA *rotacaoDuplaDireita(TNoA *no) {
    no->esq = rotacaoEsquerda(no->esq);
    return rotacaoDireita(no);
}

TNoA *rotacaoDuplaEsquerda(TNoA *no) {
    no->dir = rotacaoDireita(no->dir);
    return rotacaoEsquerda(no);
}

TNoA *busca(TNoA *no, int chave) {
    if (no == NULL || no->chave == chave) {
        return no;
    }

    if (chave < no->chave) {
        no->esq = busca(no->esq, chave);
        return rotacaoDireita(no);
    }

    no->dir = busca(no->dir, chave);
    return rotacaoEsquerda(no);
}

TNoA *insere(TNoA *no, int chave) {
    if (no == NULL) {
        TNoA *novo = (TNoA *)malloc(sizeof(TNoA));
        novo->chave = chave;
        novo->esq = novo->dir = NULL;
        return novo;
    }

    if (chave < no->chave) {
        no->esq = insere(no->esq, chave);
        if (no->esq->chave == chave) {
            return rotacaoDireita(no);
        } else if (chave < no->esq->chave) {
            return rotacaoDuplaDireita(no);
        }
        return no;
    }

    no->dir = insere(no->dir, chave);
    if (no->dir->chave == chave) {
        return rotacaoEsquerda(no);
    } else if (chave > no->dir->chave) {
        return rotacaoDuplaEsquerda(no);
    }
    return no;
}

void imprime(TNoA *nodo, int tab) {
    if (nodo != NULL) {
        imprime(nodo->esq, tab + 2);
        for (int i = 0; i < tab; i++) {
            printf(" ");
        }
        printf("%d\n", nodo->chave);
        imprime(nodo->dir, tab + 2);
    }
}

TNoA *cria(void) {
    return NULL;
}

int main(void) {
    TNoA *raiz;
    raiz = cria();
    int qt, value, x;

    scanf("%i", &qt);

    for (int i = 0; i < qt; i++) {
        scanf("%i", &value);
        raiz = insere(raiz, value);
    }

    scanf("%i", &x);
    raiz = busca(raiz, x);
    imprime(raiz, 0);

    return 0;
}