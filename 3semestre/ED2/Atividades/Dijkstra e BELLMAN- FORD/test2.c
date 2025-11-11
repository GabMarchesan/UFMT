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
void dijkstra(GRAFO* grafo, int origem, int destino, int* distancia, int* predecessor) {
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
void bellmanFord(GRAFO* grafo, int origem, int destino, int* distancia, int* predecessor) {
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

int main() {
    // Carregar instâncias do mapa rodoviário
    GRAFO* gr = criaGrafo(5);

    // Criar função para adicionar
    // arestas ao grafo
    criaAresta(gr, 0, 1, 2);
    criaAresta(gr, 1, 2, 4);
    criaAresta(gr, 2, 0, 12);
    criaAresta(gr, 2, 4, 40);
    criaAresta(gr, 3, 1, 3);
    criaAresta(gr, 4, 3, 8);

    // Escolher três vértices para cálculo do menor caminho
    int vertice1 = 0;
    int vertice2 = 2;
    int vertice3 = 3;

    // Arrays para armazenar distâncias e predecessores
    int distancia[gr->vertices];
    int predecessor[gr->vertices];

    // Executar os algoritmos de Dijkstra e Bellman-Ford para os três vértices escolhidos
    FILE* arquivo = fopen("resultados.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo de saída.\n");
        return 1;
    }

    fprintf(arquivo, "Resultados:\n");

    clock_t inicioDijkstra = clock();
    dijkstra(gr, 0, vertice1, distancia, predecessor);
    clock_t fimDijkstra = clock();
    double tempoDijkstra = (double)(fimDijkstra - inicioDijkstra) / CLOCKS_PER_SEC;

    fprintf(arquivo, "Dijkstra de 0 para %d:\n", vertice1);
    fprintf(arquivo, "Distância: %d\n", distancia[vertice1]);
    fprintf(arquivo, "Rota: ");
    int v = vertice1;
    while (v != 0) {
        fprintf(arquivo, "%d <- ", v);
        v = predecessor[v];
    }
    fprintf(arquivo, "0\n");
    fprintf(arquivo, "Tempo de execução: %.6f segundos\n\n", tempoDijkstra);

    clock_t inicioBellmanFord = clock();
    bellmanFord(gr, 0, vertice2, distancia, predecessor);
    clock_t fimBellmanFord = clock();
    double tempoBellmanFord = (double)(fimBellmanFord - inicioBellmanFord) / CLOCKS_PER_SEC;

    fprintf(arquivo, "Bellman-Ford de 0 para %d:\n", vertice2);
    fprintf(arquivo, "Distância: %d\n", distancia[vertice2]);
    fprintf(arquivo, "Rota: ");
    v = vertice2;
    while (v != 0) {
        fprintf(arquivo, "%d <- ", v);
        v = predecessor[v];
    }
    fprintf(arquivo, "0\n");
    fprintf(arquivo, "Tempo de execução: %.6f segundos\n\n", tempoBellmanFord);

    clock_t inicioDijkstra2 = clock();
    dijkstra(gr, 0, vertice3, distancia, predecessor);
    clock_t fimDijkstra2 = clock();
    double tempoDijkstra2 = (double)(fimDijkstra2 - inicioDijkstra2) / CLOCKS_PER_SEC;

    fprintf(arquivo, "Dijkstra de 0 para %d:\n", vertice3);
    fprintf(arquivo, "Distância: %d\n", distancia[vertice3]);
    fprintf(arquivo, "Rota: ");
    v = vertice3;
    while (v != 0) {
        fprintf(arquivo, "%d <- ", v);
        v = predecessor[v];
    }
    fprintf(arquivo, "0\n");
    fprintf(arquivo, "Tempo de execução: %.6f segundos\n\n", tempoDijkstra2);

    fclose(arquivo);

    printf("Resultados salvos em 'resultados.txt'.\n");

    return 0;
}
