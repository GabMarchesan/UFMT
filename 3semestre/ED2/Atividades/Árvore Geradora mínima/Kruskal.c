#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define INFINITY 1000000
#define MAX_LINE 1024

typedef int TIPOPESO;

typedef struct adjacencia {
    int vertice;
    int peso;
    struct adjacencia* prox;
} ADJACENCIA;

typedef struct vertice {
    ADJACENCIA* cab;
} VERTICE;

// Estrutura para representar uma aresta ponderada
typedef struct aresta{
    int origem;
    int destino;
    int peso;
} ARESTA;

typedef struct grafo {
    int vertices;
    int arestas;
    VERTICE* adj;
    ARESTA* listaAresta;
} GRAFO;

typedef struct conjunto{
    int rank;
    int pai;
} CONJUNTO;

// Função para comparar duas arestas com base no peso
int comparaArestas(const void* a, const void* b) {
    return ((ARESTA*)a)->peso - ((ARESTA*)b)->peso;
}

// Declaração antecipada das funções para evitar warnings
int buscar(int* subconjuntos, int i);
void unir(int* subconjuntos, int x, int y);

GRAFO* criaGrafo(int v) {
    GRAFO* g = (GRAFO*)malloc(sizeof(GRAFO));

    g->vertices = v;
    g->arestas = 0;
    g->adj = (VERTICE*)malloc(v * sizeof(VERTICE));

    for (int i = 0; i < v; i++)
        g->adj[i].cab = NULL;

    return g;
}

ADJACENCIA* criaAdjacencia(int w, int peso) {
    ADJACENCIA* tmp = (ADJACENCIA*)malloc(sizeof(ADJACENCIA));

    tmp->vertice = w;
    tmp->peso = peso;
    tmp->prox = NULL;

    return tmp;
}

int criaAresta(GRAFO* g, int v, int w, int p) {
    if (!g)
        return false;
    if ((v < 0) || (v >= g->vertices))
        return false;
    if ((w < 0) || (w >= g->vertices))
        return false;

    ADJACENCIA* novo = criaAdjacencia(w, p);

    novo->prox = g->adj[v].cab;
    g->adj[v].cab = novo;

    return true;
}

void imprime(GRAFO* g) {
    printf("Vertices: %d, Arestas: %d, \n", g->vertices, g->arestas);

    for (int i = 0; i < g->vertices; i++) {
        printf("v%d: ", i);
        ADJACENCIA* ad = g->adj[i].cab;
        while (ad) {
            printf("v%d(%d) ", ad->vertice, ad->peso);
            ad = ad->prox;
        }
        printf("\n");
    }
}

GRAFO *carregarInstancia(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        exit(1);
    }

    int vertice1, vertice2;
    TIPOPESO peso;
    char linha[MAX_LINE];
    int nVertice = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (linha[0] == 'p') {
            sscanf(linha, "p sp %d", &nVertice);
            break;
        }
    }

    GRAFO *grafo = criaGrafo(nVertice); 

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (sscanf(linha, "a %d %d %d", &vertice1, &vertice2, &peso) == 3) {
            criaAresta(grafo, vertice1, vertice2, peso);
        }
    }

    fclose(arquivo);

    return grafo;
}

void salvarResultado(char* nomeArquivo, int custo, double tempoExecucao, GRAFO* arvoreMinima) {
    
    FILE* arquivo = fopen(nomeArquivo, "w");
    
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo de saída.\n");
        return;
    }

    fprintf(arquivo, "Árvore Mínima:\n");
    fprintf(arquivo, "Custo da Árvore: %d\n", custo);
    fprintf(arquivo, "Tempo de Execução: %.6f segundos\n", tempoExecucao);

    printf("\n\n\nÁrvore Mínima:\n");
    printf("Custo da Árvore: %d\n", custo);
    printf("Tempo de Execução: %.6f segundos\n", tempoExecucao);
    
    for (int i = 0; i < arvoreMinima->vertices; i++) {
        ADJACENCIA* adj = arvoreMinima->adj[i].cab;
        while (adj) {
            printf("%d -- %d (%d)\n", i, adj->vertice, adj->peso);
            adj = adj->prox;
        }
    }

    fclose(arquivo);
}

// Algoritmo de Kruskal para encontrar a árvore geradora mínima
GRAFO* Kruskal(GRAFO* grafo, int* numArestasArvore) {

    int numVertices = grafo->vertices;
    int numArestas = grafo->arestas;
    ARESTA* arestas = grafo->listaAresta;
    CONJUNTO* conjuntos = (CONJUNTO*)malloc(numVertices * sizeof(CONJUNTO));
    GRAFO* arvore = (GRAFO*)malloc((numVertices - 1) * sizeof(GRAFO));

    // Ordena as arestas por peso
    qsort(arestas, numArestas, sizeof(ARESTA), comparaArestas);

    // Inicializa conjuntos disjuntos
    for (int i = 0; i < numVertices; i++) {
        conjuntos[i].pai = i;
        conjuntos[i].rank = 0;
    }

    int arestasNaArvore = 0;
    int i = 0;

    while (arestasNaArvore < numVertices - 1 && i < numArestas) {
        int origem = arestas[i].origem;
        int destino = arestas[i].destino;

        int raizOrigem = origem;
        int raizDestino = destino;

        while (raizOrigem != conjuntos[raizOrigem].pai) {
            raizOrigem = conjuntos[raizOrigem].pai;
        }

        while (raizDestino != conjuntos[raizDestino].pai) {
            raizDestino = conjuntos[raizDestino].pai;
        }

        if (raizOrigem != raizDestino) {
            criaAresta(grafo, origem, destino, arestas[i].peso);
            arestasNaArvore++;

            // Realiza a união dos conjuntos
            if (conjuntos[raizOrigem].rank < conjuntos[raizDestino].rank) {
                conjuntos[raizOrigem].pai = raizDestino;
            } else if (conjuntos[raizOrigem].rank > conjuntos[raizDestino].rank) {
                conjuntos[raizDestino].pai = raizOrigem;
            } else {
                conjuntos[raizDestino].pai = raizOrigem;
                conjuntos[raizOrigem].rank++;
            }
        }

        i++;
    }

    free(conjuntos);
    *numArestasArvore = arestasNaArvore;
    
    return arvore;
}


int main() {

    // Carregar a instância NY
    const char* arquivoNY = "exemploAula.txt";
    GRAFO* grafoNY = carregarInstancia(arquivoNY);

    printf("\nGRAFO DADO EM AULA:\n\n");
    imprime(grafoNY);

    // Medir o tempo de execução para o algoritmo de Kruskal na instância NY
    clock_t inicioKruskalNY = clock();
    GRAFO* arvoreMinimaKruskalNY;
    arvoreMinimaKruskalNY = (grafoNY);
    clock_t fimKruskalNY = clock();
    double tempoExecucaoKruskalNY = (double)(fimKruskalNY - inicioKruskalNY) / CLOCKS_PER_SEC;



    // Calcular o custo total da árvore mínima de NY em Kruskal
    int custoArvoreKruskalNY = 0;
    for (int i = 0; i < grafoNY->vertices; i++) {
        ADJACENCIA* adj = arvoreMinimaKruskalNY->adj[i].cab;
        while (adj) {
            custoArvoreKruskalNY += adj->peso;
            adj = adj->prox;
        }
    }

    // Salvar os resultados em um arquivo para a instância NY
    salvarResultado("saida_NY_Kruskal.txt", custoArvoreKruskalNY, tempoExecucaoKruskalNY, arvoreMinimaKruskalNY);

    // Liberar a memória da instância NY e árvores mínimas
    free(grafoNY->adj);
    free(grafoNY);
    free(arvoreMinimaKruskalNY->adj);
    free(arvoreMinimaKruskalNY);

    return 0;
}