//Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Definicao de estrutura
struct est_heap{
    int *heap;
    int quant;
    int capacidade;
};

//apelido 
typedef struct est_heap tipo_heap;

//Prototipo das funcoes
void inicializaHeap(tipo_heap*, int);
void insereHeapMax(tipo_heap*, int);
int removeHeap(tipo_heap*);
void sobeHeapMax(tipo_heap*, int);
void desceHeapMax(tipo_heap*, int);
int filhoEsq(int);
int filhoDir(int);
int pai(int);
void imprimeHeap(tipo_heap);
void heapSort(int);


//Funcao que inicializar o heap, aloca o vetor e inicializa
void inicializarHeap(tipo_heap *hp, int capacidade){
    hp->capacidade = capacidade;
    hp->quant = 0;
    hp->heap = (int*)malloc(sizeof(int)*capacidade);
}

//Funcao que insere um novo valor no heap e verifica sua 
void insereHeapMax(tipo_heap *hp, int valor){
    //verifica se o hep tem espaco disponivel
    if (hp->quant < hp->capacidade){
        hp->heap[hp->quant++] = valor; //insere novo valor ao final
        //sobe com o valor para garantir o heap maximo
        sobeHeapMax(hp,hp-> quant-1);
    }
}

//Funcao que remove o valor da raiz do heap e verifica sua
int removeHeapMax(tipo_heap *hp){
    //verifica se heap tem valor
    if(hp->quant > 0){
        int valor;
        valor = hp->heap[0];
        hp->heap[0] = hp->heap[--hp->quant];
        //chama funcao que reestrutura o heap para garantir sua 
        desceHeapMax(hp, 0);
        return valor;
    } else{
        return -1; //caso heap vazio
    }
}

//funcao que calcula o indece do filho esquerdo
int filhoEsq(int i){
    return 2*i+1;
}

//Funcao que calcula o indece do filho direito
int filhoDir(int i){
    return 2*i+2;
}

//Funcao que calcula o indece pai
int pai(int i){
    return (int) (i-1) / 2;
}

//Funcao que sobe com o valor, de modo que ele alcance sua
void sobeHeapMax(tipo_heap *hp, int i){
    int aux;
    //Enquanto houver pai e o valor do pai for menor
    if((i > 0) && ( hp->heap[pai(i)] < hp->heap[i] ) ){
        aux = hp->heap[pai(i)];
        hp->heap[pai(i)] = hp->heap[i];
        hp->heap[i] = aux;
        sobeHeapMax(hp, pai(i));
    }
}

void desdeHeapMax(tipo_heap *hp, int i){
    int aux,  idx_maior;
    //Enquanto houver filho e o valor de um dos filhos for maior

    //Verifica se tem os 2 filhos
    if(filhoDir(i) < hp->quant){
        //identifica o filho maior
        if(hp->heap[filhoEsq(i)] > hp->heap[filhoDir(i)])
            idx_maior = filhoEsq(i);
        else {
            idx_maior = filhoDir(i);
        }
        //Verifica se o valor precisa descer
        if(hp->heap[idx_maior] > hp->heap[i]){
            aux = hp->heap[idx_maior];
            hp->heap[idx_maior] = hp->heap[i];
            hp->heap[i] = aux;
            desceHeapMax(hp, idx_maior); 
        }
    } else if (filhoEsq(i) < hp->quant) {
            idx_maior = filhoDir(i);
            aux = hp->heap[idx_maior];
            hp->heap[idx_maior] = hp->heap[i];
            hp->heap[i] = aux;
            desceHeapMax(hp, idx_maior);
    }
}

void imprimeHeap(tipo_heap hp){
    
    int i;
    for (i=0; i<hp.quant; i++){
        printf("[%d] ", hp.heap[i]);
    }

    printf("\n");
}


int main() {

    //testa o heap
    tipo_heap meu_heap;
    inicializaHeap(&meu_heap, 10);

    insereHeapMax(&meu_heap, 7);
    insereHeapMax(&meu_heap, 3);
    insereHeapMax(&meu_heap, 6);
    insereHeapMax(&meu_heap, 2);
    insereHeapMax(&meu_heap, 9);
    insereHeapMax(&meu_heap, 10);
    insereHeapMax(&meu_heap, 5);
    insereHeapMax(&meu_heap, 4);

    imprimeHeap(meu_heap);
}