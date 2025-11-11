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
 *  Teste de lista dinamica com capacidade de 10.000
 * 
 **/

//Biblioptecas
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<math.h>

//Definicao de registros 
typedef struct no{
    int valor;
    struct no *proximo;
}No;

typedef struct{
    No *inicio; //priemiro ponteiro da lista
    int tam; //tamanho da lista
}Lista;

//Prototipo das funções que irão manipular a estrutura lista
void inicializarLista(Lista *lista);
void insereInicioLista(Lista*, int);
void insereFimLista(Lista*, int);
void inserePosicaoLista(Lista*, int,int);
void imprimeLista(Lista);

//Funcao Inicializar lista 
void inicializarLista(Lista *lista){
    lista->inicio =NULL;
    lista->tam = 0;
}

//Funcao insere Inicio Lista
void insereInicioLista(Lista *lista, int num){
    No *novo = malloc(sizeof(No));

    if(novo){
        novo->valor = num;
        novo->proximo = lista->inicio;
        lista->inicio = novo;
        lista->tam++; 
    } else {
        printf("[ERRO] , ao alocar memoria!\n");
    }
}

//Funcao insere fim Lista
void insereFimLista(Lista *lista, int num){
    No *aux, *novo = malloc(sizeof(No));

    if(novo){
        novo->valor = num;
        novo->proximo = NULL;

        //lista eh vazia
        if(lista->inicio == NULL){
            lista->inicio = novo; //significa que a lista estah vazia
        } else {
            aux = lista->inicio;
            while (aux->proximo)
               aux = aux->proximo;
            aux->proximo = novo;
        }
        lista->tam++;
    } else {
        printf("[ERRO] , ao alocar memoria!\n");
    }
}

//Funcao insere posicao Lista
void inserePosicaoLista(Lista *lista, int num, int ant){
    No *aux, *novo = malloc(sizeof(No));

    if(novo){
        novo->valor = num;

        if(lista->inicio == NULL){
            novo->proximo = NULL;
            lista->inicio = novo;
        } else {
            aux = lista->inicio;
            while (aux->valor != ant && aux->proximo)
               aux = aux->proximo;
            novo->proximo = aux->proximo;
            aux->proximo = novo;
        }
        lista ->tam++;
    } else {
        printf("[ERRO] , ao alocar memoria!\n");
    }
}

//Funcao imprime
void imprimeLista(Lista lista){
    No *no = lista.inicio;
    printf("Lista [");
    while (no){
        printf("%d,", no->valor);
        no = no->proximo;
    }
    printf("]\n\nElementos =%d\n", lista.tam);

}



