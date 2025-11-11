/**
 * @file gen.h
 * @author Gabriel Gomes Marchesan
 * @brief Modulo do gerador de codigo
 * @version 0.3
 * @date 2025-02-24
 * 
 */
#ifndef _GEN_H_
#define _GEN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "struct_compiler.h"
#include "symbols.h"

// Prototipos
void genAdd();
void genSub();
void genMult();
void genDiv();
void genNum(char num_string[MAX_TOKEN]);
void gen_data_section(void);

#endif //_GEN_H_