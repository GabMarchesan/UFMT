/**
 * @file synt.c
 * @author Gabriel Gomes Marchesan
 * @brief Codificacao do modulo do analisador sintatico
 * @version 0.1
 * @date 2025-02-17
 * 
 */
// Inclusão do cabeçalho
#include "synt.h"

/**
 * @brief Verifica se o proximo caracter (a frente) na cadeia eh o esperado
 *
 * @param token_tag (int) codigo do token a ser verificado
 * @return int true/false
 */
int match(int token_tag){
    if (lookahead->tag == token_tag)
    {
        lookahead = getToken(); // Pega o proximo token por meio do lexico
        return true;
    }
    return false;
}

/**
 * @brief Regra de derivacao da gramatica: FACTOR
 *
 * @return int true/false
 */
int factor() {
    if (number())
        return true;
    return false;
}

/**
 * @brief Regra de derivacao da gramatica: TERM
 *
 * @return int true/false
 */
int term(){
    if (factor() && termR())
        return true;
    return false;
}

/**
 * @brief Regra de derivacao da gramatica: TERMR
 *
 * @return int true/false
 */
int termR(){
    int test1, test2;
    if (lookahead->tag == MULT){
        test1 = match(MULT);
        test2 = factor();
        genMult();
        if (test1 && test2)
            return termR();
        return false;

    } else if (lookahead->tag == DIV) {
        test1 = match(DIV);
        test2 = factor();
        genDiv();
        if (test1 && test2)
            return termR();
        return false;
    }

    return true;
}

/**
 * @brief Regra de derivacao da gramatica: EXPR
 *
 * @return int true/false
 */
int expr(){
    if (term() && exprR())
        return true;
    return false;
}

/**
 * @brief Regra de derivacao da gramatica: EXPR
 *
 * @return int true/false
 */
int exprR(){
    int test1, test2;
    if (lookahead->tag == PLUS)
    {
        test1 = match(PLUS);
        test2 = term();
        genAdd();
        if (test1 && test2)
            return exprR();
        return false;

    }else if (lookahead->tag == MINUS){
        test1 = match(MINUS);
        test2 = term();
        genSub();
        if (test1 && test2)
            return exprR();
        return false;
    }
    return true; 
}

/**
 * @brief Regra de derivacao da gramatica: NUMBER
 *
 * @return int true/false
 */
int number() {
    char aux_lexema[MAX_TOKEN];
    strcpy(aux_lexema, lookahead->lexema);

    if (match(NUM)) {
        genNum(aux_lexema);
        return true;
    }
    return false;
}

/**
 * @brief Funcao principal (main) do compilador
 * 
 * @return int 
 */
int main(int argc, char *argv[]) {
    int acept = 0;

    if (argc < 2) {
        printf("Uso: %s <arquivo de entrada>\n", argv[0]);
        return 1;
    }

    initLex(argv[1]); // Passa o nome do arquivo para initLex()
    lookahead = getToken(); // Inicialização do lookahead

    acept = expr(); // Chamada da derivação/função inicial da gramática

    // Verifica se a cadeia pertence à linguagem
    if (acept) {
        printf("\tCadeia PERTENCENTE à linguagem\n");
    } else {
        printf("\tCadeia NÃO pertencente à linguagem\n");
    }

    return 0;
}