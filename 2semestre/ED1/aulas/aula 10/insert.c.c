//Implementação dos métodos de ordenação
//aula 10


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
void imprimeLista(tipo_lista);
void insertSort(tipo_lista*);


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
void imprimeLista(tipo_lista lst){
    printf("LISTA:\n\tCAPACIDADE = %d\n\tQUANTIDADE = %d\n\tLISTA = ", lst.capacidade, lst.qtd, lst.capacidade);
    for ( int i=0; i< lst.qtd; i++){
        printf("[%d] ", lst.lista[i]);
    }
    printf("\n");
}

//insert sort
void insertSort(tipo_lista *lst){
    tipo_lista lst_ordenada;
    inicializaLista(&lst_ordenada,lst->capacidade);

    int pos_lst, i;

    //Percorre cada valor da lista desordenada, inserindo na lista nova(ordenada)
    for(i=0; i<lst->qtd; i++){
        pos_lst = lst_ordenada.qtd;
        while((pos_lst > 0) && (lst->lista[i]<lst_ordenada.lista[pos_lst-1])){
            lst_ordenada.lista[pos_lst] = lst_ordenada.lista[pos_lst-1];
            pos_lst--;
        }
        lst_ordenada.lista[pos_lst] = lst->lista[i];
        lst_ordenada.qtd++;
    }

    for (i = 0; i != lst->qtd ; i++){
        lst->lista[i] = lst_ordenada.lista[i];
    }
}

//int main
int main(){
    int i;
    tipo_lista minha_lista;
    inicializaLista(&minha_lista, 10);

    srand(1);

    for (i=0; i<10; i++)
        insereLista(&minha_lista, rand()%100);

    imprimeLista(minha_lista);

    printf("Ordenacao pelo metodo Insert sort:\n");
    insertSort(&minha_lista);
    imprimeLista(minha_lista);
    
}