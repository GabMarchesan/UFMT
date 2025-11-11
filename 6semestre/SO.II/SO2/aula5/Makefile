PRE = riscv64-linux-gnu-
CC = $(PRE)gcc               #compilador
LD = $(PRE)ld                #linker
QEMU = qemu-system-riscv64   #emulador

CFLAGS = -ffreestanding              #Compilação sem suporte do sistema opracional
CFLAGS += -march=rv64g -mabi=lp64    #Arquitetura RISC-V 64 bits
CFLAGS += -Wall                      #Queremos todos os Warnings!
CFLAGS += -c -g -O0                  #Compilação sem otimização e com debug(g)

LDFLAGS = -nostdLib                  #não usar biblioteca padrão
LDFLAGS += -Ttext=0x80000000         #endereço de inicio do programa

OBJ = \
	boot.o \
	main.o \
	uart.o

kernel: boot.o main.o
	$(LD) $(LDFLAGS) -o kernel $(OBJ)

%.o: %.s
	$(CC) $(CFLAGS) $<

%.o: %.c
	$(CC) $(CFLAGS) $< 

clean:
	@rm -f *.o kernel

run: 
	$(QEMU) -append 'console=ttyS0'  -nographic -serial mon:stdio -smp 4 -machine virt -bios none -kernel kernel