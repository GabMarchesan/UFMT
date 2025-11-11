//Bibliotecas
#include <stdlib.h>
#include <stdio.h>

//main
int main(){
    int X;
    float Y, Total;

    scanf("%d",&X);
    scanf("%f",&Y);

    Total = X / Y;

    printf("%.3f km/l", Total);

    return 1;
}