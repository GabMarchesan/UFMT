//Bibliotecas
#include <stdio.h>
#include <stdlib.h>

//estruct tipo lista
typedef struct lista{
    int info;
    struct lista* prox;
}Tlista;

//Cria uma lista
Tlista* cria_lista(void){
    return NULL;
}

//Insere incio lista
Tlista* insere_inicio(Tlista* lst, int i){
    Tlista* novo = (Tlista*) malloc(sizeof(Tlista));
    novo->info = i;
    novo->prox = lst;
    return novo;
}

//Insere fim Lista
Tlista* insere_fim (Tlista* li, int i){
    Tlista* novo = (Tlista*) malloc(sizeof(Tlista));
    novo->info = i;
    novo->prox = NULL;
    Tlista* p = li;
    Tlista* q = li;
    while (p != NULL) { // Encontra o ultimo elemento 
        q = p; // q recebe o valor de p onde está apontando
        p = p->prox; // p aponta para nulo
    }
    if (q != NULL) { // Se a lista original não estiver vazia
        q->prox = novo;
    } else {
        li = novo;
    }
    return li;
}

//insere fim recursivo
Tlista* insere_fim_recursivo(Tlista* lst, int i){
    if(lst == NULL || lst->prox == NULL){
        Tlista *novo = (Tlista*) malloc(sizeof(Tlista));
        novo->info; 
    }
}

//insere lista ordenada
Tlista* insere_ordenada(Tlista* lst, int i){
    Tlista* novo;
    //Cria um novo elemento
    novo = (Tlista*) malloc(sizeof(Tlista));
    novo->info = i;
    Tlista* p = lst; //ponteiro para percorrer a lista
    if(p == NULL){ //lista vazia -- insere inicio
        novo->prox = NULL;
        lst = novo;
    } else if (p->info > i){ // insere inicio da lista
        novo->prox = lst;
        lst = novo;
    } else {  //procura posição dde inserção
        while (p->prox != NULL && p->prox->info < i){
            p = p->prox;
        }
        novo->prox = p->prox;
        p->prox = novo;
    }
    return lst;
}

//Imprime elementos da lista
void imprime_lista(Tlista* lst){
    Tlista* p;
    for(p = lst; p != NULL; p = p->prox){
        printf("Info = %d\n", p->info);
    }
}

//int main
int main(void){
    Tlista* lst;

    //cria uma lista vazia
    lst = cria_no(1);

    //insere elementos no inicio da lista
    lst = insere_inicio(lst,2);
    lst = insere_inicio(lst,3);
    lst = insere_inicio(lst,5);
    lst = insere_inicio(lst,4);

    imprime(lst);

    return lst;
}