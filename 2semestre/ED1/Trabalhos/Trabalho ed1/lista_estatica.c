/**
 *
 * UFMT - Ciencia da Computacao
 * 
 * ED1 - Prof. Ivairton
 * 
 * Aluno: Herrison Batista de Meneses Junior // Gabriel Gomes Marchesan
 * 
 * Data: 17/03/2023
 * 
 * 
 *  Teste de lista estática com capacidade de 10.000
 * 
 **/
#ifndef lista_estatica
#define lista_estatica

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<math.h>

#define tam 10000

struct strlist {
    int *listaq; 
    int elementos;
    int TAM; 
};

typedef struct strlist tipoLista;

int inicializaLista(tipoLista*, int);
int listaCheia(tipoLista);
int listaVazia(tipoLista);
int insereInicio(tipoLista*, int);
int insereFim(tipoLista*, int);
int inserePos(tipoLista*, int, int);
void imprimeLista(tipoLista);


// Inicializa a lista, alocando memoria para o vetor e inicializando
// a variavel de controle da quantidade dos dados.
int inicializaLista(tipoLista* l, int tamanho) {
    
    int i;
    l->listaq = (int*) malloc( sizeof(int)*tamanho );
    if (l->listaq != NULL ){
        l->TAM = tamanho;
        l->elementos = 0;
        
        for (i=0; i<l->TAM; i++) {
            l->listaq[i] = 0;
        }
        return 1;
    }
    return 0; 
}


int listaCheia(tipoLista l) {
    if ( l.elementos < l.TAM ) { // verifica o número de elementos é menor que o tamanho da lista 
        return 0;
    } else {
        return 1;
    }
}

int listaVazia(tipoLista l) {
    if ( l.elementos > 0 ) { // verifica se existe algo na lista
        return 0;
    } else {
        return 1;
    }
}

int insereFim(tipoLista* l, int valor) {
    if ( ! listaCheia (*l) ) {
        l->listaq[l->elementos++] = valor; // insere o valor de parametro no fim da lista
        return 1;
    }
    return 0;
}

int insereInicio(tipoLista *l, int valor) {
    if ( ! listaCheia(*l) ) {
        int i;
        for (i = l->elementos; i > 0; i-- ) {
            l->listaq[i] = l->listaq[i-1];
        }
        l->listaq[0] = valor;
        l->elementos++;
        return 1;
    }
    return 0;
}

int inserePos(tipoLista* l, int valor, int pos) {
    if ( ! listaCheia(*l) ) {
        if ( (pos >= 0) && (pos < l->TAM) && (pos <= l->elementos) ) {
            int i;
            for (i = l->elementos; i > pos; i-- ) {
                l->listaq[i] = l->listaq[i-1];
            }
            l->listaq[pos] = valor;
            l->elementos++;
            return 1;
        }
    }
    return 0;
}




void imprimeLista(tipoLista l) {
    int i;
    printf("Lista: [");
    for (i = 0; i < l.elementos; i++) {
        printf("%d,", l.listaq[i]);
    }
    printf("]\n\nelementos = %d\n", l.elementos);
}
#endif