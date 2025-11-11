/**
 * @file gen.h
 * @author Edilayne Ramos e Gabriel Marchesan
 * @brief Modulo do gerador de codigo
 * @version 0.4
 * @date 2025-05-03
 * 
 */
 
#ifndef _GEN_H_
#define _GEN_H_

//bibliotecas
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
void genVar(char var_name[MAX_TOKEN]);
void gen_preambule(void);
void gen_data_section(void);
void gen_preambule_code(void);
void gen_epilog_code(void);
void gen_read(char*, int);
void gen_write(char*, int);
void gen_label_name(char*);
void gen_label_func_name(char*);
void gen_label(char*);
void gen_cond_jump(char*);
void gen_incond_jump(char*);
void gen_func_jump(char*);
void gen_bool(int);
void gen_return();
void gen_bool_label_name(char*);
void gen_assign(char*, int);
void gen_return();

#endif //_GEN_H_