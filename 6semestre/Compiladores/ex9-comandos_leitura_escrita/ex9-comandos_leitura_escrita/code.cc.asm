;Procedimento para geracao do executavel apos compilacao (em Linux):
;(1) compilacao do Assembly com nasm: $ nasm -f elf64 <nome_do_arquivo>
;(2) likedicao: $ ld -m elf_x86_64 <nome_arquivo_objeto>

extern printf
extern scanf

section .data
fmtstr0 db "%d", 0
a: dd 0
b: dd 0

section .text
  global main
main:
_start:

;le valor inteiro
mov edx,4
mov ecx,a
mov ebx,1
mov eax,3
int 0x80

;escreve valor inteiro
mov edx,4
mov ecx,a
mov ebx,1
mov eax,4
int 0x80

;encerra programa
mov ebx,0
mov eax,1
int 0x80
