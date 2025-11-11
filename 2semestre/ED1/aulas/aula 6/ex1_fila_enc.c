//Bibliotecas
#include <stdlib.h>
#include <stdio.h>

//Definição do tipo
struct estrutura_no{ //Estrutura encadeada
    int valor;
    struct estrutura_no *prox; //aponta para o mesmo tipo, porém para próxima posição
};

typedef struct estrutura_no tipo_no;

//Prototipo das funções
void insereFilaEnc(tipo_no**, int);
int removeFilaEnc(tipo_no**);
void imprimeFilaEnc(tipo_no*);

//Função Insere fila encadeada 
void insereFilaEnc(tipo_no **fila, int valor){
    tipo_no *novo_no, *aux;

    //Aloca memória para o novo nó
    novo_no = (tipo_no*) malloc(sizeof(tipo_no));
    novo_no->valor = valor; //armazena novo valor na estrutura
    novo_no->prox = NULL; //Inicializa o ponteiro proximo do nó com o nulo

    //verifica se a fila esta vazia
    if((*fila) == NULL){
       (*fila) = novo_no;
    } else {
        //Percorre a estrutura encadeada até encontrar o ultimo nó
        aux = (*fila);
        while (aux->prox != NULL)
            aux = aux->prox;
        aux->prox = novo_no;
    }
}

//Função remove fila

int removeFilaEnc(tipo_no **fila){
    int valor;
    tipo_no *aux;

    //verifica se fila é vazia
    if ( (*fila) == NULL) {
        return -1; //Retorna -1 apra sinalizar erro
    } else {
        if((*fila)->prox == NULL){
            valor = (*fila)->valor;
            free(*fila);
            (*fila) = NULL; //Atualiza o ponteiro da fila para nulo

        } else {

            aux = (*fila);
            (*fila) = (*fila)->prox; //atualiza o ponteiro da fila
            valor = aux -> valor;
            free(aux);
        }

    return valor;
    }
}

//Função imprime fila

void imprimeFilaEnc(tipo_no *fl){
    printf("fila: [");
    while (fl != NULL){
       printf("%d ->", fl-> valor);
       fl = fl->prox;
    }
    printf("-> NULO ]\n");
}

//para teste de estrutura
int main(int argc, char *argv[]){
    tipo_no *minha_fila;

    minha_fila = NULL;

    insereFilaEnc(&minha_fila, 7);
    insereFilaEnc(&minha_fila, 1);
    insereFilaEnc(&minha_fila, 9);
    insereFilaEnc(&minha_fila, 5);
    insereFilaEnc(&minha_fila, 3);

    imprimeFilaEnc(minha_fila);

    printf("Valor removido da fila =%d\n", removeFilaEnc(&minha_fila));
    printf("Valor removido da fila =%d\n", removeFilaEnc(&minha_fila));
    printf("Valor removido da fila =%d\n", removeFilaEnc(&minha_fila));

    imprimeFilaEnc(minha_fila);

    return 1;
}