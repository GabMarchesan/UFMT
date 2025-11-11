// codeGeneration.c

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
    SymTableEntry* entry = findVar(id);
    if (entry == NULL) return 0;

    char access[64];
    if (entry->symbol_class == VAR_GLOBAL) {
        sprintf(access, "[%s]", entry->identifier);
    } else {
        if (entry->symbol_class == PARAM) {
             sprintf(access, "[rbp+%d]", entry->offset);
        } else { // VAR_LOCAL
             sprintf(access, "[rbp-%d]", entry->offset);
        }
    }

    if (entry->type == TYPE_INT || entry->type == TYPE_BOOL) {
        sprintf(dest + strlen(dest), "mov rsi, %s\n", access);
        if (ln) sprintf(dest + strlen(dest), "mov rdi, fmt_dln\n");
        else sprintf(dest + strlen(dest), "mov rdi, fmt_d\n");
        sprintf(dest + strlen(dest), "mov rax, 0\n");
    } else if (entry->type == TYPE_STRING) {
        sprintf(dest + strlen(dest), "mov rsi, %s\n", access); 
        if (ln) sprintf(dest + strlen(dest), "mov rdi, fmt_sln\n");
        else sprintf(dest + strlen(dest), "mov rdi, fmt_s\n");
        sprintf(dest + strlen(dest), "mov rax, 0\n");
    } else { // TYPE_FLOAT
        sprintf(dest + strlen(dest), "movsd xmm0, %s\n", access);
        if (ln) sprintf(dest + strlen(dest), "mov rdi, fmt_fln\n");
        else sprintf(dest + strlen(dest), "mov rdi, fmt_f\n");
        sprintf(dest + strlen(dest), "mov rax, 1\n");
    }
    sprintf(dest + strlen(dest), "call printf\n");
    return 1;
}

void makeLabel(char* out_label){
    static int label_count = 0;
    sprintf(out_label, "L%d", label_count++);
}

void makeCodeDeclaration(char* dest, char* identifier, Type type, SymbolClass s_class){
    if (s_class == VAR_GLOBAL) {
        if (type == TYPE_INT) {
            sprintf(dest, "%s: dq 0\n", identifier);
        } else if (type == TYPE_BOOL) {
            sprintf(dest, "%s: dq 0\n", identifier);
        } else if (type == TYPE_FLOAT) {
            sprintf(dest, "%s: dq 0.0\n", identifier);
        } else if (type == TYPE_STRING) {
            sprintf(dest, "%s: dq 0\n", identifier);
        }
    } else {
        dest[0] = '\0';
    }
}

void dumpCodeDeclarationEnd(){
    fprintf(out_file, "\nsection .text\n");
    fprintf(out_file, "global main\n\n");
}

int makeCodeRead(char* dest, char *id){
    SymTableEntry* entry = findVar(id);
    dest[0] = '\0';
    if (entry == NULL) return 0;

    char access[64];
    if (entry->symbol_class == VAR_GLOBAL) {
        sprintf(access, "%s", entry->identifier);
    } else {
         if (entry->symbol_class == PARAM) {
             sprintf(access, "rbp+%d", entry->offset);
        } else { 
             sprintf(access, "rbp-%d", entry->offset);
        }
    }

    if (entry->type == TYPE_INT || entry->type == TYPE_BOOL) sprintf(dest + strlen(dest), "mov rdi,fmt_d\n");
    else if (entry->type == TYPE_FLOAT) sprintf(dest + strlen(dest), "mov rdi,fmt_f\n");
    else if (entry->type == TYPE_STRING) sprintf(dest + strlen(dest), "mov rdi,fmt_s\n");

    sprintf(dest + strlen(dest), "lea rsi, [%s]\n", access);
    sprintf(dest + strlen(dest), "mov rax,0\n");
    sprintf(dest + strlen(dest), "call scanf\n");
    return 1;
}

int makeCodeAssignment(char* dest, char* id, char* expr){   
    SymTableEntry* entry = findVar(id);
    dest[0] = '\0';
    if (entry == NULL) return 0;

    sprintf(dest + strlen(dest), "%s", expr);

    char access[64];
    if (entry->symbol_class == VAR_GLOBAL) {
        sprintf(access, "[%s]", entry->identifier);
    } else {
        if (entry->symbol_class == PARAM) {
             sprintf(access, "[rbp+%d]", entry->offset);
        } else { 
             sprintf(access, "[rbp-%d]", entry->offset);
        }
    }
    
    if (entry->type == TYPE_INT || entry->type == TYPE_BOOL) {
        sprintf(dest + strlen(dest), "movsd xmm0, [rsp]\n");
        sprintf(dest + strlen(dest), "add rsp, 8\n");
        sprintf(dest + strlen(dest), "cvttsd2si rbx, xmm0\n");
        sprintf(dest + strlen(dest), "mov %s, rbx\n", access);
    } else { 
        sprintf(dest + strlen(dest), "movsd xmm0, [rsp]\n");
        sprintf(dest + strlen(dest), "add rsp, 8\n");
        sprintf(dest + strlen(dest), "movsd %s, xmm0\n", access);
    }
    return 1;
}

int makeCodeStringAssignment(char* dest, char* id, char* str_literal) {
    SymTableEntry* entry = findVar(id);
    if (entry == NULL || entry->type != TYPE_STRING) {
        return 0;
    }

    char label[20];
    sprintf(label, "SL%d", string_literal_counter++);
    sprintf(string_literals_buffer + strlen(string_literals_buffer), "%s: db \"%s\", 0\n", label, str_literal);

    char access[64];
     if (entry->symbol_class == VAR_GLOBAL) {
        sprintf(access, "[%s]", entry->identifier);
    } else {
        sprintf(access, "[rbp-%d]", entry->offset);
    }

    sprintf(dest, "mov rax, %s\nmov %s, rax\n", label, access);
    return 1;
}


int makeCodeLoad(char* dest, char* id, int ref){
    dest[0] = '\0';
    if (ref == 0) {
        sprintf(dest + strlen(dest), "mov rax, %s\n", id);
        sprintf(dest + strlen(dest), "cvtsi2sd xmm0, rax\n");
    } else {
        SymTableEntry* entry = findVar(id);
        if (entry == NULL) return 0;
        
        char access[64];
        if (entry->symbol_class == VAR_GLOBAL) {
            sprintf(access, "[%s]", entry->identifier);
        } else {
             if (entry->symbol_class == PARAM) {
                sprintf(access, "[rbp+%d]", entry->offset);
            } else { // VAR_LOCAL
                sprintf(access, "[rbp-%d]", entry->offset);
            }
        }

        if (entry->type == TYPE_INT || entry->type == TYPE_BOOL) {
            sprintf(dest + strlen(dest), "mov rax, %s\n", access);
            sprintf(dest + strlen(dest), "cvtsi2sd xmm0, rax\n");
        } else { 
            sprintf(dest + strlen(dest), "movsd xmm0, %s\n", access);
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

void makeCodeFunctionPrologue(char* dest, char* func_id, int local_vars_size) {
    sprintf(dest, "\n%s:\n", func_id);
    sprintf(dest + strlen(dest), "push rbp\n");
    sprintf(dest + strlen(dest), "mov rbp, rsp\n");
    if (local_vars_size > 0) {
        sprintf(dest + strlen(dest), "sub rsp, %d\n", local_vars_size);
    }
}

void makeCodeFunctionEpilogue(char* dest) {
    sprintf(dest, "mov rsp, rbp\n");
    sprintf(dest + strlen(dest), "pop rbp\n");
    sprintf(dest + strlen(dest), "ret\n");
}

void makeCodeFunctionCall(char* dest, char* func_id, char* params_code, int param_count) {
    sprintf(dest, "%s", params_code);
    sprintf(dest + strlen(dest), "call %s\n", func_id);
    if (param_count > 0) {
        sprintf(dest + strlen(dest), "add rsp, %d\n", param_count * 8);
    }
    sprintf(dest + strlen(dest), "sub rsp, 8\n");
    sprintf(dest + strlen(dest), "movsd [rsp], xmm0\n");
}

void makeCodeReturn(char* dest, char* expr_code) {
    sprintf(dest, "%s", expr_code);
    sprintf(dest + strlen(dest), "movsd xmm0, [rsp]\n");
    sprintf(dest + strlen(dest), "add rsp, 8\n");
}