//Bibliotecas
#include <stdlib.h>
#include <stdio.h>

//Definição de constante
#define N 10

//Definição de tipo 
struct str_lista{
    int lista[N];
    int contador;
};

//apelido
typedef struct str_lista tipo_lista;

//Prototipo das funções que irão manipular a estrutura lista
void insereInicioLista(tipo_lista*,int);
void insereFimLista(tipo_lista*,int);
void inserePosicaoLista(tipo_lista*,int,int);
int removoInicioLista(tipo_lista*);
int removoFimLista(tipo_lista*);
int removePosicaoLista(tipo_lista*);
int removeValorLista(tipo_lista*);
int buscaValorLista(tipo_lista*);
int verificaListaCheia(tipo_lista);
int verificaListaVazia(tipo_lista);
int buscaMaiorValorLista(tipo_lista);
int buscaMenorValorlista(tipo_lista);
void imprimeLista(tipo_lista);

//Implementação de função

//função lista cheia
int verificaListaCheia(tipo_lista lista){
    if (lista.contador == N )
      return 1;
    else 
      return 0;
}

//função lista vazia
int verificaListaVazia(tipo_lista lista){
    if (lista.contador < 0)
      return 1;
    else 
      return 0;
    
}

//Função Insere inicio lista
void insereInicioLista(tipo_lista *lista, int valor){
    if(verificaListaCheia == 1){
      printf("[ERRO] a lista estah cheia\n");
        return 0;
    } else { 
      lista->lista[lista->contador]=valor;
    }
}

//Função Insere fim lista

void insereFimLista(tipo_lista *lista, int valor){
    if(verificaListaVazia == 1){
        printf("[ERRO] a lista estah vazia\n");
          return 0;
    } else {
        lista->lista[lista->contador]=valor;
    }
}

//
