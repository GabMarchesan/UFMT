/**
 * @file synt.h
 * @author Gabriel Gomes Marchesan
 * @brief Modulo do analisado sintatico
 * @version 0.1
 * @date 2025-02-17
 * 
 */

#ifndef _SYNT_H_
#define _SYNT_H_

//Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "struct_compiler.h"
#include "lex.h"
#include "gen.h"

// Variaveis globais
type_token *lookahead;

// Prototipação das funções
int match(int token_tag);
int factor();
int term();
int termR();
int expr();
int exprR();
int number();

int main();

#endif //_SYNT_H_