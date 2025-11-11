#ifndef CODE_H
#define CODE_H

#include <stdio.h>
#include <stdlib.h>
#include "symbolTable.h"

#define STRING_SIZE 256
#define JUMPS_ARRAY_OFFSET 4

extern FILE* out_file;

void makeLabel(char* out_label);
void makeCodeDeclaration(char* dest, char* identifier, Type type, char* value);
void dumpCodeDeclarationEnd();
int makeCodeRead(char* dest, char *id);
int makeCodeWriteID(char* dest, char* id, int ln);
int makeCodeWriteExpr(char* dest, char* expr_code, int ln);
void makeCodeWriteString(char* dest, char* str);
void dumpStringLiterals();
int makeCodeAssignment(char* dest, char* id, char* expr);
int makeCodeLoad(char* dest, char* id, int ref);
void makeCodeAdd(char* dest, char* value1, char* value2);
void makeCodeSub(char* dest, char* value1, char* value2);
void makeCodeMul(char* dest, char* value1, char* value2);
void makeCodeDiv(char* dest, char* value1, char* value2);
void makeCodeMod(char* dest, char* value1, char* value2);
int makeCodeComp(char* dest, char* expr1, char* expr2);
void makeCodeIf(char* dest, char* expr_code, int expr_jump, char* block_code);
void makeCodeWhile(char* dest, char* expr_code, int expr_jump, char* block_code);
void makeCodeIfElse(char* dest, char* expr_code, int expr_jump, char* block_code_if, char* block_code_else);

#endif