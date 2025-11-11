extern printf
extern scanf
section .data
fmt_d: db "%ld", 0
fmt_f: db "%f", 0
fmt_s: db "%s", 0
fmt_dln: db "%ld", 10, 0
fmt_fln: db "%f", 10, 0
fmt_sln: db "%s", 10, 0
total: dq 0.0
media: dq 0.0
impar: dq 0
empresa: dq 0

soma:
push rbp
mov rbp, rsp
movsd xmm0, [rbp+16]
sub rsp, 8
movsd [rsp], xmm0
movsd xmm0, [rbp+24]
sub rsp, 8
movsd [rsp], xmm0
 movsd xmm1,[rsp]
 add rsp,8
 movsd xmm0,[rsp]
 add rsp,8
 addsd xmm0,xmm1
 sub rsp,8
 movsd [rsp],xmm0
movsd xmm0, [rsp]
add rsp, 8
mov rsp, rbp
pop rbp
ret

main:
push rbp
mov rbp, rsp
sub rsp, 24
mov rax, SL0
mov [empresa], rax
mov rdi, SL1
mov rax, 0
call printf
mov rdi,fmt_s
lea rsi, [rbp-8]
mov rax,0
call scanf
mov rdi,fmt_f
lea rsi, [rbp-16]
mov rax,0
call scanf
mov rdi,fmt_f
lea rsi, [rbp-24]
mov rax,0
call scanf
movsd xmm0, [rbp-24]
sub rsp, 8
movsd [rsp], xmm0
movsd xmm0, [rbp-16]
sub rsp, 8
movsd [rsp], xmm0
call soma
add rsp, 16
sub rsp, 8
movsd [rsp], xmm0
movsd xmm0, [rsp]
add rsp, 8
movsd [total], xmm0
movsd xmm0, [total]
sub rsp, 8
movsd [rsp], xmm0
mov rax, 2
cvtsi2sd xmm0, rax
sub rsp, 8
movsd [rsp], xmm0
 movsd xmm1,[rsp]
 add rsp,8
 movsd xmm0,[rsp]
 add rsp,8
 divsd xmm0,xmm1
 sub rsp,8
 movsd [rsp],xmm0
movsd xmm0, [rsp]
add rsp, 8
movsd [media], xmm0
mov rax, 0
cvtsi2sd xmm0, rax
sub rsp, 8
movsd [rsp], xmm0
movsd xmm0, [rsp]
add rsp, 8
cvttsd2si rbx, xmm0
mov [impar], rbx
L2:
movsd xmm0, [total]
sub rsp, 8
movsd [rsp], xmm0
mov rax, 0
cvtsi2sd xmm0, rax
sub rsp, 8
movsd [rsp], xmm0
movsd xmm1, [rsp]
add rsp, 8
movsd xmm0, [rsp]
add rsp, 8
ucomisd xmm0, xmm1
jng L3
mov rax, [impar]
cvtsi2sd xmm0, rax
sub rsp, 8
movsd [rsp], xmm0
mov rax, 0
cvtsi2sd xmm0, rax
sub rsp, 8
movsd [rsp], xmm0
movsd xmm1, [rsp]
add rsp, 8
movsd xmm0, [rsp]
add rsp, 8
ucomisd xmm0, xmm1
jne L0
mov rax, 1
cvtsi2sd xmm0, rax
sub rsp, 8
movsd [rsp], xmm0
movsd xmm0, [rsp]
add rsp, 8
cvttsd2si rbx, xmm0
mov [impar], rbx
jmp L1
L0:
mov rax, 0
cvtsi2sd xmm0, rax
sub rsp, 8
movsd [rsp], xmm0
movsd xmm0, [rsp]
add rsp, 8
cvttsd2si rbx, xmm0
mov [impar], rbx
L1:
movsd xmm0, [total]
sub rsp, 8
movsd [rsp], xmm0
mov rax, 1
cvtsi2sd xmm0, rax
sub rsp, 8
movsd [rsp], xmm0
 movsd xmm1,[rsp]
 add rsp,8
 movsd xmm0,[rsp]
 add rsp,8
 subsd xmm0,xmm1
 sub rsp,8
 movsd [rsp],xmm0
movsd xmm0, [rsp]
add rsp, 8
movsd [total], xmm0
jmp L2
L3:
mov rsi, [empresa]
mov rdi, fmt_sln
mov rax, 0
call printf
mov rsi, [rbp-8]
mov rdi, fmt_sln
mov rax, 0
call printf
mov rdi, SL2
mov rax, 0
call printf
movsd xmm0, [media]
mov rdi, fmt_fln
mov rax, 1
call printf
mov rdi, SL3
mov rax, 0
call printf
mov rax, [impar]
cvtsi2sd xmm0, rax
sub rsp, 8
movsd [rsp], xmm0
mov rax, 1
cvtsi2sd xmm0, rax
sub rsp, 8
movsd [rsp], xmm0
movsd xmm1, [rsp]
add rsp, 8
movsd xmm0, [rsp]
add rsp, 8
ucomisd xmm0, xmm1
jne L4
mov rdi, SL4
mov rax, 0
call printf
jmp L5
L4:
mov rdi, SL5
mov rax, 0
call printf
L5:
mov rax, 0
cvtsi2sd xmm0, rax
sub rsp, 8
movsd [rsp], xmm0
movsd xmm0, [rsp]
add rsp, 8
mov rsp, rbp
pop rbp
ret
SL0: db "UFMT", 0
SL1: db "Escreva seu nome:", 0
SL2: db "Sua media eh:", 0
SL3: db "Seu numero eh ", 0
SL4: db "impar", 0
SL5: db "par", 0

section .text
global main

