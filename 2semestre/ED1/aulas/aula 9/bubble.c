//Implementação dos métodos de ordenação
//aula 9


//Bibliotecas 
#include <stdio.h>
#include <stdlib.h>

//definiçao de estrutura 
struct est_lista{
    int *lista;
    int capacidade; // tamanho do vetor
    int qtd;
};

//Apelido 
typedef struct est_lista tipo_lista;

//prototipo de funções 
void inicializaLista(tipo_lista*, int); //int é o tamanho do vetor
void insereLista(tipo_lista*, int);
void imprimeLista(tipo_lista*);
void bubbleSort(tipo_lista*);




void inicializaLista(tipo_lista *lst, int tamanho){
    lst->capacidade = tamanho;
    lst->qtd = 0;
    //Alocação de memória do vetor
    lst->lista = (int*) malloc(sizeof(int)*tamanho);
}

//insere Lista
void insereLista(tipo_lista *lst, int valor){
    if (lst->qtd < lst->capacidade){
        lst->lista[lst->qtd++] = valor;
    } else {
        printf("[ATENCAO] lista estah cheia. Nenhum valor serah inserido.\n");
    }
}

//Imprime Lista
void imprimeLista(tipo_lista *lst){
    printf("LISTA:\n\tCAPACIDADE = %d\n\tQUANTIDADE = %d\n\tLISTA = [ ", lst->capacidade, lst->qtd, lst->capacidade);
    for ( int i=0; i< lst->qtd; i++){
        printf("%d,", lst->lista[i]);
    }
    printf("]\n");
}

//Bubble sort
void bubbleSort(tipo_lista *lst){
    int troca, aux, i;

    do{
        troca = 0;//flag usada para saber quando ocorre uam troca no bubble
        for ( i = 0; i < lst->qtd-1; i++){
            if ( lst->lista[i] > lst->lista[i+1]){
                aux = lst->lista[i];
                lst->lista[i]= lst->lista[i+1];
                lst->lista[i+1] = aux;
                troca=1;
            }
        } 
    } while (troca);
}


int main(){
    int i;
    tipo_lista minha_lista;
    inicializaLista(&minha_lista, 10);

    srand(1);

    for (i=0; i<10; i++)
        insereLista(&minha_lista, rand()%100);

    imprimeLista(&minha_lista);

    printf("Ordenacao pelo metodo Bubble sort:\n");
    bubbleSort(&minha_lista);
    imprimeLista(&minha_lista);
    
}