
/**
 *
 * UFMT - Ciencia da Computacao
 * 
 * ED1 - Prof. Ivairton
 * 
 * Aluno: Herrison Batista de Meneses Junior / Gabriel Gomes Marchesan
 * 
 * Data: 19/03/2023
 * 
 * 
 *  Teste de lista estática com capacidade de 10.000
 * 
 **/

// Inclusao de bibliotecas
#include<stdio.h>
#include<stdlib.h>
// Inclusao de bibliotecas locais
#include "lista_D_enc.c"



int main(){

    No *removido, *lista = NULL;

int pos, ran;  // pos eh a variavel que armazena valor da posicao a ser inserida
//ran eh a variavel que armazena os numeros a serem inseridos

     clock_t t;
     t = clock(); //armazena tempo
         
      for (int i = 0; i < 3333; i++){ // inicio do loop gerando 3.333 valores aleatorios e inserindo eles no inicio da lista
    ran =  rand() % 99999;
       insereInicio(&lista, ran);
    }
  for (int j = 0; j < 3334; j++){ // inicio do loop gerando 3.334 valores aleatorios e inserindo eles no fim da lista
     ran =  rand() % 99999;
         insereFim(&lista, ran);
     }
       for (int k = 0; k < 3333; k++){ // inicio do loop gerando 3.333 valores aleatorios e inserindo eles em uma posicao aleatoria
     ran =  rand() % 99999;
     pos = rand() % 6666; // sorteia um numero entre 0 e 6666 para ser definido como posicao
          inserePos(&lista, ran, pos); // insere o valor sorteado na posicao sorteada
          
       }

t = clock() - t; //tempo final - tempo inicial

   imprimeLista(lista); // usa a funcao imprime lista para imprimir a lista
    
 printf("\nTempo de execucao: %lf", ((double)t)/((CLOCKS_PER_SEC/1000))); //conversão para double e imprime o tempo gasto
  getchar ( );

   
    return 0;
}