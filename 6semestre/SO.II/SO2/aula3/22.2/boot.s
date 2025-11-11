.global _start

.equ STACK_SIZE, 4096 #0x1000
# mhartid é o regitrador que contém o ID da CPU

_start:
csrr t0, mhartid  #copia o ID da CPU para t0
bnez t0, park  #t0 é igual a 0? se sim, pula para park

la sp, stack_end #carra o endereço de stack_end para sp

j entry #pula para a função entry em C

park:
    wfi
    j park
.skip STACK_SIZE
stack_end:
