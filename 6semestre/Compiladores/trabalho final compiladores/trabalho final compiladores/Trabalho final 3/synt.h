/**
 * @file synt.h
 * @author Edilayne Ramos e Gabriel Marchesan
 * @brief Modulo do analisado sintatico
 * @version 0.6
 * @date 2025-05-03
 * 
 */

#ifndef _SYNT_H_
#define _SYNT_H_

//bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "struct_compiler.h"
#include "lex.h"
#include "gen.h"
#include "symbols.h"

/*
 * Gramatica: 
 *      program -> declarations begin statements end func_code
 *      declarations-> declaration declarations | [vazio]
 *      declaration -> { int | float | string | char } id { DeclaracaoV | DeclaraçãoF }
 *      DeclaraçãoV → ;
 *      DeclaraçãoF → (  [{int|float|string|char}id,]* [{int|float|string|char}id] | <vazio>  ) ;
 *      statements --> statement statements | [vazio]
 *      statement  --> read_stmt | write_stmt | if_stmt | if_else_stmt | while_stmt | assign_stmt |func_call_cmd | declaration_local
 *      read_stmt  --> read id;
 *      write_stmt --> write id;
 *      if_stmt    --> if '(' rel_expr ')' begin statements end [ else begin statements end  ]  (ideal, ainda por fazer)
 *      if_stmt    --> if '(' E ')' begin statements end [ else begin statements end ] (implementado)
 *      assign_stmt -> id '=' E;
 *      rel_expr   --> id rel_operator E
 *      rel_operator -->  '<' | '>' | '<=' | '>=' | '==' | '!='
 *      E          --> expressões com numeros inteiros, cujo resultado final fica na pilha.
        func_code ---> { int | float | string | char } id (parametros) begin statements end | <vazio>
 */


// Prototipos
void program (void);
void declarations(void);
void statements (void);
int declaration(void);
int declarationV(char *var_name, int var_type);
int declarationF(char *func_name, int func_type);
int statement(void);
int func_code();
int func_call_cmd(char *func_name);


int B(void);
int boolOperator(int*);

int E();
int ER();
int T();
int TR();
int F();

int main();

#endif //_SYNT_H_