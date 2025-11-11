/**
 * @file synt.c
 * @author Edilayne Ramos e Gabriel Marchesan
 * @brief Codificacao do modulo do analisador sintatico
 * @version 0.5
 * @date 2025-05-03
 * 
 */

// Inclusao do cabecalho
#include "synt.h"
#include "gen.h"

// Variaveis globais
type_token *lookahead;
extern type_symbol_table_variables *local_symbol_table_variables;
extern type_symbol_table_variables global_symbol_table_variables;
extern type_symbol_table_string symbol_table_string;
extern char output_file_name[MAX_CHAR];
extern FILE *output_file;

/**
 * @brief Verifica se o proximo caracter (a frente) na cadeia eh o esperado
 * 
 * @param token_tag (int) codigo do token a ser verificado 
 * @return int true/false
 */
int match(int token_tag) {
    if ( lookahead->tag == token_tag ) {
        lookahead = getToken(); //Pega o proximo token por meio do lexico
        return true;
    }
    printf("[ERRO] Entrada esperada: %s\n", lookahead->lexema);
    return false;
}

/**
 * @brief Regra de derivacao inicial
 */
void program (void) {
    gen_preambule(); //Temporariamente cria um preambulo adicional que permite o uso das funcoes scanf e printf
    declarations();
    gen_preambule_code(); //Chamada do gerador de codigo para escrita do cabecalho da secao de codigo
    
    if (lookahead->tag == BEGIN) {
        match(BEGIN);

        //aloca espaço da tabela de variaveis locais
        local_symbol_table_variables = (type_symbol_table_variables*) malloc(sizeof(type_symbol_table_variables));
        local_symbol_table_variables->next = NULL;

        statements();

        global_symbol_table_variables.next = local_symbol_table_variables;

        if (lookahead->tag == END) {
            match(END);
            func_code();
            gen_epilog_code();
            gen_data_section(); //Chamada do gerador de codigo para declaracao de variaveis
        } else {
            printf("[ERRO] Não encontrado o END da função principal.\n");
        }
    } else {
        printf("[ERRO] Não encontrada o BEGIN da função principal.\n");
    }
}

/**
 * @brief Regra de derivacao para declaracoes
 */
void declarations(void) {
    while ( declaration() ); //Laco para processamento continuo das declaracoes
}

/**
 * @brief Regra de derivacao declaracao
 * @return int true/false
 */
int declaration (void) {
    type_symbol_table_entry *search_symbol_var;
    type_symbol_function_entry *search_symbol_func;
    char name[MAX_CHAR];
    int type = lookahead->tag;

    //Verifica o tipo da variavel
    type = lookahead->tag;
    
    if ( type == INT || type == FLOAT || type == CHAR || type == STRING) { 
        match(type);
        strcpy(name, lookahead->lexema);

        if ( match(ID) ) {

            if (lookahead->tag == SEMICOLON){ //caso de declaracao de variavel
                search_symbol_var = sym_find(name, &global_symbol_table_variables );

                    if ( (search_symbol_var != NULL) ) {
                        printf ("[ERROR] Variavel '%s' ja declarada.\n", name); 
                        return false;
                    } else {
                        return declarationV(name, type);
                    }

            } else if (lookahead->tag == OPEN_PAR) { //caso de declaraco de funcao
                search_symbol_func = sym_func_find(name);
                    
                if ( (search_symbol_func != NULL) ) {
                    printf ("[ERROR] Funcao '%s' ja declarada.\n", name); 
                    return false;
                } else {
                    return declarationF(name, type);
                }

            } else {
                printf("[ERROR] Nao foi possivel identificar se é variavel ou funcao.\n");
                return false;
            }
        } else {
            printf("[ERROR] Deveria existir um ID.\n");
            return false;
        }

    } else if (lookahead->tag == ENDTOKEN ||
                lookahead->tag == READ ||
                lookahead->tag == WRITE ||
                lookahead->tag == BEGIN) {
                
        return false;         
    } else {
        printf ("[ERRO] Tipo desconhecido: %d %s.\n", lookahead->tag, lookahead->lexema);
        return false; 
    }
}

/**
 * @brief Regra de derivacao declaracaoV, responsavel por declarar uma variavel
 * @param var_name nome da variável
 * @param var_type tipo da variável
 * @return int true/false
 */
int declarationV(char *var_name, int var_type) {
    
    int ok1;
    
    ok1 = match(SEMICOLON);
    
    if ( ok1 == true ) {
        sym_declare( var_name, var_type, 0, &global_symbol_table_variables);
        return true;
    } else {
        printf("[ERROR] Deveria existir um ';'.\n");
        return false;
    }

}

/**
 * @brief Regra de derivacao declaracaoF, responsavel por declarar uma funcao
 * @param func_name nome da função
 * @param func_type tipo da funcao
 * @return int true/false
 */
int declarationF(char *func_name, int func_type) {
    
    int nparams = 0;
    int ok1, ok2;
    type_symbol_table_entry params[MAX_PARAMS];

    if ( match(OPEN_PAR) ) {

        int end = false;
        int var_typ;
        
        while ( (end != true)) {

            char var_name[MAX_CHAR];
            var_typ = lookahead->tag;

            if (var_typ == INT || var_typ == FLOAT || var_typ == CHAR || var_typ == STRING) {
                match(var_typ);
                
                if(lookahead->tag == ID){

                    strcpy(var_name, lookahead->lexema);
                    match(ID);
                    strncpy(params[nparams].name, var_name, MAX_TOKSZ);
                    params[nparams].type = var_typ;
                    nparams++;

                    //verifica se há uma virgula ou se acabou os parametros
                    if (lookahead->tag == COMMA) {
                        match(COMMA);
                    } else if (lookahead->tag == CLOSE_PAR) {
                        end = true;
                    } else {
                        printf("[ERRO] Esperado ',' ou ')' apos a declaracao de variavel.\n");
                        return false;
                    }
                } else {
                    printf("[ERROR] Esperado ID identificador da variavel da funcao\n"); 
                    return false;
                }
            } else {
                //caso n tenha parametros
                end = true;
            }
        }
    } 

    if (lookahead->tag == CLOSE_PAR) {
        ok1 = match(CLOSE_PAR);
        ok2 = match(SEMICOLON);
        if (ok1 == true && ok2 == true) {

            char label[MAX_CHAR];
            gen_label_func_name(label);
            sym_func_declare(func_name, func_type, params, nparams, label);
            return true;

        } else {
            printf("[ERRO] Esperado ';' ou ')' apos a declaracao de funcao.\n");
            return false;
        }
    }
    return false;
}

/**
 * @brief Regra de derivacao para comandos
 */
void statements (void) {
   while ( statement() );  //processa enquanto houver comandos
}

/**
 * @brief Regra de derivacao que processa os comandos
 * 
 * @return int true/false
 */
int statement (void) {

    char lexeme_of_id[MAX_CHAR];
    type_symbol_table_entry *search_symbol_local;
    type_symbol_table_entry *search_symbol;
    type_symbol_table_string_entry *gen_string;
    int ok1, ok2, type;
    char string_value[MAX_CHAR];

    if (lookahead->tag==INT||lookahead->tag==FLOAT||lookahead->tag==CHAR||lookahead->tag==STRING) {
        int type = lookahead->tag;
        match(type);
        char name[MAX_CHAR];
        strcpy(name, lookahead->lexema);
        
        if ( match(ID) ) {

            if (lookahead->tag == SEMICOLON){ 
        
                type_symbol_table_entry *search_symbol_var;
                search_symbol_var = sym_find(name, local_symbol_table_variables);
                
                if ( (search_symbol_var != NULL) ) {
                    printf ("[ERROR] Variavel '%s' ja declarada.\n", name); 
                    return false;
                } else {

                    int ok1;
                    ok1 = match(SEMICOLON);
                    
                    if ( ok1 = true ) {
                        sym_declare( name, type, 0, local_symbol_table_variables);
                        return true;

                    } else {
                        printf("[ERROR] Deveria existir um ';'.\n");
                        return false;
                    }
                        
                }

            } else {
                printf("[ERROR] Nao foi possivel declarar a variavel, pois nao ha ;\n");
                return false;
            }

        } else {
            printf("[ERROR] Deveria existir um ID.\n");
            return false;
        }
        
    } else if (lookahead->tag == READ) {
        match(READ);
        strcpy(lexeme_of_id, lookahead->lexema);
        ok1 = match(ID);
        search_symbol = sym_find(lexeme_of_id, &global_symbol_table_variables);
        search_symbol_local = sym_find(lexeme_of_id, local_symbol_table_variables);
        
        if (search_symbol_local != NULL) {
            type = search_symbol_local->type;
            gen_read(lexeme_of_id, type);
            ok2 = match(SEMICOLON);
            return ok1 && ok2;
            
        } else if (search_symbol != NULL){
            type = search_symbol->type;
            gen_read(lexeme_of_id, type);
            ok2 = match(SEMICOLON);
            return ok1 && ok2;

        } else {
            printf("[ERRO] Simbolo desconhecido (Variavel nao declarada): %s\n", lexeme_of_id);
            return false;
        }
    } else if (lookahead->tag == WRITE) {
        match(WRITE);
        if (lookahead->tag == STRING) {
            strcpy(string_value, lookahead->lexema);
            gen_string = sym_string_declare(string_value);
            match(STRING);
            if (gen_string != NULL) {
                strcpy(lexeme_of_id, gen_string->name);
                gen_write(lexeme_of_id, STRING);
                match(SEMICOLON);
            }
        } else if (lookahead->tag == ID) {
            strcpy(lexeme_of_id, lookahead->lexema);
            match(ID);
            search_symbol = sym_find(lexeme_of_id, &global_symbol_table_variables); // Erro aqui
            if (search_symbol != NULL) {
                type = search_symbol->type;
                gen_write(lexeme_of_id, type);
                match(SEMICOLON);
                return true;
            } else {
                printf("[ERRO] Simbolo desconhecido (Variavel nao declarada): %s\n", lexeme_of_id);
                return false;
            }
        }
    } else if (lookahead->tag == IF) {
        char label_else[MAX_CHAR];
        char label_end[MAX_CHAR];
        gen_label_name(label_else);
        gen_label_name(label_end);

        match(IF);
        match(OPEN_PAR);
        B(); //Expressao booleana
        gen_cond_jump(label_else);
        match(CLOSE_PAR);
        
        match(BEGIN);
        statements();
        match(END);

        gen_incond_jump(label_end);
        gen_label(label_else);

        //Verifica se ocorre um ELSE
        if (lookahead->tag == ELSE) {
            match(ELSE);
            match(BEGIN);
            statements();
            match(END);
        }
        gen_label(label_end);
        return 1;

    } else if (lookahead->tag == WHILE) {
        char label_begin[MAX_CHAR];
        char label_end[MAX_CHAR];
        gen_label_name(label_begin);
        gen_label_name(label_end);

        match(WHILE);
        match(OPEN_PAR);
        gen_label(label_begin);
        B(); //Expressao booleana
        gen_cond_jump(label_end);
        match(CLOSE_PAR);
        match(BEGIN);
        printf("Entrei no WHILE\n");
        statements();
        match(END);
        printf("Sai no WHILE\n");
        gen_incond_jump(label_begin);
        gen_label(label_end);
        return true;
    } else if (lookahead->tag == ID){
        
        char name[MAX_CHAR];
        strcpy(name, lookahead->lexema);
        match(ID);

        if (lookahead->tag == ASSIGN) {
            match(ASSIGN);
            search_symbol = sym_find(name, local_symbol_table_variables);

            if (search_symbol != NULL) {
                type = search_symbol->type;

                if (lookahead->tag == ID ||lookahead->tag == '(' || lookahead->tag == NUM){
                    E();
                }

                gen_assign(name, type);
                match(SEMICOLON);
                return true;

            } else {

                search_symbol = sym_find(name, &global_symbol_table_variables);
                
                if (search_symbol != NULL) {

                    type = search_symbol->type;

                    if (lookahead->tag == ID ||lookahead->tag == '(' || lookahead->tag == NUM){
                        E();
                    }

                    gen_assign(name, type);
                    match(SEMICOLON);
                    return true;

                } else {
                    printf("[ERROR] Simbolo desconhecido (Variavel nao declarada): %s\n", name);
                    return false;
                }
            }

        } else if (lookahead->tag == OPEN_PAR) {
            int ok;
            ok = func_call_cmd(name);
            return ok;

        } else {
            printf("[ERROR] Simbolo desconhecido. Esperava '=' ou '('\n");
            return false;
        }

    } else if (lookahead->tag == ENDTOKEN || lookahead->tag == END) {
        return false;
    } else {
        printf("[ERRO] Comando desconhecido.\nTag=%d; Lexema=%s\n",lookahead->tag, lookahead->lexema);
        return false;
    }
}

int func_code(){

    printf("\n\n==============ABRI FUNC CODE===============\n\n");
    int type;
    type = lookahead->tag;
        
    if ( type == INT || type == FLOAT || type == CHAR || type == STRING){
        match(type);

        if (lookahead->tag == ID){
                
            char func_name[MAX_CHAR];
            strcpy(func_name, lookahead->lexema);

            type_symbol_table_entry *search_symbol;
            type_symbol_function_entry *search_symbol_func;
            search_symbol_func = sym_func_find(func_name);

            //verifica se a funçào existe na tabela
            if (search_symbol_func == NULL) {
                printf("[ERRO] A funcao chamada não existe na tabela de funcoes.\n");
                return false;
            } else {
                match(ID);

                //aloca espaço da tabela de variaveis locais
                local_symbol_table_variables = (type_symbol_table_variables*) malloc(sizeof(type_symbol_table_variables));
                local_symbol_table_variables->next = NULL;

                if (lookahead->tag == OPEN_PAR){                    
                    match(OPEN_PAR);

                    int end = false;
                    int nparams = search_symbol_func->nparams;
                    int nparamsDados = 0;
                    int i = 0; //vezes rodadas
                    int type_param = lookahead->tag;
                    char name_param[MAX_CHAR];

                    while (end == false && nparams > 0) {

                        match(type_param);
                        strcpy(name_param, lookahead->lexema);

                        if ( (search_symbol_func->params[nparamsDados].type == type_param) && (strcmp(search_symbol_func->params[nparamsDados].name,name_param) == 0)){
                            printf("\nO TIPO NA TABELA EH %d E O TIPO NO PARAMETRO EH %d\n", search_symbol_func->params[nparamsDados].type, type_param );
                            printf("O NOME NA TABELA EH %s E O NOME NO PARAMETRO EH %s\n", search_symbol_func->params[nparamsDados].name, name_param );
                            sym_declare( name_param, type_param, 0, local_symbol_table_variables);
                            match(ID);
                            nparamsDados++;

                            if (lookahead->tag == CLOSE_PAR){ //verifica se a implementação da funçao acabou

                                if (nparams != nparamsDados){ //verifica se a qtd de parametros dados n bate corretamenta
                                    printf("[ERROR] Faltou passar %d parametros na implementacao da função\n", (nparams - nparamsDados));
                                    return false;

                                } else { //caso a qtd bata
                                    end = true; // sai da verifica dos parametros
                                }

                            } else {
                                match(COMMA);
                                type_param = lookahead->tag;
                            }

                        } else {
                            printf("[ERRO] Parametro não é do mesmo tipo ou nome que o declarado no protótipo.\n");
                            return false;
                        }

                        i++;
                    }

                    if (match(CLOSE_PAR)) {
                        if (match(BEGIN)) {
                            gen_label(search_symbol_func->label); // Gera o rótulo
                            fprintf(output_file, ";prologo da funcao\n");
                            fprintf(output_file, "push rbp\n");
                            fprintf(output_file, "mov rbp, rsp\n");
                            statements();
                            global_symbol_table_variables.next = local_symbol_table_variables;
                            if (match(END)) {
                                fprintf(output_file, ";epilogo da funcao\n");
                                fprintf(output_file, "mov rsp, rbp\n");
                                fprintf(output_file, "pop rbp\n");
                                gen_return();
                                printf("FUNCAO IMPLEMENTADA COM SUCESSO!\n");
                                printSTVariables(local_symbol_table_variables);
                                printf("\nvou atualizar a flag da funcao %s\n", func_name);
                                atualiza_flag(func_name);
                                func_code();
                            }
                        }
                    }
                
                }
            }
        } else {
            printf("[ERROR] não foi inserido o nome da função na sua implementação\n");
            return false;
        }

    } else {

        int ok1;
        printf("CHEGUEI AQUI ONDE AS FUNCOES FORAM IMPLEMENTADAS!\n");
        //faz um for verificando se todas as funções (flag) foram implementadas
        ok1 = todas_implemetadas();

        if (ok1 == true){
            printf("todas as funcoes foram implementadas!\n");
            return true;
        } else {
            printf("[ERROR] alguma funcao n foi implementada!\n");
            return false;
        }

    }

}

/**
 * @brief Chama uma funçao que exista
 * 
 * @param func_name nome da função
 * @return int 
 */
int func_call_cmd(char *func_name) {
    type_symbol_function_entry *search_symbol_func = sym_func_find(func_name);
    
    if (search_symbol_func == NULL) {
        printf("[ERRO] A funcao chamada não existe na tabela de funcoes.\n");
        return false;
    }

    if (!match(OPEN_PAR)) {
        printf("[ERROR] Esperado '(' na chamada da função\n");
        return false;
    }

    int end = false;
    int nparams = search_symbol_func->nparams;
    int nparamsDados = 0;

    while (end == false && nparams > 0) {
        type_symbol_table_entry *search_symbol = NULL;
        int param_type = search_symbol_func->params[nparamsDados].type;

        if (lookahead->tag == ID) {
            search_symbol = sym_find(lookahead->lexema, &global_symbol_table_variables);
            if (search_symbol == NULL) {
                printf("[ERRO] A variavel passada no parametro não foi declarada.\n");
                return false;
            }
            if (param_type != search_symbol->type) {
                printf("[ERRO] Parametro não é do mesmo tipo que o declarado no protótipo.\n");
                return false;
            }
            match(ID);
            nparamsDados++;
        } else if (lookahead->tag == NUM || lookahead->tag == OPEN_PAR) {
            if (param_type != INT && param_type != FLOAT) {
                printf("[ERRO] Expressao aritmetica incompativel com tipo %d\n", param_type);
                return false;
            }
            E(); // Processa expressão aritmética
            nparamsDados++;
        } else {
            printf("[ERRO] Parametro esperado (ID, NUM ou expressao).\n");
            return false;
        }

        if (lookahead->tag == CLOSE_PAR) {
            if (nparams != nparamsDados) {
                printf("[ERROR] Faltou passar %d parametros na chamada da função\n", (nparams - nparamsDados));
                return false;
            }
            end = true;
        } else {
            if (!match(COMMA)) {
                printf("[ERROR] Esperado ',' entre parâmetros\n");
                return false;
            }
        }
    }

    int ok = match(CLOSE_PAR);
    int ok2 = match(SEMICOLON);

    if (ok && ok2) {
        gen_func_jump(search_symbol_func->label);
        return true;
    } else {
        if (!ok) printf("[ERROR] não há um ')' na chamada da função\n");
        if (!ok2) printf("[ERROR] não há um ';' na chamada da função\n");
        return false;
    }
}

/**
 * @brief Regra de derivação que analiza expressoes booleanas
 *        no padrao 'id op_rel expr'
 * 
 */
int B() {

    int operator;

    //caso comece com o ID
    if(lookahead->tag == ID){
        char lexema_of_id[MAX_CHAR];
        strcpy(lexema_of_id, lookahead->lexema);
        match(ID);

    //caso comece com expressão aritmetica
    } else if (lookahead->tag == '(' || lookahead->tag == NUM){
        E();

    //caso erro
    } else {
        printf("[ERROR] ID, número ou expressão aritmética não experada\n");
        return false;
    }

    //verifica o operador
    if (boolOperator(&operator)){

        //verifica oq vem depois do operador
        //caso ID
        if (lookahead->tag == ID){
            
            char lexema_of_id2[MAX_CHAR];
            strcpy(lexema_of_id2, lookahead->lexema);

            match(ID);
            gen_bool(operator);
            return true;

        //caso Exp_aritmetica ou numero
        } else if (lookahead->tag == '(' || lookahead->tag == NUM ){
            E();
            gen_bool(operator);
            return true;
        
        //caso erro
        } else {
            printf("[ERROR] ID, número ou expressão aritmética não experada\n");
            return false;
        }

    //caso operador n seja reconhecido
    } else {
        printf("[ERROR] Operador desconhecido: %s\n", lookahead->lexema);
        return false;
    }
}

int boolOperator(int *operator) {
    int lookahead_tag;
    lookahead_tag = lookahead->tag;
    printf("A TAG EH %d\n", lookahead_tag);
    *operator = -1;
    
    if (lookahead_tag == EQUAL ||
        lookahead_tag == NE ||
        lookahead_tag == LT ||
        lookahead_tag == GT ||
        lookahead_tag == LE ||
        lookahead_tag == GE) {
            *operator = lookahead_tag;
            match(lookahead_tag);
            return true;
    } else {
        printf("[ERRO] Operador relacional experado.\n");
        return false;
    }
}

//------
/* Funções que representam a gramatica que reconhece expressoes aritmeticas */
/* Elaborada nas primeiras aulas */
int E() {
    int b1, b2;
    b1 = T();
    if (b1) 
        b2 = ER();
    return b1 && b2;
}

int ER() {
    if (lookahead->tag == '+')  { 
        int b1, b2;
        match('+');
        b1 = T();
        genAdd();
        if (b1) 
            b2 = ER();
        return b1 && b2;
    } else if (lookahead->tag == '-') {
        int b1, b2;
        match('-');
        b1 = T();
        genSub();
        if (b1)
            b2 = ER();
        return b1 && b2;      
    } else if (lookahead -> tag == ')') {
        return true;
    } else if (lookahead -> tag == ENDTOKEN) {
        return true;
    } else if (lookahead -> tag == '*') {
        return true;
    } else if (lookahead -> tag == '/') {
        return true;
    } else {
        return false;
    }
}

int T() {
    int b1, b2;
    b1 = F();
    if (b1)
        b2 = TR();
    return b1 && b2;
}

int TR() {
    if (lookahead->tag == '*') { 
        int b1, b2;
        match('*');
        b1 = F();
        genMult();
        if (b1)
            b2 = TR();
        return b1 && b2;
    } else if (lookahead -> tag == '/') {
        int b1, b2;
        match('/');
        b1 = F();
        genDiv();
        if (b1)
            b2 = TR();
        return b1 && b2;
    } else if (lookahead->tag == ')') {
        return true;
    } else if (lookahead->tag == ENDTOKEN){ //EOF
        return true;
    } else if (lookahead->tag == '+') {
        return true;
    } else if (lookahead->tag == '-') {
        return true;
    } else {
        return false;
    }
}

int F() {
    if (lookahead->tag == '(') {
        int b1, b2;
        match('(');
        b1 = E();
        if (!b1) return false;
        b2 = match(')');
        return b1 && b2;
    
    } else if (lookahead->tag == NUM) {
        int b1;
        char lexema[MAX_TOKEN];
        strcpy(lexema, lookahead->lexema);
        b1 = match(NUM);
        genNum(lexema);
        return b1;
    
    } else if (lookahead->tag == ID) {
        int b1;
        char lexema[MAX_TOKEN];
        strcpy(lexema, lookahead->lexema);
        b1 = match(ID);
        genVar(lexema);
        return b1;

    } else {
        return false;
    }
}
/*fim da importação */
//------


//--------------------- MAIN -----------------------

/**
 * @brief Funcao principal (main) do compilador
 * 
 * @return int 
 */
int main(int argc, char *argv[]) {

    //Inicializa a tabela de simbolo global
    initSymbolTableVariables(&global_symbol_table_variables);
    initSymbolTableString();

    //Verifica a passagem de parametro
    if (argc != 2) {
        printf("[ERROR]\n\tÉ necessário informar um arquivo de entrada (código) como parâmetro.\n\n");
        exit(EXIT_FAILURE);
    }
    
    initLex(argv[1]); //Carrega codigo
    lookahead = getToken(); //Inicializacao do lookahead

    //Abre o arquivo de saida 
    strcpy(output_file_name, argv[1]);
    strcat(output_file_name, ".asm");
    output_file = fopen(output_file_name, "w+");

    program(); //Chamada da derivacao/funcao inicial da gramatica

    //printTSFunction();

    fclose(output_file);
    return 1;
}