// symbolTable.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"

ScopeManager scope_manager;

// Função Hash
int hashFunction(int max_size, char* key) {
    int i = 0;
    int sum = 0;
    int factor = 0;
    while (key[i] != '\0') {
        sum += (key[i] + factor);
        factor += 113;
        i++;
    }
    return sum % max_size;
}

void initSymTable(SymTable** table) {
    *table = (SymTable*) malloc(sizeof(SymTable));
    if (*table == NULL) {
        fprintf(stderr, "Erro de alocação para SymTable\n");
        exit(1);
    }
    (*table)->size = 0;
    (*table)->max_size = MAX_SIZE_HASH_ARRAY;
    (*table)->current_local_offset = 8;

    (*table)->array = (SymTableNode*) malloc((*table)->max_size * sizeof(SymTableNode));
    if ((*table)->array == NULL) {
        fprintf(stderr, "Erro de alocação para o array da SymTable\n");
        exit(1);
    }

    for (int i = 0; i < (*table)->max_size; i++) {
        (*table)->array[i].data.identifier[0] = '\0';
        (*table)->array[i].next = NULL;
    }
}

void initScopeManager() {
    initSymTable(&scope_manager.global_scope);
    scope_manager.local_scope = NULL;
    scope_manager.function_table = NULL;
}

void createLocalScope() {
    if (scope_manager.local_scope != NULL) {
        destroyLocalScope();
    }
    initSymTable(&scope_manager.local_scope);
}

void destroyLocalScope() {
    if (scope_manager.local_scope != NULL) {
        freeSymTable(scope_manager.local_scope);
        scope_manager.local_scope = NULL;
    }
}

void freeSymTable(SymTable* table) {
    if (!table) return;

    for (int i = 0; i < table->max_size; i++) {
        SymTableNode* node = table->array[i].next;
        while (node != NULL) {
            SymTableNode* aux = node;
            node = node->next;
            free(aux);
        }
    }
    free(table->array);
    free(table);
}

int addVar(SymTable* table, char* identifier, Type type, SymbolClass s_class) {
    if (table == NULL) return 0;
    
    int pos = hashFunction(table->max_size, identifier);
    SymTableNode* aux = &table->array[pos];

    if (aux->data.identifier[0] != '\0' && strcmp(aux->data.identifier, identifier) == 0) {
        return 0;
    }
    
    while (aux->next != NULL) {
        if (strcmp(aux->next->data.identifier, identifier) == 0) return 0;
        aux = aux->next;
    }

    SymTableNode* newNode;
    if (table->array[pos].data.identifier[0] == '\0') {
        newNode = &table->array[pos];
    } else {
        newNode = (SymTableNode*) malloc(sizeof(SymTableNode));
        if (newNode == NULL) return 0;
        aux->next = newNode;
        newNode->next = NULL;
    }

    strncpy(newNode->data.identifier, identifier, MAX_SIZE_SYMBOL - 1);
    newNode->data.identifier[MAX_SIZE_SYMBOL - 1] = '\0';
    newNode->data.type = type;
    newNode->data.symbol_class = s_class;

    if (s_class == VAR_LOCAL) {
        newNode->data.offset = table->current_local_offset;
        table->current_local_offset += 8;
    } else if (s_class == PARAM) {
        newNode->data.offset = 0; 
    }

    table->size++;
    return 1;
}

SymTableEntry* findVar(char* identifier) {
    if (scope_manager.local_scope != NULL) {
        int pos = hashFunction(scope_manager.local_scope->max_size, identifier);
        SymTableNode *aux = &scope_manager.local_scope->array[pos];
        while (aux != NULL && aux->data.identifier[0] != '\0') {
            if (strcmp(aux->data.identifier, identifier) == 0) {
                return &aux->data;
            }
            aux = aux->next;
        }
    }

    if (scope_manager.global_scope != NULL) {
        int pos = hashFunction(scope_manager.global_scope->max_size, identifier);
        SymTableNode *aux = &scope_manager.global_scope->array[pos];
        while (aux != NULL && aux->data.identifier[0] != '\0') {
            if (strcmp(aux->data.identifier, identifier) == 0) {
                return &aux->data;
            }
            aux = aux->next;
        }
    }
    
    return NULL;
}

void addFunc(char* identifier, Type return_type, ParamNode* params) {
    if (findFunc(identifier) != NULL) {
        yyerror("Funcao redefinida");
        return;
    }

    FuncTableNode* newNode = (FuncTableNode*) malloc(sizeof(FuncTableNode));
    strncpy(newNode->data.identifier, identifier, MAX_SIZE_SYMBOL - 1);
    newNode->data.identifier[MAX_SIZE_SYMBOL-1] = '\0';
    newNode->data.return_type = return_type;
    newNode->data.params = params;
    
    int count = 0;
    ParamNode* p = params;
    while(p != NULL) {
        count++;
        p = p->next;
    }
    newNode->data.param_count = count;

    newNode->next = scope_manager.function_table;
    scope_manager.function_table = newNode;
}

FuncTableEntry* findFunc(char* identifier) {
    FuncTableNode* current = scope_manager.function_table;
    while (current != NULL) {
        if (strcmp(current->data.identifier, identifier) == 0) {
            return &current->data;
        }
        current = current->next;
    }
    return NULL;
}

ParamNode* createParam(Type type, char* identifier, ParamNode* next) {
    ParamNode* newNode = (ParamNode*) malloc(sizeof(ParamNode));
    newNode->type = type;
    strncpy(newNode->identifier, identifier, MAX_SIZE_SYMBOL - 1);
    newNode->identifier[MAX_SIZE_SYMBOL - 1] = '\0';
    newNode->next = next;
    return newNode;
}

void freeParams(ParamNode* params) {
    ParamNode* current = params;
    while (current != NULL) {
        ParamNode* aux = current;
        current = current->next;
        free(aux);
    }
}

// Implementação das funções auxiliares para a lista de argumentos
ArgNode* createArg(Type type, char* code, ArgNode* next) {
    ArgNode* new_arg = (ArgNode*) malloc(sizeof(ArgNode));
    if (new_arg == NULL) {
        yyerror("Erro de alocacao de memoria para argumento");
        exit(1);
    }
    new_arg->type = type;
    strcpy(new_arg->code, code);
    new_arg->next = next;
    return new_arg;
}

void freeArgs(ArgNode* args) {
    ArgNode* current = args;
    while(current != NULL) {
        ArgNode* temp = current;
        current = current->next;
        free(temp);
    }
}

int countArgs(ArgNode* args) {
    int count = 0;
    ArgNode* current = args;
    while(current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}