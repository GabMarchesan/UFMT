#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define true 1
#define false 0
#define INFINITY INT_MAX

typedef int bool;
typedef int TIPOPESO;

typedef struct adjacencia {
    int vertice;
    TIPOPESO peso;
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

// Função que cria o grafo
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

    return (tmp);
}

int criaAresta(GRAFO* g, int v, int w, TIPOPESO p) {
    if (!g)
        return (false);
    if ((v < 0) || (v >= g->vertices))
        return (false);
    if ((w < 0) || (w >= g->vertices))
        return (false);

    ADJACENCIA* novo = criaAdjacencia(w, p);

    novo->prox = g->adj[v].cab;
    g->adj[v].cab = novo;

    ADJACENCIA* novo2 = criaAdjacencia(v, p);

    novo2->prox = g->adj[w].cab;
    g->adj[w].cab = novo2;
    g->arestas++;

    return (true);
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

// Função que executa o algoritmo de Dijkstra
void dijkstra(GRAFO* grafo, int origem, int* distancia, int* predecessor) {
    bool* visitado = (bool*)malloc(grafo->vertices * sizeof(bool));

    for (int i = 0; i < grafo->vertices; i++) {
        distancia[i] = INFINITY;
        predecessor[i] = -1;
        visitado[i] = false;
    }

    distancia[origem] = 0;

    for (int count = 0; count < grafo->vertices - 1; count++) {
        int u = -1;
        for (int v = 0; v < grafo->vertices; v++) {
            if (!visitado[v] && (u == -1 || distancia[v] < distancia[u])) {
                u = v;
            }
        }

        visitado[u] = true;

        ADJACENCIA* adj = grafo->adj[u].cab;
        while (adj) {
            int v = adj->vertice;
            int peso = adj->peso;

            if (!visitado[v] && distancia[u] + peso < distancia[v]) {
                distancia[v] = distancia[u] + peso;
                predecessor[v] = u;
            }

            adj = adj->prox;
        }
    }
}

// Função que executa o algoritmo de Bellman-Ford
void bellmanFord(GRAFO* grafo, int origem, int* distancia, int* predecessor) {
    for (int i = 0; i < grafo->vertices; i++) {
        distancia[i] = INFINITY;
        predecessor[i] = -1;
    }

    distancia[origem] = 0;

    for (int i = 1; i <= grafo->vertices - 1; i++) {
        for (int u = 0; u < grafo->vertices; u++) {
            ADJACENCIA* adj = grafo->adj[u].cab;
            while (adj) {
                int v = adj->vertice;
                int peso = adj->peso;

                if (distancia[u] != INFINITY && distancia[u] + peso < distancia[v]) {
                    distancia[v] = distancia[u] + peso;
                    predecessor[v] = u;
                }

                adj = adj->prox;
            }
        }
    }
}

void carregarInstancia(GRAFO* grafo, char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        exit(1);
    }

    int vertice1, vertice2, peso;
    while (fscanf(arquivo, "%d %d %d", &vertice1, &vertice2, &peso) == 3) {
        criaAresta(grafo, vertice1, vertice2, peso);
    }

    fclose(arquivo);
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

int main() {
    //char arquivoNY[256];
    //char arquivoCAL[256];

    //printf("Digite o nome do arquivo de instância NY (New York City): ");
    //scanf("%s", arquivoNY);

    //printf("Digite o nome do arquivo de instância CAL (California and Nevada): ");
    //scanf("%s", arquivoCAL);
    const char *arquivoCAL = "CAL.txt";
    const char *arquivoNY = "NY.txt";

    GRAFO *grafoCAL = lerArquivoDeGrafo(arquivoCAL);
    GRAFO *grafoNY = lerArquivoDeGrafo(arquivoNY);

    imprime(grafoNY);

    //GRAFO* grafoNY = criaGrafo(0); // Substitua 0 pelo número correto de vértices
    //GRAFO* grafoCAL = criaGrafo(0); // Substitua 0 pelo número correto de vértices

    //carregarInstancia(grafoNY, arquivoNY);
    //carregarInstancia(grafoCAL, arquivoCAL);

    int origem1 = 0; // Escolha o vértice de origem desejado
    int destino1 = 1; // Escolha o vértice de destino desejado

    int origem2 = 2; // Escolha o vértice de origem desejado
    int destino2 = 3; // Escolha o vértice de destino desejado

    int* distancia1 = (int*)malloc(grafoNY->vertices * sizeof(int));
    int* predecessor1 = (int*)malloc(grafoNY->vertices * sizeof(int));

    int* distancia2 = (int*)malloc(grafoCAL->vertices * sizeof(int));
    int* predecessor2 = (int*)malloc(grafoCAL->vertices * sizeof(int));

    // Medir o tempo de execução para o algoritmo 1
    clock_t inicio1 = clock();
    dijkstra(grafoNY, origem1, destino1, distancia1);
    clock_t fim1 = clock();
    double tempoExecucao1 = (double)(fim1 - inicio1) / CLOCKS_PER_SEC;

    // Medir o tempo de execução para o algoritmo 2
    clock_t inicio2 = clock();
    bellmanFord(grafoCAL, origem2, distancia2);
    clock_t fim2 = clock();
    double tempoExecucao2 = (double)(fim2 - inicio2) / CLOCKS_PER_SEC;

    // Salvar os resultados em arquivos
    salvarResultado("saida_NY.txt", origem1, destino1, distancia1, predecessor1, tempoExecucao1);
    salvarResultado("saida_CAL.txt", origem2, destino2, distancia2, predecessor2, tempoExecucao2);

    // Liberar a memória
    free(distancia1);
    free(predecessor1);
    free(distancia2);
    free(predecessor2);
    free(grafoNY->adj);
    free(grafoNY);
    free(grafoCAL->adj);
    free(grafoCAL);

    return 0;
}
