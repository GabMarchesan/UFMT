/**
 * @file lex.c
 * @author Gabriel Gomes Marchesan
 * @brief Codificacao do modulo do analisador lexico
 * @version 0.1
 * @date 2025-02-17
 * 
 */

// Inclusao do cabecalho
#include "lex.h"

int pos;
char string[MAX_CHAR];


/**
 * @brief Processo de inicializacao do lexico. Tem funcao de receber o codigo
 *        de entrada.
 * 
 */
void initLex(const char *filename) {

    // Abra o arquivo para leitura
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file); 
    fseek(file, 0, SEEK_SET); 

    // Aloca espaço para o buffer
    char *file_content = (char*) malloc(file_size + 1);

    // Lê o conteúdo do arquivo para o buffer
    fread(file_content, 1, file_size, file);
    file_content[file_size] = '\0'; 

    fclose(file);

    // Atribua o buffer ao seu ponteiro global "string"
    strcpy(string, file_content);

    // Libere o buffer de conteúdo do arquivo
    free(file_content);

    pos = 0;
}

/**
 * @brief Analisa o proximo comando no código de entrada e retorna estrutura de
 * dados com Token correspondente (ponteiro).
 * 
 * @return type_token* 
 */
type_token *getToken() {
    char buffer[MAX_CHAR];
    int pos_buffer;
    type_token *token;
    
    pos_buffer = 0;
    token = (type_token*) malloc(sizeof(type_token));

    // Consome espacos
    while ( isspace(string[pos]) ) {
        pos++;
    }

    // Verifica se NUMERO
    if ( isdigit(string[pos]) ) {
        // constroi buffer com os digitos
        while ( isdigit(string[pos]) ) {
            buffer[pos_buffer++] = string[pos++];
        }
        buffer[pos_buffer] = '\0';
        token->tag = NUM;
        strcpy( token->lexema, buffer ); //copia buffer para lexema
    } 
    //Verifica se PLUS (+)
    else if (string[pos] == PLUS) {
        token->tag = PLUS;
        strcpy(token->lexema, "+");
        pos++;
    }
    //Verifica se MINUS (-)
    else if (string[pos] == MINUS) {
        token->tag = MINUS;
        strcpy(token->lexema, "-");
        pos++;
    }
    //Verifica se MULT (*)
    else if (string[pos] == MULT) { 
        token->tag = MULT;
        strcpy(token->lexema, "*");
        pos++;
    }
    //Verifica se DIV (/)
    else if (string[pos] == DIV) { 
        token->tag = DIV;
        strcpy(token->lexema, "/");
        pos++;
    }
    //Verifica se FIM DE CADEIA
    else if (string[pos] == ENDTOKEN) {
        token->tag = ENDTOKEN;
        strcpy(token->lexema, "");
    }
    //ERRO
    else {
        token->tag = ERROR;
        strcpy(token->lexema, "");
    }

    return token;
}