#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define INFINITY INT_MAX
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

typedef struct grafo {
    int vertices;
    int arestas;
    VERTICE* adj;
} GRAFO;

// Estrutura para representar uma aresta ponderada
typedef struct {
    int origem;
    int destino;
    int peso;
} ARESTA;

// Função para comparar duas arestas com base no peso
int comparaArestas(const void* a, const void* b) {
    return ((ARESTA*)a)->peso - ((ARESTA*)b)->peso;
}

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

void salvarResultado(char* nomeArquivo, int origem, int destino, int* distancia, int* predecessor, double tempoExecucao) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo de saída.\n");
        return;
    }

    fprintf(arquivo, "Origem: %d, Destino: %d\n", origem, destino);
    fprintf(arquivo, "Distância: %d\n", distancia[destino]);
    fprintf(arquivo, "Rota: ");
    int v = destino;
    while (v != origem) {
        fprintf(arquivo, "%d <- ", v);
        v = predecessor[v];
    }
    fprintf(arquivo, "%d\n", origem);
    fprintf(arquivo, "Tempo de Execução: %.6f segundos\n", tempoExecucao);

    fclose(arquivo);
}

// Função auxiliar para o algoritmo de Kruskal
int buscar(int* subconjuntos, int i) {
    if (subconjuntos[i] == -1)
        return i;
    return buscar(subconjuntos, subconjuntos[i]);
}

// Função auxiliar para o algoritmo de Kruskal
void unir(int* subconjuntos, int x, int y) {
    int raizX = buscar(subconjuntos, x);
    int raizY = buscar(subconjuntos, y);
    subconjuntos[raizX] = raizY;
}

// Algoritmo de Kruskal para encontrar a árvore geradora mínima
GRAFO* kruskal(GRAFO* grafo) {
    int numVertices = grafo->vertices;
    ARESTA* arestas = (ARESTA*)malloc(grafo->arestas * sizeof(ARESTA));

    // Inicializar vetor de subconjuntos
    int* subconjuntos = (int*)malloc(numVertices * sizeof(int));
    memset(subconjuntos, -1, numVertices * sizeof(int));

    // Copiar as arestas do grafo para o vetor
    int index = 0;
    for (int v = 0; v < numVertices; v++) {
        ADJACENCIA* adj = grafo->adj[v].cab;
        while (adj) {
            if (v < adj->vertice) {
                arestas[index].origem = v;
                arestas[index].destino = adj->vertice;
                arestas[index].peso = adj->peso;
                index++;
            }
            adj = adj->prox;
        }
    }

    // Ordenar as arestas por peso
    qsort(arestas, grafo->arestas, sizeof(ARESTA), comparaArestas);

    // Criar o grafo resultante
    GRAFO* arvoreMinima = criaGrafo(numVertices);

    int arestasInseridas = 0;
    index = 0;
    while (arestasInseridas < numVertices - 1) {
        ARESTA proximaAresta = arestas[index++];
        int raizOrigem = buscar(subconjuntos, proximaAresta.origem);
        int raizDestino = buscar(subconjuntos, proximaAresta.destino);

        if (raizOrigem != raizDestino) {
            criaAresta(arvoreMinima, proximaAresta.origem, proximaAresta.destino, proximaAresta.peso);
            unir(subconjuntos, raizOrigem, raizDestino);
            arestasInseridas++;
        }
    }

    free(arestas);
    free(subconjuntos);

    return arvoreMinima;
}

// Algoritmo de Prim para encontrar a árvore geradora mínima
GRAFO* prim(GRAFO* grafo) {
    int numVertices = grafo->vertices;
    int* pai = (int*)malloc(numVertices * sizeof(int));
    int* chave = (int*)malloc(numVertices * sizeof(int));
    bool* naArvore = (bool*)malloc(numVertices * sizeof(bool));

    for (int i = 0; i < numVertices; i++) {
        chave[i] = INFINITY;
        naArvore[i] = false;
    }

    chave[0] = 0;
    pai[0] = -1;

    for (int count = 0; count < numVertices - 1; count++) {
        int u = -1;
        for (int v = 0; v < numVertices; v++) {
            if (!naArvore[v] && (u == -1 || chave[v] < chave[u]))
                u = v;
        }

        naArvore[u] = true;

        ADJACENCIA* adj = grafo->adj[u].cab;
        while (adj) {
            int v = adj->vertice;
            int peso = adj->peso;
            if (!naArvore[v] && peso < chave[v]) {
                pai[v] = u;
                chave[v] = peso;
            }
            adj = adj->prox;
        }
    }

    GRAFO* arvoreMinima = criaGrafo(numVertices);

    for (int i = 1; i < numVertices; i++) {
        criaAresta(arvoreMinima, pai[i], i, chave[i]);
    }

    free(pai);
    free(chave);
    free(naArvore);

    return arvoreMinima;
}

int main() {
    // Carregar as instâncias
    const char* arquivoCAL = "CAL.txt";
    const char* arquivoNY = "NY.txt";

    GRAFO* grafoNY = carregarInstancia(arquivoNY);
    GRAFO* grafoCAL = carregarInstancia(arquivoCAL);

    // Definir origens e destinos
    int origem1 = 0; // Escolha o vértice de origem desejado
    int destino1 = 1; // Escolha o vértice de destino desejado

    int origem2 = 2; // Escolha o vértice de origem desejado
    int destino2 = 3; // Escolha o vértice de destino desejado

    // Medir o tempo de execução para os algoritmos de Kruskal
    clock_t inicioKruskalNY = clock();
    GRAFO* arvoreMinimaKruskalNY = kruskal(grafoNY);
    clock_t fimKruskalNY = clock();
    double tempoExecucaoKruskalNY = (double)(fimKruskalNY - inicioKruskalNY) / CLOCKS_PER_SEC;

    clock_t inicioKruskalCAL = clock();
    GRAFO* arvoreMinimaKruskalCAL = kruskal(grafoCAL);
    clock_t fimKruskalCAL = clock();
    double tempoExecucaoKruskalCAL = (double)(fimKruskalCAL - inicioKruskalCAL) / CLOCKS_PER_SEC;

    // Medir o tempo de execução para os algoritmos de Prim
    clock_t inicioPrimNY = clock();
    GRAFO* arvoreMinimaPrimNY = prim(grafoNY);
    clock_t fimPrimNY = clock();
    double tempoExecucaoPrimNY = (double)(fimPrimNY - inicioPrimNY) / CLOCKS_PER_SEC;

    clock_t inicioPrimCAL = clock();
    GRAFO* arvoreMinimaPrimCAL = prim(grafoCAL);
    clock_t fimPrimCAL = clock();
    double tempoExecucaoPrimCAL = (double)(fimPrimCAL - inicioPrimCAL) / CLOCKS_PER_SEC;

    // Salvar os resultados em arquivos
    salvarResultado("saida_NY_Kruskal.txt", origem1, destino1, NULL, NULL, tempoExecucaoKruskalNY);
    salvarResultado("saida_CAL_Kruskal.txt", origem2, destino2, NULL, NULL, tempoExecucaoKruskalCAL);
    salvarResultado("saida_NY_Prim.txt", origem1, destino1, NULL, NULL, tempoExecucaoPrimNY);
    salvarResultado("saida_CAL_Prim.txt", origem2, destino2, NULL, NULL, tempoExecucaoPrimCAL);

    // Liberar a memória
    free(grafoNY->adj);
    free(grafoNY);
    free(grafoCAL->adj);
    free(grafoCAL);
    free(arvoreMinimaKruskalNY->adj);
    free(arvoreMinimaKruskalNY);
    free(arvoreMinimaKruskalCAL->adj);
    free(arvoreMinimaKruskalCAL);
    free(arvoreMinimaPrimNY->adj);
    free(arvoreMinimaPrimNY);
    free(arvoreMinimaPrimCAL->adj);
    free(arvoreMinimaPrimCAL);

    return 0;
}
