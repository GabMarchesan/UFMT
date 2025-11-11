#include "memlayout.h"
#include "types.h"

//Registradores UART
#define RBR 0  //Receiver Buffer Register
#define THR 0  //Reguistrador p/transmissão de dados
#define IER 1  //Reg. p/ habilitar interrupção
#define FCR 2  //Controle de FIFO
#define LCR 3  //Line Control Register
#define LSR 5  //Line Status Register

char
read_reg(uint8 reg){
    char c;
    volatile char * base = (char *)UART;
    c = *(base + reg);
    return c;
}

void
write_reg(uint8 reg, char c){
    volatile char * base = (char *)UART;
    *(base + reg) = c;
}

void
uart_init(){
    //Habilita FIFO
    write_reg(FCR, 1);
    //Dados de 8 bits
    write_reg(LCR,3);
    //Habilita a interrupção
    write_reg(IER,1);
}

int
uart_getc(){
    char c;
    if((read_reg(LSR) & 1) == 0){
        return -1;
    }
    c = read_reg(RBR);
    return c;
} 

//Imprime caracter
void uart_putc(char c){
    while((read_reg(LSR) & (1 << 5)) == 0)
    ;
    write_reg(THR,c);
}