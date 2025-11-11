/**
 * UFMT - Ciencia da Computacao
 * Disciplina de ED 1
 *
 * @file ex19-lista_enc.h
 * @author Ivairton
 * @brief Implementação de LISTA com alocacao dinamica
 * @version 0.1
 * @date 2023-03-14
 */

#ifndef __EX19_LISTA_ENC__   // Verifica se modulo jah foi definido
#define __EX19_LISTA_ENC__   // Define nome de modulo

#include<stdio.h>
#include<stdlib.h>

//Definicao de estrutura
struct est_lista {
    int valor;
    struct est_lista *prox;
};
typedef struct est_lista tipo_lista;

//Prototipo das funcoes
void insereInicioLista(tipo_lista**, int);
void insereFimLista(tipo_lista**, int);
void inserePosLista(tipo_lista**, int, int);
int removeInicioLista(tipo_lista**);
int removeFimLista(tipo_lista**);
int removePosLista(tipo_lista**,int);
void imprimeLista(tipo_lista*);

#endif //__EX19_LISTA_ENC__