#include <stdio.h>
#include <stdlib.h>

int main(){
    
    //declaracao de variaveis 
    double raio, area;

    //inicializacao
    scanf("%lf",&raio);
    area = 3.14159*raio*raio;
    
    //imprime os resultados
     printf("A=%.4lf\n", area);

    return 0;

}