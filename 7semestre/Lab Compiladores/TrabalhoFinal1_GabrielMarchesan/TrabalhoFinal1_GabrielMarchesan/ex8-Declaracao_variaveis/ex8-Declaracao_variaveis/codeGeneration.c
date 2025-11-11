#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"
#include "codeGeneration.h"

static char string_literals_buffer[2048] = "";
static int string_literal_counter = 0;
char jumps[10][4] = {"jnl", "jng", "jne", "jnz", "", "jz", "je", "jg", "jl", "jmp"};

void makeCodeWriteString(char* dest, char* str) {
    char label[20];
    sprintf(label, "SL%d", string_literal_counter++);
    sprintf(string_literals_buffer + strlen(string_literals_buffer), "%s: db \"%s\", 0\n", label, str);
    sprintf(dest, "mov rdi, %s\nmov rax, 0\ncall printf\n", label);
}

void dumpStringLiterals() {
    if (strlen(string_literals_buffer) > 0) {
        fprintf(out_file, "%s", string_literals_buffer);
    }
}

int makeCodeWriteExpr(char* dest, char* expr_code, int ln){
    dest[0] = '\0';
    sprintf(dest + strlen(dest), "%s", expr_code);
    sprintf(dest + strlen(dest), "movsd xmm0, [rsp]\nadd rsp, 8\n");
    if (ln) sprintf(dest + strlen(dest), "mov rdi, fmt_fln\n");
    else sprintf(dest + strlen(dest), "mov rdi, fmt_f\n");
    sprintf(dest + strlen(dest), "mov rax, 1\ncall printf\n");
    return 1;
}

int makeCodeWriteID(char* dest, char* id, int ln){
    dest[0] = '\0';
    SymTableEntry* ret = findSymTable(&table, id);
    if (ret == NULL) return 0;

    if (ret->type == INTEGER) {
        sprintf(dest + strlen(dest), "mov rsi, [%s]\n", ret->identifier);
        if (ln) sprintf(dest + strlen(dest), "mov rdi, fmt_dln\n");
        else sprintf(dest + strlen(dest), "mov rdi, fmt_d\n");
        sprintf(dest + strlen(dest), "mov rax, 0\n");
    } else { // REAL
        sprintf(dest + strlen(dest), "movsd xmm0, [%s]\n", ret->identifier);
        if (ln) sprintf(dest + strlen(dest), "mov rdi, fmt_fln\n");
        else sprintf(dest + strlen(dest), "mov rdi, fmt_f\n");
        sprintf(dest + strlen(dest), "mov rax, 1\n");
    }
    sprintf(dest + strlen(dest), "call printf\n");
    return 1;
}

void makeLabel(char* out_label){
    static int label_count = 0;
    strcpy(out_label, "label");
    char s[10];
    sprintf(s, "%d", label_count);
    strcat(out_label, s);
    label_count++;
}

void makeCodeDeclaration(char* dest, char* identifier, Type type, char* value){
    if (type == INTEGER) {
        sprintf(dest, "%s: dq 0\n", identifier);
    } else if (type == REAL) {
        sprintf(dest, "%s: dq 0.0\n", identifier);
    }
}

void dumpCodeDeclarationEnd(){
    fprintf(out_file, "\nsection .text\n");
    fprintf(out_file, "global main\n");
    fprintf(out_file, "\nmain:\n");
}

int makeCodeRead(char* dest, char *id){
    SymTableEntry* ret = findSymTable(&table,id);
    dest[0] = '\0';
    if (ret == NULL) return 0;

    if (ret->type == INTEGER) sprintf(dest + strlen(dest), "mov rdi,fmt_d\n");
    else if (ret->type == REAL) sprintf(dest + strlen(dest), "mov rdi,fmt_f\n");

    sprintf(dest + strlen(dest), "mov rsi,%s\n", ret->identifier);
    sprintf(dest + strlen(dest), "mov rax,0\n");
    sprintf(dest + strlen(dest), "call scanf\n");
    return 1;
}

int makeCodeAssignment(char* dest, char* id, char* expr){   
    SymTableEntry* ret = findSymTable(&table, id);
    dest[0] = '\0';
    if (ret == NULL) return 0;

    sprintf(dest + strlen(dest), "%s", expr);
    
    if (ret->type == INTEGER) {
        sprintf(dest + strlen(dest), "movsd xmm0, [rsp]\n");
        sprintf(dest + strlen(dest), "add rsp, 8\n");
        sprintf(dest + strlen(dest), "cvttsd2si rbx, xmm0\n");
        sprintf(dest + strlen(dest), "mov [%s], rbx\n", ret->identifier);
    } else { 
        sprintf(dest + strlen(dest), "movsd xmm0, [rsp]\n");
        sprintf(dest + strlen(dest), "add rsp, 8\n");
        sprintf(dest + strlen(dest), "movsd [%s], xmm0\n", ret->identifier);
    }
    return 1;
}

int makeCodeLoad(char* dest, char* id, int ref){
    dest[0] = '\0';
    if (ref == 0) {
        sprintf(dest + strlen(dest), "mov rax, %s\n", id);
        sprintf(dest + strlen(dest), "cvtsi2sd xmm0, rax\n");
    } else { 
        SymTableEntry* ret = findSymTable(&table, id);
        if (ret == NULL) return 0;
        if (ret->type == INTEGER) {
            sprintf(dest + strlen(dest), "mov rax, [%s]\n", ret->identifier);
            sprintf(dest + strlen(dest), "cvtsi2sd xmm0, rax\n");
        } else {
            sprintf(dest + strlen(dest), "movsd xmm0, [%s]\n", ret->identifier);
        }
    }
    sprintf(dest + strlen(dest), "sub rsp, 8\n");
    sprintf(dest + strlen(dest), "movsd [rsp], xmm0\n");
    return 1;
}

void makeCodeAdd(char* dest, char* v1, char* v2) { sprintf(dest, "%s%s movsd xmm1,[rsp]\n add rsp,8\n movsd xmm0,[rsp]\n add rsp,8\n addsd xmm0,xmm1\n sub rsp,8\n movsd [rsp],xmm0\n", v1, v2); }
void makeCodeSub(char* dest, char* v1, char* v2) { sprintf(dest, "%s%s movsd xmm1,[rsp]\n add rsp,8\n movsd xmm0,[rsp]\n add rsp,8\n subsd xmm0,xmm1\n sub rsp,8\n movsd [rsp],xmm0\n", v1, v2); }
void makeCodeMul(char* dest, char* v1, char* v2) { sprintf(dest, "%s%s movsd xmm1,[rsp]\n add rsp,8\n movsd xmm0,[rsp]\n add rsp,8\n mulsd xmm0,xmm1\n sub rsp,8\n movsd [rsp],xmm0\n", v1, v2); }
void makeCodeDiv(char* dest, char* v1, char* v2) { sprintf(dest, "%s%s movsd xmm1,[rsp]\n add rsp,8\n movsd xmm0,[rsp]\n add rsp,8\n divsd xmm0,xmm1\n sub rsp,8\n movsd [rsp],xmm0\n", v1, v2); }
void makeCodeMod(char* dest, char* v1, char* v2) { sprintf(dest, "%s%s pop r8\n pop rax\n xor rdx,rdx\n idiv r8\n push rdx\n", v1, v2); }

int makeCodeComp(char* dest, char* expr1, char* expr2){
    dest[0] = '\0';
    sprintf(dest + strlen(dest), "%s%s", expr1, expr2);
    sprintf(dest + strlen(dest), "movsd xmm1, [rsp]\nadd rsp, 8\n");
    sprintf(dest + strlen(dest), "movsd xmm0, [rsp]\nadd rsp, 8\n");
    sprintf(dest + strlen(dest), "ucomisd xmm0, xmm1\n");
    return 1;
}

void makeCodeIf(char* dest, char* expr_code, int expr_jump, char* block_code){
    char label[16]; makeLabel(label);
    sprintf(dest, "%s%s %s\n%s%s:\n", expr_code, jumps[expr_jump + JUMPS_ARRAY_OFFSET], label, block_code, label);
}

void makeCodeIfElse(char* dest, char* expr_code, int expr_jump, char* block_code_if, char* block_code_else){
    char else_label[16], final_label[16]; makeLabel(else_label); makeLabel(final_label);
    sprintf(dest, "%s%s %s\n%sjmp %s\n%s:\n%s%s:\n", expr_code, jumps[expr_jump + JUMPS_ARRAY_OFFSET], else_label, block_code_if, final_label, else_label, block_code_else, final_label);
}

void makeCodeWhile(char* dest, char* expr_code, int expr_jump, char* block_code){
    char loop_label[16], final_label[16]; makeLabel(loop_label); makeLabel(final_label);
    sprintf(dest, "%s:\n%s%s %s\n%sjmp %s\n%s:\n", loop_label, expr_code, jumps[expr_jump + JUMPS_ARRAY_OFFSET], final_label, block_code, loop_label, final_label);
}