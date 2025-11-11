/**
 * @file lex.h
 * @author Gabriel Gomes Marchesan
 * @brief Modulo do analisador lexico
 * @version 0.1
 * @date 2025-02-17
 * 
 */
#ifndef _LEX_H_
#define _LEX_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "struct_compiler.h"

// Variáveis globais (apenas declaração)
extern int pos;
extern char string[MAX_CHAR];

// Protótipos
void initLex();
type_token *getToken();

#endif //_LEX_H_