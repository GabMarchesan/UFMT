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
void quickSort(tipo_lista*);
void quick(int*,int, int);


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

//Quick sor
void quickSort(tipo_lista *lst){
    //chama funcao recurciva, com os parametros vetor, inicio e fim
    quick(lst->lista, 0 , lst->qtd-1);
}

//void quick
void quick(int *vetor, int inicio, int fim){
    int i, j, meio, pivo, aux;

    //escolhe o elemento do meio
    meio = (int) (fim-inicio)/2;
    pivo = vetor[meio];

    i = inicio;
    j = fim;

    //verifica se os elementos aa esquerda sao menores, e os aa direita sao maiores, trocando quando necessario ateh que i e j se encontrem
    while(i < j){
        while (vetor[i] < pivo){
            i++;
        }
        while (vetor[j] > pivo){
            j--;
        }
        if (i < j){ //Realiza troca de valores
            aux = vetor[i];
            vetor[i] = vetor[j];
            vetor[j] = aux;
        }
    }
    //Realiza a chamanda recursiva para cada parte(antes e depois)
    if ( j-1 > inicio){ //chamda recursiva para 1a parte(antes do pivo)
        quick(vetor, inicio, j-1);
    }
    //chamada recursiva para a 2a parte (depois do pivo)
    if ( i+1 < fim){
       quick(vetor, i+1, fim); 
    }
}

//main
int main(){
    int i;
    tipo_lista minha_lista;
    inicializaLista(&minha_lista, 10);

    srand(1);

    for (i=0; i<10; i++)
        insereLista(&minha_lista, rand()%100);

    imprimeLista(&minha_lista);

    printf("Ordenacao pelo metodo Quick sort:\n");
    quickSort(&minha_lista);
    imprimeLista(&minha_lista);
    
}