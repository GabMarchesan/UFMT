//Implementação dos métodos de ordenação
//aula 11


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
void shellSort(tipo_lista*);


//Inicializa lista
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
    printf("LISTA:\n\tCAPACIDADE = %d\n\tQUANTIDADE = %d\n\tLISTA = ", lst->capacidade, lst->qtd, lst->capacidade);
    for ( int i=0; i< lst->qtd; i++){
        printf("[%d] ", lst->lista[i]);
    }
    printf("\n");
}

    //metodo shell sort 
void shellSort (tipo_lista *lista1){

    int i, aux;
    int salto = (int)lista1->qtd/2;

    while (salto > 1){

        for (i = 0; i+salto < lista1->qtd; i++){

            if(lista1->lista[i] > lista1->lista[i+salto]){
                aux = lista1->lista[i];
                lista1->lista[i] = lista1->lista[i+salto];
                lista1->lista[i+salto] = aux;
            }
        }

        salto = (int) salto/2;
    }
}
int main(){
    int i;
    tipo_lista minha_lista;
    inicializaLista(&minha_lista, 10);

    srand(1);

    for (i=0; i<10; i++)
        insereLista(&minha_lista, rand()%100);

    imprimeLista(&minha_lista);

    printf("Ordenacao pelo metodo Shell sort:\n");
    shellSort(&minha_lista);
    imprimeLista(&minha_lista);
    
}