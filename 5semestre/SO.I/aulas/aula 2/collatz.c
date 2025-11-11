// Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// main
int n = 7;
int count;
while( n > 1) {
    if (n % 2 == 0){
        n = n / 2;
    }
    else {
        n = 3 * n + 1;
    }
    count = sprintf(ptr, %d, n);
    ptr += count;
}