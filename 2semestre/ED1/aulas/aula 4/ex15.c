//Bibliotecas
#include <stdlib.h>
#include <stdio.h>

//Definição de constate(guarda só 10 valores)
#define N 10

//Definicao de um novo tipo de dados(registro)

struct st_fila{
    int fila[N];
    int contador;
};

//Apelido 
typedef struct st_fila tipo_fila;

//Prototipação de funções que manipulão a estrutura(implementando por referencia)
void insere_fila(tipo_fila*,int);
int remove_fila(tipo_fila*); //usa o ponteiro,pois vai editar os dados da fila
int verifica_fila_cheia(tipo_fila); //Não preica de ponteiro, pois só via exibir os dados da fila
int verifica_fila_vazia(tipo_fila);
int qtd_elementos_fila(tipo_fila);
void imprime_fila(tipo_fila);

//Implementação das fuções 

void insere_fila(tipo_fila *fl, int valor){
    if ( ! verifica_fila_cheia(*fl)){ // ! é negação
        fl ->fila[ fl->contador++] = valor;
    } else {
        printf("[ERRO] a estrutura da fila estah cheia! nada serah feito!\n");
    }
}

//Função remove fila
int remove_fila(tipo_fila *fl){
    int i, valor;
    if( ! verifica_fila_vazia(*fl)){
        valor = fl->fila[0];
        for ( i=0;i<(fl->contador-1);i++)
           fl->fila[i] = fl->fila[i+1];
        fl->contador--;
    } else {
        valor = 1;
        printf("[ERRO] tentaiva de remover um valor de uma fila vazia\n");
    }
    return valor;
}

//Função fila cheia
int verifica_fila_cheia(tipo_fila fl){
    if( fl.contador < N)
      return 0;
    else 
      return 1;
}

//Função fila vazia 
int verifica_fila_vazia(tipo_fila fl){
    if (fl.contador == 0)
      return 1;
    else 
      return 0;
}

//Função imprime 
void imprime_fila(tipo_fila fl) {
    int i;
    printf("Estrutura FILA:\nQuantidade de elementos = %d, com capacidade de %d\n[ ", fl.contador, N);
    for ( i=0; i<fl.contador; i++)
        printf("%d ", fl.fila[i]);
    printf("]\n");
}

//int main

int main(){
    tipo_fila minha_fila;
    minha_fila.contador = 0;

    insere_fila(&minha_fila, 8);
    insere_fila(&minha_fila, 6);
    insere_fila(&minha_fila, 7);
    insere_fila(&minha_fila, 4);
    insere_fila(&minha_fila, 4);

    imprime_fila(minha_fila);

    printf("Removi o valor %d da fila!\n", remove_fila(&minha_fila));

    imprime_fila(minha_fila);


    return 0;
}
