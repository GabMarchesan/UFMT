/**
 * UFMT -- Ciencia da Computacao
 * Compiladores - Prof. Ivairton
 * Aluno: Gabriel Gomes Marchesan
 * 
 *Tarefa 2 - Nova versão do código Exemplo 2 
 *(implementar geração de código com registradores)
**/
#include <stdio.h>
#include <stdlib.h>

//Constantes
#define MAX_CHAR 32
#define true 1
#define false 0

//Variaveis globais
int pos = 0;
char string[MAX_CHAR];
char lookahead;

//Prototipos
int list();
int listR();
int digit();
int match(char c);
void nextToken(); // nextToken - quer saber oq representa

//Verifica o proximo caracter (a frente) na cadeia eh o esperado
int match(char c) {
    if ( lookahead == c ) { //Verifica se o próximo caractere é igual ao caractere que entreguei como parametro
        nextToken();
        return true; // Enquanto estiver batendo os resultados, retorna verdadeiro(deu certo)
    }
    return false; // Se não bateu os resultados, ocorreu uma falha
}

//Funcao que passa ao proximo simbolo da cadeia
void nextToken() { 
    pos++;
    lookahead = string[pos]; //lookahead - token atual
}

//Funcao da regra de derivacao 'list'
int list() { // Vão ser do tipo inteiro, pois o código só vai funcionar se todas as regras funcionare, caso ocorra erro vai funcionar
    if ( digit() && listR() ) // Somente se as duas funções devolverem verdadeiro a função irá retornar verdadeiro
        return true;
    return false; // se retornar falso, ocorreu erro em alguma parte da função
}

//Funcao da regra de derivacao 'listR'
int listR() {
    int test1, test2; // Facil de ver(Próximos códigos não aplicar!)
    if ( lookahead == '+' ) { //lookahead ainda não é token, é caracter 
        test1 = match('+'); //match - Vai consumir o simbolo e chamar o próximo token
        test2 = digit();

        //Adicao de comando para geracao de codigo:
        printf("pop rax\n"); //Remove um valor do topo da pilha e o armazena no registrador RAX
        printf("pop rbx\n"); //Remove outro valor do topo da pilha e o armazena no registrador RBX
        printf("add rax,rbx\n"); //Soma o valor em RAX com o valor em RBX e armazena o resultado em RAX
        printf("push rax\n"); //Coloca o valor armazenado em RAX de volta no topo da pilha

        if (test1 && test2)
            return listR();
        return false;

    } else if ( lookahead == '-' ) {
        test1 = match('-');
        test2 = digit();

        //Adicao de comando para geracao de codigo:
        printf("pop rbx\n"); //Remove um valor do topo da pilha e o coloca no registrador RBX
        printf("pop rax\n"); //Remove outro valor da pilha e o coloca no registrador RAX
        printf("sub rax,rbx\n"); //Subtrai o valor em RBX do valor em RAX e armazena o resultado em RAX
        printf("push rax\n"); //oloca o resultado da subtração de volta no topo da pilha

        if (test1 && test2)
            return listR();
        return false;

    } else {
        if ( match('\0') ) // Verifica se é final de cadeia, se for
            return true; // Devolve verdadeiro
        printf("Erro: Caractere '%c' inesperado na posição %d.", lookahead, pos);
        return false; // Caso não seja retorna falso
    }
}

//Funcao da regra de derivacao 'digit''
int digit() { // Verificar se os digitos é verdadeiro, dos que eu espero
    
    char ch;
    ch = lookahead;

    
    if ( 
        match('0') || match('1') ||
        match('2') || match('3') ||
        match('4') || match('5') ||
        match('6') || match('7') ||
        match('8') || match('9') ) 
    {
        //Adicao de comando para geracao de codigo:
        printf("mov rax %c\n", ch);
        printf("push rax\n");
        return true;
    }
    printf("Erro: Esperado um dígito na posição %d, mas encontrado '%c'.", pos, lookahead);
    return false;
}


//Funcao principal
int main() {

    int acept = false; // Verificar se reconheceu ou não reconheceu de acordo com a gramatica

    printf("Informe uma cadeia:");
    scanf("%s", string); //leitura da cadeia
    
    lookahead = string[0]; //inicializacao do lookahead

    acept = list(); //Chamada da derivacao/funcao inicial da gramatica
    // Chama a função list primeiro, pois é a primeira chamada da gramatica

    if (acept) {
        printf("\nCadeia PERTENCENTE a linguagem\n"); // Se for verdadeiro retorna isso
    } else {
        printf("\nCadeia NAO pertencente a linguagem\n"); // Caso de alguma falha, irá retornar essa mensagem
    }

    return 1;
}