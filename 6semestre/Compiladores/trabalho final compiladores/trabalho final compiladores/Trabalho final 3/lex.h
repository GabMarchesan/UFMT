/**
 * @file lex.h
 * @author Edilayne Ramos e Gabriel Marchesan
 * @brief Modulo do analisador lexico
 * @version 0.5
 * @date 2025-05-03
 * 
 */

#ifndef _LEX_H_
#define _LEX_H_

//bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "struct_compiler.h"

// Prototipos
void initLex(char*);
type_token *getToken();
type_token *keyWordFind(char*);

#endif //_LEX_H_