#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbolTable.h"
#include "codeGeneration.h"


// Codigo para declaracao de variaveis
void makeCodeDeclaration(char* dest, char* identifier, Type type, char* value) {
    if (type == INTEGER) {
        if (value == NULL)
        	sprintf(dest, "%s: dq 0\n", identifier);
        else {
        	long x = atol(value); // Usar long para 64 bits (dq)
            sprintf(dest, "%s: dq %ld\n", identifier, x);
        }
    } else if (type == REAL) {
        if (value == NULL)
            sprintf(dest, "%s: dq 0.0\n", identifier);
        else {
            double x = atof(value);
            sprintf(dest, "%s: dq %f\n", identifier, x);
        }
    } else {
		fprintf(stderr, "Erro: tipo desconhecido na geracao de codigo para declaracao\n");
    }
}


// Termino da secao de dados e comeco da secao de codigo
void dumpCodeDeclarationEnd() {
    fprintf(out_file, "\nsection .text\n");
    fprintf(out_file, "global main\n");
    fprintf(out_file, "\nmain:\n");
}


int makeCodeAssignment(char* dest, char* id, char* expr) {   
    // A verificação do símbolo já foi feita no analisador sintático
    SymTableEntry* ret = findSymTable(&table, id);
    dest[0] = '\0';
 
    if (ret->type == INTEGER) {
        sprintf(dest + strlen(dest), "%s", expr);
        sprintf(dest + strlen(dest), "pop rbx\n");
        sprintf(dest + strlen(dest), "mov [%s],rbx\n", ret->identifier);
    } else if (ret->type == REAL) {
        // Para implementar: usar registradores XMM
        // sprintf(dest + strlen(dest), "%s", expr);
        // sprintf(dest + strlen(dest), "movsd xmm0, [rsp]\n");
        // sprintf(dest + strlen(dest), "add rsp, 8\n");
        // sprintf(dest + strlen(dest), "movsd [%s], xmm0\n", ret->identifier);
        fprintf(stderr, "Atribuicao de float ainda nao implementada na linha %d\n", cont_lines);
        return 0;
    } else {
        fprintf(stderr, "Operacao nao suportada na linha %d\n", cont_lines);
        return 0;
    }
    return 1;
}


int makeCodeLoad(char* dest, char* id, int ref) {
    dest[0] = '\0';

    if (ref == 0) { // É um literal (número)
        sprintf(dest + strlen(dest), "mov rbx,%s\n", id);
        sprintf(dest + strlen(dest), "push rbx\n");
        return 1;
    }

    // É uma variável (ID)
    // A verificação do símbolo já foi feita no analisador sintático
    SymTableEntry* ret = findSymTable(&table, id);
   
    sprintf(dest + strlen(dest), "mov rbx,[%s]\n", ret->identifier);
    sprintf(dest + strlen(dest), "push rbx\n");
    return 1;
}


void makeCodeAdd(char* dest, char* value) {
    sprintf(dest + strlen(dest), "%s", value);
    sprintf(dest + strlen(dest), "pop rcx\n");
    sprintf(dest + strlen(dest), "pop rbx\n");
    sprintf(dest + strlen(dest), "add rbx,rcx\n");
    sprintf(dest + strlen(dest), "push rbx\n");
}


void makeCodeSub(char* dest, char* value) {   
    sprintf(dest + strlen(dest), "%s", value);
    sprintf(dest + strlen(dest), "pop rcx\n");
    sprintf(dest + strlen(dest), "pop rbx\n");
    sprintf(dest + strlen(dest), "sub rbx,rcx\n");
    sprintf(dest + strlen(dest), "push rbx\n");

}

void makeCodeMul(char* dest, char* value2) {
    sprintf(dest + strlen(dest), "%s", value2);
    sprintf(dest + strlen(dest), "pop rcx\npop rbx\nimul rbx,rcx\npush rbx\n");
}


void makeCodeDiv(char* dest, char* value2) {
    sprintf(dest + strlen(dest), "%s", value2);
    sprintf(dest + strlen(dest), "pop r8\n"); // Divisor
    sprintf(dest + strlen(dest), "pop rax\n"); // Dividendo
    sprintf(dest + strlen(dest), "cqo\n");     // Estende rax para rdx:rax
    sprintf(dest + strlen(dest), "idiv r8\n");
    sprintf(dest + strlen(dest), "push rax\n"); // Quociente
}


void makeCodeMod(char* dest, char* value2) {
    sprintf(dest + strlen(dest), "%s", value2);
    sprintf(dest + strlen(dest), "pop r8\n");
    sprintf(dest + strlen(dest), "pop rax\n");
    sprintf(dest + strlen(dest), "cqo\n");
    sprintf(dest + strlen(dest), "idiv r8\n");
    sprintf(dest + strlen(dest), "push rdx\n"); // Resto
}