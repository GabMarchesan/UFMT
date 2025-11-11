//Bibliotecas
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Definicao da capacidade do noh
#define T 3

//Definicao do  noh da arvore
struct est_no_m_filhos{
    int valores[T];
    struct est_no_m_filhos *filhos[T+1];
};

//apelido
typedef struct est_no_m_filhos tipo_no_m_filhos;

//prototipo das funcoes
tipo_no_m_filhos *alocaNoMFilhos(int);
void insereArvMFilhos(tipo_no_m_filhos **, int);
void percursoOrdemMFilhos(tipo_no_m_filhos *);
void percursoPreOrdemMFilhos(tipo_no_m_filhos *);
void percursoPosOrdemMFilhos(tipo_no_m_filhos *);
int alturaArvMFilhos(tipo_no_m_filhos *);
void imprimeNosPorNivelMFilhos(tipo_no_m_filhos*, int);
int contTotalnohMFilhos(tipo_no_m_filhos*);
int buscaMaiorValorMFilhos(tipo_no_m_filhos*);
int buscaMenorValorMFilhos(tipo_no_m_filhos*);
void removeArvMFilhos(tipo_no_m_filhos**, int);

tipo_no_m_filhos *alocaNoMFilhos(int valor) {
    tipo_no_m_filhos *novo_no = (tipo_no_m_filhos*) malloc(sizeof(tipo_no_m_filhos));
    novo_no->valores[0] = valor;
    for(int i = 1; i < T; i++) {
        novo_no->valores[i] = -1; // inicializa os demais valores com -1
    }
    for(int i = 0; i < T+1; i++) {
        novo_no->filhos[i] = NULL; // inicializa todos os filhos como nulos
    }
    return novo_no;
}

void insereArvMFilhos(tipo_no_m_filhos **raiz, int valor) {
    if(*raiz == NULL) {
        *raiz = alocaNoMFilhos(valor);
        return;
    }
    tipo_no_m_filhos *no_atual = *raiz, *no_pai = NULL, *novo_no = NULL;
    int i, j, valor_atual;
    while(no_atual != NULL) {
        no_pai = no_atual;
        valor_atual = no_atual->valores[0];
        if(valor == valor_atual) {
            return;
        }
        if(valor < valor_atual) {
            no_atual = no_atual->filhos[0];
        }
        else {
            for(i = 0; i < T-1 && no_atual->valores[i+1] != -1; i++) {
                if(valor > no_atual->valores[i] && valor < no_atual->valores[i+1]) {
                    no_atual = no_atual->filhos[i+1];
                    break;
                }
            }
            if(i == T-1 || no_atual->valores[i+1] == -1) {
                no_atual = no_atual->filhos[i+1];
            }
        }
    }
    novo_no = alocaNoMFilhos(valor);
    if(valor < valor_atual) {
        no_pai->filhos[0] = novo_no;
    }
    else {
        for(j = 0; j < T-1 && no_pai->valores[j+1] != -1; j++);
        no_pai->filhos[j+1] = novo_no;
    }
    for(j = T-2; j >= i; j--) {
        no_pai->valores[j+1] = no_pai->valores[j];
    }
    no_pai->valores[i] = valor;
}

void percursoOrdemMFilhos(tipo_no_m_filhos *raiz) {
    if(raiz == NULL) {
        return;
    }
    int i;
    for(i = 0; i < T; i++) {
        if(raiz->filhos[i] != NULL) {
            percursoOrdemMFilhos(raiz->filhos[i]);
        }
        if(raiz->valores[i] != -1) {
            printf("%d ", raiz->valores[i]);
        }
    }
    if(raiz->filhos[i] != NULL) {
        percursoOrdemMFilhos(raiz->filhos[i]);
    }
}

void percursoPreOrdemMFilhos(tipo_no_m_filhos *raiz) {
    if(raiz == NULL) {
        return;
    }
    int i;
    for(i = 0; i < T; i++) {
        if(raiz->valores[i] != -1) {
            printf("%d ", raiz->valores[i]);
        }
        if(raiz->filhos[i] != NULL) {
            percursoPreOrdemMFilhos(raiz->filhos[i]);
        }
    }
    if(raiz->filhos[i] != NULL) {
        percursoPreOrdemMFilhos(raiz->filhos[i]);
    }
}

void percursoPosOrdemMFilhos(tipo_no_m_filhos *raiz) {
    if(raiz == NULL) {
        return;
    }
    int i;
    for(i = 0; i < T; i++) {
        if(raiz->filhos[i] != NULL) {
            percursoPosOrdemMFilhos(raiz->filhos[i]);
        }
    }
    for(i = 0; i < T; i++) {
        if(raiz->valores[i] != -1) {
            printf("%d ", raiz->valores[i]);
        }
    }
    if(raiz->filhos[i] != NULL) {
        percursoPosOrdemMFilhos(raiz->filhos[i]);
    }
}

int alturaArvMFilhos(tipo_no_m_filhos *raiz) {
    if(raiz == NULL) {
        return -1;
    }
    int i, altura = 0, max_altura = 0;
    for(i = 0; i < T+1; i++) {
        altura = alturaArvMFilhos(raiz->filhos[i]);
        if(altura > max_altura) {
            max_altura = altura;
        }
    }
    return max_altura + 1;
}

void imprimeNosPorNivelMFilhos(tipo_no_m_filhos *raiz, int nivel) {
    if(raiz == NULL) {
        return;
    }
    if(nivel == 0) {
        int i;
        for(i = 0; i < T; i++) {
            if(raiz->valores[i] != -1) {
                printf("%d ", raiz->valores[i]);
            }
        }
    }
    else {
        int i;
        for(i = 0; i < T+1; i++) {
            imprimeNosPorNivelMFilhos(raiz->filhos[i], nivel-1);
        }
    }
}

int contTotalnohMFilhos(tipo_no_m_filhos *raiz) {
    if(raiz == NULL) {
        return 0;
    }
    int i, cont = 1;
    for(i = 0; i < T+1; i++) {
        cont += contTotalnohMFilhos(raiz->filhos[i]);
    }
    return cont;
}

int buscaMaiorValorMFilhos(tipo_no_m_filhos *raiz) {
    if(raiz == NULL) {
        return -1;
    }
    while(raiz->filhos[T] != NULL) {
        raiz = raiz->filhos[T];
    }
    int i;
    for(i = T-1; i >= 0; i--) {
        if(raiz->valores[i] != -1) {
            return raiz->valores[i];
        }
    }
    return -1;
}

int buscaMenorValorMFilhos(tipo_no_m_filhos *raiz) {
    if(raiz == NULL) {
        return -1;
    }
    while(raiz->filhos[0] != NULL) {
        raiz = raiz->filhos[0];
    }
    return raiz->valores[0];
}

void removeArvMFilhos(tipo_no_m_filhos **raiz, int valor) {
    if(*raiz == NULL) {
        return;
    }
    int i;
    for(i = 0; i < T; i++) {
        if((*raiz)->valores[i] == valor) {
            if((*raiz)->filhos[i] == NULL) {
                int j;
                for(j = i; j < T-1; j++) {
                    (*raiz)->valores[j] = (*raiz)->valores[j+1];
                    (*raiz)->filhos[j] = (*raiz)->filhos[j+1];
                }
                (*raiz)->valores[T-1] = -1;
                (*raiz)->filhos[T-1] = NULL;
                return;
            }
            else {
                int max_sub_arv = buscaMaiorValorMFilhos((*raiz)->filhos[i]);
                (*raiz)->valores[i] = max_sub_arv;
                removeArvMFilhos(&((*raiz)->filhos[i]), max_sub_arv);
                return;
            }
        }
        else if((*raiz)->valores[i] > valor || (*raiz)->valores[i] == -1) {
            if((*raiz)->filhos[i] != NULL) {
                removeArvMFilhos(&((*raiz)->filhos[i]), valor);
                return;
            }
            else {
                return;
            }
        }
    }
    if((*raiz)->filhos[i] != NULL) {
        removeArvMFilhos(&((*raiz)->filhos[i]), valor);
        return;
    }
    else {
        return;
    }
}

/*
int main() {
    tipo_no_m_filhos *raiz = NULL;
    int opcao, valor, nivel;
    do {
        printf("\n\nMENU:\n\n");
        printf("1 - Inserir valores na arvore\n");
        printf("2 - Percurso em ordem\n");
        printf("3 - Percurso em pre ordem\n");
        printf("4 - Percurso em pos ordem\n");
        printf("5 - Altura da arvore\n");
        printf("6 - Imprime os nos por nivel\n");
        printf("7 - Numero total de nos\n");
        printf("8 - Busca o menor valor\n");
        printf("9 - Busca o maior valor\n");
        printf("10 - Remover um valor da arvore\n");
        printf("11 - Sair\n\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &opcao);
        switch(opcao) {
            case 1:
                printf("Digite os valores que deseja inserir, separados por espaco: ");
                char valores[100];
                fgets(valores, 100, stdin); // para limpar o buffer do teclado
                fgets(valores, 100, stdin);
                char *token = strtok(valores, " ");
                while(token != NULL) {
                    valor = atoi(token);
                    insereArvMFilhos(&raiz, valor);
                    token = strtok(NULL, " ");
                }
                break;
            case 2:
                printf("\nPercurso em ordem:\n");
                percursoOrdemMFilhos(raiz);
                break;
            case 3:
                printf("\nPercurso em pre ordem:\n");
                percursoPreOrdemMFilhos(raiz);
                break;
            case 4:
                printf("\nPercurso em pos ordem:\n");
                percursoPosOrdemMFilhos(raiz);
                break;
            case 5:
                printf("\nAltura da arvore: %d\n", alturaArvMFilhos(raiz));
                break;
            case 6:
                printf("Digite o nivel: ");
                scanf("%d", &nivel);
                imprimeNosPorNivelMFilhos(raiz, nivel);
                break;
            case 7:
                printf("\nNumero total de nos: %d\n", contTotalnohMFilhos(raiz));
                break;
            case 8:
                printf("\nMenor valor: %d\n", buscaMenorValorMFilhos(raiz));
                break;
            case 9:
                printf("\nMaior valor: %d\n", buscaMaiorValorMFilhos(raiz));
                break;
            case 10:
                printf("Digite o valor que deseja remover: ");
                scanf("%d", &valor);
                removeArvMFilhos(&raiz, valor);
                break;
            case 11:
                printf("\nSaindo...\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
        }
    } while(opcao != 11);
    return 0;
}

*/
int main() {
    tipo_no_m_filhos *arvore = NULL;

    insereArvMFilhos(&arvore, 10);
    insereArvMFilhos(&arvore, 38);
    insereArvMFilhos(&arvore, 7);
    insereArvMFilhos(&arvore, 1);
    insereArvMFilhos(&arvore, 9);
    insereArvMFilhos(&arvore, 5);
    insereArvMFilhos(&arvore, 22);
    insereArvMFilhos(&arvore, 12);
    insereArvMFilhos(&arvore, 4);
    insereArvMFilhos(&arvore, 2);
    insereArvMFilhos(&arvore, 3);

    percursoOrdemMFilhos(arvore); printf("\n"); // Imprime os valores em ordem

    return 0;
}





   
