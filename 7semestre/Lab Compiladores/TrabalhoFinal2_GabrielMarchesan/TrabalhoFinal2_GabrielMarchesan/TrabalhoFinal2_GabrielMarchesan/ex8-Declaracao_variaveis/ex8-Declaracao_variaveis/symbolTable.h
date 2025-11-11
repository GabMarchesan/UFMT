// symbolTable.h

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_SIZE_HASH_ARRAY 20
#define MAX_SIZE_SYMBOL 32

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_CHAR,
    TYPE_BOOL
} Type;

extern int cont_lines;
void yyerror(char *s);

typedef enum {
    VAR_GLOBAL,
    VAR_LOCAL,
    PARAM
} SymbolClass;

typedef struct symTableEntry {
    char identifier[MAX_SIZE_SYMBOL];
    Type type;
    SymbolClass symbol_class;
    int offset;
} SymTableEntry;

typedef struct symTableNode {
    SymTableEntry data;
    struct symTableNode *next;
} SymTableNode;

typedef struct symTable {
    SymTableNode* array;
    int max_size;
    int size;
    int current_local_offset;
} SymTable;

typedef struct ParamNode {
    Type type;
    char identifier[MAX_SIZE_SYMBOL];
    struct ParamNode* next;
} ParamNode;

typedef struct FuncTableEntry {
    char identifier[MAX_SIZE_SYMBOL];
    Type return_type;
    ParamNode* params;
    int param_count;
} FuncTableEntry;

typedef struct FuncTableNode {
    FuncTableEntry data;
    struct FuncTableNode* next;
} FuncTableNode;

typedef struct ArgNode {
    Type type;
    char code[4096];
    struct ArgNode* next;
} ArgNode;

typedef struct ScopeManager {
    SymTable* global_scope;
    SymTable* local_scope;
    FuncTableNode* function_table;
} ScopeManager;

extern ScopeManager scope_manager;

void initSymTable(SymTable** table);
void freeSymTable(SymTable* table);
int addVar(SymTable* table, char* identifier, Type type, SymbolClass s_class);
SymTableEntry* findVar(char* identifier);

// Funções do Gerenciador de Escopo
void initScopeManager();
void createLocalScope();
void destroyLocalScope();

// Funções da Tabela de Funções
void addFunc(char* identifier, Type return_type, ParamNode* params);
FuncTableEntry* findFunc(char* identifier);
ParamNode* createParam(Type type, char* identifier, ParamNode* next);
void freeParams(ParamNode* params);

// Funções auxiliares para a lista de argumentos
ArgNode* createArg(Type type, char* code, ArgNode* next);
void freeArgs(ArgNode* args);
int countArgs(ArgNode* args);

#endif