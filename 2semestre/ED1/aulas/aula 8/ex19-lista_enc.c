// Implementacao das funcoes

#include "ex19-lista_enc.h"

/**
 * @brief Funcao que insere no inicio da lista encadeada.
 * 
 * @param lst Lista por referencia.
 * @param vl Valor a ser inserido.
 */
void insereInicioLista(tipo_lista **lst, int vl) {
    tipo_lista *novo_no;
    novo_no = (tipo_lista*) malloc(sizeof(tipo_lista));
    if (novo_no != NULL) {
        novo_no->prox = (*lst);
        (*lst) = novo_no;
    } else {
        printf("[ERRO] Falha na alocacao de memoria para o novo noh.\n");
    }
}

/**
 * @brief Funcao que insere no fim da estrutura encadeada da lista.
 * 
 * @param lst Referencia para a estrutura da lista.
 * @param vl Valor a ser inserido.
 */
void insereFimLista(tipo_lista **lst, int vl) {
    tipo_lista *aux;
    
    //Verifica se lista estah vazia
    if ((*lst) == NULL) {
        (*lst) = (tipo_lista*) malloc(sizeof(tipo_lista));
        (*lst)->prox = NULL;
        (*lst)->valor = vl;
    } else {
        aux = (*lst);
        while (aux->prox != NULL)
            aux = aux->prox;
        aux->prox = (tipo_lista*) malloc(sizeof(tipo_lista));
        aux->prox->prox = NULL;
        aux->prox->valor = vl;
    }
}

/**
 * @brief Funcao que insere numa posicao especifica da lista encadeada.
 * 
 * @param lst Referencia para a lista.
 * @param vl Valor a ser inserido na lista.
 * @param pos Posicao onde o valor deve ser inserido (indice).
 */
void inserePosLista(tipo_lista **lst, int vl, int pos) {
    int cont;
    tipo_lista *aux, *ant, *novo_no;

    if (pos == 0) {
        insereInicioLista(lst, vl);
    } else if (pos > 0) {
        cont = 0;
        aux = (*lst);
        while ( (aux != NULL) && (cont < pos) ) {
            ant = aux;
            aux = aux->prox;
            cont++;
        }
        //Verifica se a posicao correta foi alcancada
        if ( (cont == pos)) {
            novo_no = (tipo_lista*) malloc(sizeof(tipo_lista));
            novo_no->prox = aux;
            novo_no->valor = vl;
            ant->prox = novo_no;
        }
    }
}

/**
 * @brief Funcao que remove o prmeiro no da lista.
 * 
 * @param lst Referencia para a lista.
 * @return int Valor removido.
 */
int removeInicioLista(tipo_lista **lst) {
    int valor;
    tipo_lista *aux;

    if ((*lst) == NULL) {
        printf("[ERRO] Lista vazia, nao eh possivel remover valor.\n");
        return -1;
    } else {
        aux = (*lst);
        valor = aux->valor;
        (*lst) = (*lst)->prox;
        free(aux);
        return valor;
    }
}

/**
 * @brief Funcao que remove o ultimo valor da estrutura.
 * 
 * @param lst Referencia a lista.
 * @return int Valor removido.
 */
int removeFimLista(tipo_lista **lst) {
    int valor;
    tipo_lista *aux, *ant;
    if ((*lst) != NULL) { //Verifica se lista nao estah vazia
        if ( (*lst)->prox == NULL) { //Verifica se lista tam unico no
            valor = (*lst)->valor;
            free( (*lst) );
            (*lst) = NULL;
            return valor;
        } else { //Percorre a lista para encontrar o ultimo no
            aux = (*lst);
            while (aux->prox != NULL) {
                ant = aux;
                aux = aux->prox;
            }
            valor = aux->valor;
            ant->prox = NULL;
            free(aux);
            return valor;
        }
    }
}

/**
 * @brief Funcao que remove um valor de uma posicao especifica da lista.
 * 
 * @param lst Referencia para a lista.
 * @param pos Posicao do noh a ser removido.
 * @return int Valor removido.
 */
int removePosLista(tipo_lista **lst, int pos) {
    int valor, cont;
    tipo_lista *aux, *ant;

    if (pos == 0) {
        return removeInicioLista(lst);
    } else {
        cont = 0;
        if ((*lst) != NULL) {
            aux  = (*lst);
            while ((aux != NULL) && (cont < pos)) {
                ant = aux;
                aux = aux->prox;
            }
            if (aux != NULL) {
                valor = aux->valor;
                ant->prox = aux->prox;
                free(aux);
                return valor;
            }
        }
    }
}


void imprimeLista(tipo_lista *lst) {
    printf("LISTA:\n ->");
    while (lst != NULL) {
        printf(" %d ->", lst->valor);
        lst = lst->prox;
    }
    printf("[NULO]\n");
}