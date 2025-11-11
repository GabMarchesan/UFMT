//Implementcao da tabela hash
//dia 27/04/2023
//ED1

//Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//Definicao da estrutura da tabela hash

//definicao do noh que guardarah os dados
struct est_dado{
    int chave;
    int dado;
    struct est_dado *prox;
};

//apelido
typedef struct est_dado tipo_dado;

//Definicao da estrutura da tabela hash
struct est_tab_hash{
    tipo_dado **tabela;
    int capacidade;
};

//apelido
typedef struct est_tab_hash tipo_tab_hash;

//Prototipo das funcoes
void inicializaTabHash(tipo_tab_hash*, int);
int funcHash(tipo_tab_hash, int);
void insereTabHash(tipo_tab_hash*, int, int);
tipo_dado *buscaTabHash(tipo_tab_hash, int);
void imprimeTabHash(tipo_tab_hash);

//funcao que inicializa a estrutura da tabela HASH
void inicializaTabHash(tipo_tab_hash *tab_hash, int capacidade){
    tab_hash->capacidade =  capacidade;
    tab_hash->tabela = (tipo_dado**) malloc(sizeof(tipo_dado*)*capacidade);
    //inicializa cada campo do vvetor(ponteiro) com nulo
    int i;
    for (i=0; i<capacidade; i++){
        tab_hash->tabela[i] = NULL;
    }
}

//Funcao Hash que recebe uma chave e determina o indice
int funcHash(tipo_tab_hash tab_hash, int chave){
    //Metodo mais simples do calculo do indice, consiste do resto da divisao da chave pela capacidade da tabela
    return(int) chave % tab_hash.capacidade;
}

//Funcao que insere um novo valor na tabela Hash.
void insertTabHash(tipo_tab_hash *tab_hash, int chave, int dado){
    int idx_tab;
    tipo_dado *novo_dado;

    //Identifica a posicao onde o dado deve ser armazenado
    idx_tab = funcHash(*tab_hash, chave);

    //aloca um novo noh na memoria
    novo_dado = (tipo_dado*) malloc(sizeof(tipo_dado));
    novo_dado->chave = chave;
    novo_dado->dado = dado; //armazena o valor
    //Faz com que o proximo do novo noh aponte para a lista se a posicao estiver vazia, irah apontar pa5ra NULL
    //Se a posicao tiver valor(colisao), insere no inicio
    novo_dado->prox = tab_hash->tabela[idx_tab];
    tab_hash->tabela[idx_tab] = novo_dado;
}

//Funcao que busca pelo dado a partir da chave
tipo_dado *buscaTabHash(tipo_tab_hash tab_hash, int chave){
    int idx_dado;
    tipo_dado *aux;

    idx_dado = funcHash(tab_hash, chave);
    //Verifica se a posicao na tabela estah vazia
    if(tab_hash.tabela[idx_dado] == NULL){
        return NULL;
    } else { //Se a posicao tem dado, busca pela chave
        aux = tab_hash.tabela[idx_dado];
        while(aux != NULL){
            if (aux->chave == chave){
                return aux;
            } else {
                aux = aux->prox;
            }
        }
        return NULL;
    }
}

//Funcao para debugacao, imprimindo a tabela hash
void imprimeTabHash(tipo_tab_hash tab_hash){
    int i;
    char string[100], str_aux[100];
    tipo_dado *aux;
    printf("Tabela HASH:\n\tCapacidade da tabela = %d\n\tTabela: [", );
    for (i=0; i<tab_hash.capacidade; i++){
        if(tab_hash.tabela[i] == NULL){
            printf("* ");
        } else {
            aux = tab_hash.tabela[i];
            sprintf(string, "}");
            while(aux != NULL){
                sprintf(str_aux, "%d%d",aux->chave, aux->dado);
                stract(string, str_aux);
                aux = aux->prox;
            }
            stract(string,"}");
            printf("%s",string);
        }
    }
    printf("]\n");
}

//int main
int main(){
    tipo_tab_hash minha_tab_hash;

    inicializaTabHash(&minha_tab_hash, 10);

    insereTabHash(&minha_tab_hash, 7, 7);
    insereTabHash(&minha_tab_hash, 3, 3);
    insereTabHash(&minha_tab_hash, 9, 9);
    insereTabHash(&minha_tab_hash, 17, 17);
    insereTabHash(&minha_tab_hash, 33, 33);

    imprimeTabHash(minha_tab_hash);
    tipo_dado *meu_dado;
    meu_dado

    return 1;
}



