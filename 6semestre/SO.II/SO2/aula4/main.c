#include "memlayout.h"

static void putc(char c){
    volatile char *reg = (char *) UART;
    *reg = c;
}

void puts(char *s){
    while(*s != 0){
        putc(*s++);
        /*  O mesmo que:
            putc(*s);
            s++;
        */
    }
}

void entry(){
    puts("Olá mundo! <disse o kernel>\n");
}