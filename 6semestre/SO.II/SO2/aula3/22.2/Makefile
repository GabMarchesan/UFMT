PRE = riscv64-linux-gnu-
CC = $(PRE)gcc #compilador
LD = $(PRE)ld #linker

CFLAGS = -ffreestanding              #Compilação sem suporte do sistema opracional
CFLAGS += -march=rv64g -mabi=lp64    #Arquitetura RISC-V 64 bits
CFLAGS += -Wall                      #Queremos todos os Warnings!
CFLAGS += -c -g -O0                  #Compilação sem otimização e com debug(g)

LDFLAGS = -nostdLib                  #não usar biblioteca padrão
LDFLAGS += -Ttext=0.80000000         #endereço de inicio do programa

kernel: boot.o main.o

boot.o:	boot.s
	$(CC) $(CFLAGS) boot.s 

main.o:	main.c
	$(CC) $(CFLAGS) main.c 

clean:
	@rm -f *.o