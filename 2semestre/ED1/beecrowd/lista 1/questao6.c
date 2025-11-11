//Bibliotecas
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//main
int main(){
    double A, B, C, T;

    scanf("%lf%lf%lf",&A,&B,&C);

    if(((B * B) - 4 * A * C) < 0 || A == 0){
        printf("Impossivel calcular\n");
    }
    else {
        T = sqrt((B * B) - 4 * A * C);
        printf("R1 = %.50g\nR2 = %.5g\n", ((-B + T) / ( 2 * A)), ((-B - T) / ( 2 * A)));
    }
    return 0;

}