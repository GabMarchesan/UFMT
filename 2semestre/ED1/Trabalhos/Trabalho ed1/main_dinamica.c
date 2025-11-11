/**
 *
 * UFMT - Ciencia da Computacao
 * 
 * ED1 - Prof. Ivairton
 * 
 * Aluno: Herrison Batista de Meneses Junior // Gabriel Gomes Marchesan
 * 
 * Data: 17/03/2023
 * 
 * 
 *  Teste de lista dinamica com capacidade de 10.000
 * 
 **/

// Inclusao de bibliotecas

#include<stdio.h>
#include<stdlib.h>

// Inclusao de bibliotecas locais

#include"lista_dinamica.c"

int main() {
    int pos, ran, tam; // POS eh a variavel que armazena valor da posicao a ser inserida
 // RAN eh a variavel que armazena os numeros a serem inseridos
 // TAM eh a Variavel que armazena o tamanho da lista(a ser inserido pelo usuario)
  
     clock_t t;
     t = clock(); //armazena tempo
     printf("Digite o tamanho da lista...");
     scanf("%d", &tam);
 


    Lista lista; // declarando a lista e usando a função para iniciar

     // Chamar a lista e seu tamanho para ser alocado
     inicializarLista(&lista);
    
      for (int i = 0; i < 3333; i++){ // inicio do loop gerando 3.333 valores aleatorios e inserindo eles no inicio da lista
    ran =  rand() %99999; //gera numeros aleatorios entre 0 e 99.999
       insereInicioLista(&lista, ran);
    }
  for (int j = 0; j < 3334; j++){ // inicio do loop gerando 3.334 valores aleatorios e inserindo eles no fim da lista
     ran =  rand() % 99999; //gera numeros aleatorios entre 0 e 99.999
         insereFimLista(&lista, ran);
     }
       for (int k = 0; k < 3333; k++){ // inicio do loop gerando 3.333 valores aleatorios e inserindo eles em uma posicao aleatoria
     ran =  rand() % 99999;
     pos = rand() % 6666; // sorteia um numero entre 0 e 6666 para ser definido como posicao
          inserePosicaoLista(&lista, ran, pos); // insere o valor sorteado na posicao sorteada
          
       }

t = clock() - t; //tempo final - tempo inicial

   imprimeLista(lista); // usa a funcao imprime lista para imprimir a lista
    
 printf("\nTempo de execucao: %lf", ((double)t)/((CLOCKS_PER_SEC/1000))); //conversão para double e imprime o tempo gasto
  getchar ( );

    return 0;
}