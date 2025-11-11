//Bibliotecas 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//inclusão de bibliotecas próprias
#include "ordenacao.c"

int main(){
    int i;
    tipo_lista minha_lista;
    inicializaLista(&minha_lista, 10);

    srand(1);

    for (i=0; i<10; i++)
        insereLista(&minha_lista, rand()%100);

    imprimeLista(&minha_lista);

    printf("Ordenacao pelo metodo Bubble sort:\n");
    bubbleSort(&minha_lista);
    imprimeLista(&minha_lista);
    
}