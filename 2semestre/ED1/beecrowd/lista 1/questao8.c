//Bibliotecas 
#include <stdio.h>

//main

int main(){
    int in, duracao, fim;

    scanf("%d %d", &in, &fim);

    if( in > fim){
        duracao = 24 + fim - in;
    } else if (fim > in){
        duracao = fim - in;
    } else {
        duracao = 24;
    }

    printf("O JOGO DUROU %d HORA(S)\n", duracao);

 return 0;


    
}