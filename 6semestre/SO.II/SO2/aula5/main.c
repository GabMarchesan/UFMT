#include "memlayout.h"
#include "defs.h"
#include "console.h"

//static void putc(char c){
//    volatile char *reg = (char *) UART;
//    *reg = c;
//}

void puts(char *s){
    while(*s != 0){
        uart_putc(*s++);
        /*  O mesmo que:
            putc(*s);
            s++;
        */
    }
}

void entry(){
    
    int c;
    //int i = 0;

    uart_init();
    //limpar tela e mover cursor para posição 0;0
    puts(CLEAR_SCREEN CURSOR_UP_LEFT); 
    puts(BOLD PURPLE_BACK WHITE);
    puts("BoS - Braga's Operating System\n\n");
    puts(RESET_COLOR);
    puts(BOLD WHITE_BACK PINK);
    
    puts("Progress: [__________]\n");

    for(;;){
        c = uart_getc(); //Lê o caracter do teclado
        
        if(c == -1) //Nada de novo para ler
            continue;

        switch(c){
            case '\r': //enter
                uart_putc('\r');
                uart_putc('\n');
                break;
            case 127: 
                uart_putc('\b'); //move cursor uma posição para a esquerda
                uart_putc(' '); //apaga caracter e move cursor para a direita
                uart_putc('\b'); //voltar cursor para posição original
                break;
            /*case 110:
                if(i <= 10){
                    puts(BOLD PINK_BACK);
                    uart_putc(' ');
                    i++;
                    break;
                } else {
                    puts("Progress complet\n");
                    break;
                }
            */
                
            default:
                uart_putc(c);

        }
    }

}