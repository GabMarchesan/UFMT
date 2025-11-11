
/**
 *
 * UFMT - Ciencia da Computacao
 * 
 * ED1 - Prof. Ivairton
 * 
 * Aluno: Herrison Batista de Meneses Junior / Gabriel Gomes Marchesan
 * 
 * Data: 19/03/2023
 * 
 * 
 *  Teste de lista estática com capacidade de 10.000
 * 
 **/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<math.h>
#ifndef lista_D_enc
#define lista_D_enc

typedef struct no{
    int valor;
    struct no *proximo;
    struct no *anterior;
}No;

void insereFim(No **, int );
void inserePos(No **, int , int );
void insereInicio(No **, int );
void imprimeLista(No *);

void insereInicio(No **lista, int num){
    No *novo = malloc(sizeof(No));

    if(novo){
        novo->valor = num;
        novo->proximo = *lista;
        novo->anterior = NULL;
        if(*lista)
            (*lista)->anterior = novo;
        *lista = novo;
    }
    else
        printf("Erro ao alocar memoria!\n");
}

void insereFim(No **lista, int num){
    No *aux, *novo = malloc(sizeof(No));

    if(novo){
        novo->valor = num;
        novo->proximo = NULL;

        if(*lista == NULL){
            *lista = novo;
            novo->anterior = NULL;
        }
        else{
            aux = *lista;
            while(aux->proximo)
                aux = aux->proximo;
            aux->proximo = novo;
            novo->anterior = aux;
        }
    }
    else
        printf("Erro ao alocar memoria!\n");
}

void inserePos(No **lista, int num, int pos){
    No *aux, *novo = malloc(sizeof(No));

    if(novo){
        novo->valor = num;
        if(*lista == NULL){
            novo->proximo = NULL;
            novo->anterior = NULL;
            *lista = novo;
        }
        else{
            aux = *lista;
            while(aux->valor != pos && aux->proximo)
                aux = aux->proximo;
            novo->proximo = aux->proximo;
            if(aux->proximo)
                aux->proximo->anterior = novo;
            novo->anterior = aux;
            aux->proximo = novo;
        }
    }
    else
        printf("Erro ao alocar memoria!\n");
}




void imprimeLista(No *no){
    int cont = 0;
    printf("\n\tLista: ");
    while(no){
        printf("%d ", no->valor);
        no = no->proximo;
        cont++;
    }
    printf("\n\n");
    printf("\nelementos: %d ", cont);
}



#endif