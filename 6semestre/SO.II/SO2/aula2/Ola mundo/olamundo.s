.global _start
.global finaliza

_start:
    #informar ao SO que queremos executar a função write
    li a7, 64 # ID da chamada de sistema para o WRITE

    #configurar os argumentos para write
    li a0, 1 #descritor do arquivo da saida padrão (terminal)
    la a1, msg #endereco da string a ser impressa
    li a2, 11 #tamanho da string a ser impressa

    ecall #Instrucão para chamar o SO --> n tem nenhum argumento


#terminar execução do prorama invocando exit()
# exit(0)
finaliza:
    li a0, 0 #código de saida
    li a7, 94 #id da chamada de sistema para o exit
    ecall

msg:
    .ascii "Olá mundo\n" #string terminada em 0
