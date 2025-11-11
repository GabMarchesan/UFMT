//Bibliotecas 
#include <stdio.h>

//main 

int main(){

    int n, i, cont = 0;
    float r = 0;

    do{

    scanf("%d", &n);
    if ( n < 0)
    {

      break;

    }


    r += n;
    cont++;

  }while (n > 0);

  printf("%.2f\n", r/cont);
  
  return 0;

}
