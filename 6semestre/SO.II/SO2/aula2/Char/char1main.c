//biblioteca
#include <stdio.h>

//prototipo
char first_char(char *s);
char n_char(char *s, int pos);

//main
int main(){
    
    char *s = "You shall not pass!";
    int pos = 2;
    
    printf("O primeiro caracter da string %s eh %c\n", s, first_char(s));
    printf("O %d° caracter da string %s eh %c\n", pos, s, n_char(s, pos));
    
    return 0;
}