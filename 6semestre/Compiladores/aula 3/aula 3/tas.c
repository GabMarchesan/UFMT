/**
 * UFMT -- Ciencia da Computacao
 * Compiladores - Prof. Ivairton
 * 
 * Aluno: Gabriel Gomes Marchesan
 * 
 * Exemplo 4 - Analisador sintático a partir da TAS
**/

//Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Constantes
#define MAX_CHAR 32
#define true 1
#define false 0
#define ERROR -1

//Regras e simbolos da gramatica
#define E  0
#define ER 1
#define T  2
#define TR 3
#define F  4

//Terminais:
#define id    0
#define sum   1
#define mult  2
#define o_par 3
#define c_par 4
#define end   5
#define empty 6

//Definicao de tipos
//Tipo array int
struct arr_int{
    int arr[MAX_CHAR]; 
    int cont;  
    char *lexema[MAX_CHAR];//vetor que guarda o lexema das variaveis
};
typedef struct arr_int typeArrayInt;

//Pilha
struct t_pilha {
    int array[MAX_CHAR];
    int count;
};
typedef struct t_pilha TipoPilha;


//Variaveis globais
char string[MAX_CHAR];
char lookahead;
char *pilha_char[MAX_CHAR];//guarda o lexema das variaveis
int pos;//posição do lookahead
TipoPilha pilha;

//Tabela tas
typeArrayInt TAS[5][6] = {
                            {{T,ER},{2},{ERROR},{ERROR},{T,ER},{ERROR},{ERROR}},
                            {{ERROR},{sum,T,ER},{ERROR},{ERROR},{empty},{empty}},
                            {{F,TR},{ERROR},{ERROR},{F,TR},{ERROR},{ERROR}},
                            {{ERROR},{empty},{mult,F,TR},{ERROR},{empty},{empty}},
                            {{id},{ERROR},{ERROR},{o_par,E,c_par},{ERROR},{ERROR}}
                        };

//Prototipos
void empilha(int);
int desempilha();
int match(int);
void nextToken(); 
int digit(); 
int isNumber(char); 
int verificaLookahead(); 
int driver();

//Manipulacao da pilha
void empilha(int valor){
    
    pilha.array[pilha.count] = valor;
    pilha.count++;
    
}

int desempilha() {
    printf("Desempilhando: [%s]\n", pilha_char[pilha.count-1]);
    pilha.count--;
    pilha_char[pilha.count] = '\0';
    return pilha.array[pilha.count];

}

//Verifica o proximo caracter (a frente) na cadeia eh o esperado
int match(int c) {
    int entrada = verificaLookahead();

    if (entrada == c){
        if (c == id) {
            digit();
        }
        nextToken();
        return true;  
    }else if(c == empty){
        return true;
    }
    printf("ERROR ---> O caracter não foi reconhecido\n");
    exit(-1);
    
}

//Funcao que passa ao proximo simbolo da cadeia
void nextToken() {  
    pos++;
    lookahead = string[pos];

}

//Funcao da regra de derivacao 'digit''
int digit() {
    char ch = lookahead;
    if (isNumber(ch)) {
        printf("mov rax, %c\n", ch);
        printf("push rax\n");
        nextToken();
        return true;
    }
    return false;
}

//Is Number
int isNumber(char ch) {
    return ch >= '0' && ch <= '9';
}

//Verifica o lookahead
int verificaLookahead() {
    if (isNumber(lookahead)) return id;
    if (lookahead == '+') return sum;
    if (lookahead == '-') return sum;
    if (lookahead == '*') return mult;
    if (lookahead == '(') return o_par;
    if (lookahead == ')') return c_par;
    if (lookahead == '\0') return end;
    return ERROR;
}

//ListR
int listR() {
    int test1, test2;
    if (lookahead == '+') {
        test1 = match('+');
        test2 = digit();
        printf("pop rbx\n");
        printf("pop rax\n");
        printf("add rax, rbx\n");
        printf("push rax\n");
        if (test1 && test2) 
            return listR();
        return false;
    } else if (lookahead == '-') {
        test1 = match('-');
        test2 = digit();
        printf("pop rbx\n");
        printf("pop rax\n");
        printf("sub rax, rbx\n");
        printf("push rax\n");
        if (test1 && test2) 
            return listR();
        return false;
    } else {
        if (match('\0')) return true;
        return false;
    }
}

//Funcao principal
int main() {

    //TAS inicializada em sua declaracao, conforme conteudo de aula.

    pilha.count = 0; //Inicializacao do contador da pilha
    empilha(end);
    empilha(E);

    int acept = false;

    printf("Informe uma cadeia:\n\n");
    //scanf("%s", string); //leitura da cadeia
    strcpy(string,"2+3");
    
    lookahead = string[0]; //inicializacao do lookahead

    acept = listR(); //Chamada da derivacao/funcao inicial da gramatica

    if (acept) {
        printf("\nCadeia PERTENCENTE a linguagem!\n");
    } else {
        printf("\nCadeia NAO pertencente a linguagem.\n");
    }

    return 1;
}