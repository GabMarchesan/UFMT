;UFMT-Compiladores
;Prof. Ivairton
;Procedimento para geracao do executavel apos compilacao (em Linux):
;(1) compilacao do Assembly com nasm: $ nasm -f elf64 <nome_do_arquivo>
;(2) likedicao: $ ld -m elf_x86_64 <nome_arquivo_objeto>

extern printf
extern scanf

	section .text
	global main,_start
main:
_start:
;Armazenamento de numero
mov rax,1
push rax
;atribuicao
pop rax
mov [i], rax
;Armazenamento de numero
mov rax,1
push rax
;Armazenamento de numero
mov rax,1
push rax
;jump_func
jal func_label0
func_label0:
;prologo da funcao
push rbp
mov rbp, rsp
;Armazenamento de numero
mov rax,0
push rax
;atribuicao
pop rax
mov [x], rax
;epilogo da funcao
mov rsp, rbp
pop rbp
;retorno da funcao
ret
;encerra programa
mov rax, 60
mov rdi, 0
syscall

	section .data
str0: db "%d",0
str1: db "%s",0
str2: db "%c",0
str3: db "%lf",0
i: dd 0
