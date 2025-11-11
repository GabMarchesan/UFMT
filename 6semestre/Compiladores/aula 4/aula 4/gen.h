/**
 * @file gen.h
 * @author Gabriel Gomes Marchesan
 * @brief Modulo do gerador de codigo
 * @version 0.1
 * @date 2025-02-17
 * 
 */
#ifndef _GEN_H_
#define _GEN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "struct_compiler.h"

// Prototipos
void genAdd();
void genSub();
void genMult();
void genDiv();
void genNum(char num_string[MAX_TOKEN]);

#endif //_GEN_H_