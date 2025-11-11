//Bibliotecas
#include <stdio.h>
#include <stdlib.h>

//Cores
#define RED 0
#define BLACK 1

//estrutura
typedef struct sNoA{
    int chave;
    int cor;
    struct sNoA *esq;
    struct sNoA *dir;
    struct sNoA *pai;
}TNoa;

//função da cor
int cor(TNoA *no){
    if(no == NULL){
        return BLACK;
    } else {
        return no->cor;
    }
}

//funcao rotação a esquerda
TNoA *rotacao_esquerda(TNoA *pt){
    TnoA *ptu;

    ptu = pt->dir;
    pt->dir = ptu->esq;
    if(ptu->esq != NULL){
        ptu->esq->pai = pt;
    }

    ptu->pai = pt->pai;
    ptu->esq = pt;
    pt->pai = ptu;

    pt = ptu;

    return pt;
}

//função insere
TNoa *insere_fixup(TNoA *T, TNoA *x){
    TNoA *tio;
    while(cor(x->pai) == RED){
        //Verifica se o pai de x é filho a esquerda do avo de x
        if(x->pai == x->pai->pai->esq){
            tio = (x->pai)->pai->dir;

            //se o tio de x for vermelho
            if(cor(tio) == RED){
                //realiza a recolaração do pai, tio e avo
                x->pai->cor = BLACK;
                tio->cor = BLACK;
                x->pai->pai->cor = RED;
                x = x->pai->pai;
            } else { //entao o tio é preto
                //Verificar se o x é filho a direita
                if(x == x->pai->dir){
                    x = x->pai;
                    x = rotacao_esquerda(x);
                } else {
                    .
                }
            }
        }
    }
}