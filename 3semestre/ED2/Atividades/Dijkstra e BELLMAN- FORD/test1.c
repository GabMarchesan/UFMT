#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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
    /* dados serão armazenados aqui */
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
void dijkstra(GRAFO* grafo, int origem, int destino) {
    int* distancia = (int*)malloc(grafo->vertices * sizeof(int));
    int* predecessor = (int*)malloc(grafo->vertices * sizeof(int));
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

    // Calcule a rota a partir do vetor de predecessores
    int v = destino;
    while (v != -1) {
        printf("%d <- ", v);
        v = predecessor[v];
    }
    printf("\nDistância de %d a %d: %d\n", origem, destino, distancia[destino]);

    free(distancia);
    free(predecessor);
    free(visitado);
}

// Função que executa o algoritmo de Bellman-Ford
void bellmanFord(GRAFO* grafo, int origem, int destino) {
    int* distancia = (int*)malloc(grafo->vertices * sizeof(int));
    int* predecessor = (int*)malloc(grafo->vertices * sizeof(int));

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

    // Verifique se há ciclos de peso negativo
    for (int u = 0; u < grafo->vertices; u++) {
        ADJACENCIA* adj = grafo->adj[u].cab;
        while (adj) {
            int v = adj->vertice;
            int peso = adj->peso;

            if (distancia[u] != INFINITY && distancia[u] + peso < distancia[v]) {
                printf("O grafo contém um ciclo de peso negativo\n");
                return;
            }

            adj = adj->prox;
        }
    }

    // Calcule a rota a partir do vetor de predecessores
    int v = destino;
    while (v != -1) {
        printf("%d <- ", v);
        v = predecessor[v];
    }
    printf("\nDistância de %d a %d: %d\n", origem, destino, distancia[destino]);

    free(distancia);
    free(predecessor);
}

int main() {
    // Inicializa o grafo
    GRAFO* gr = criaGrafo(5);

    // Criando função para adicionar
    // arestas ao grafo
    criaAresta(gr, 0, 1, 2);
    criaAresta(gr, 1, 2, 4);
    criaAresta(gr, 2, 0, 12);
    criaAresta(gr, 2, 4, 40);
    criaAresta(gr, 3, 1, 3);
    criaAresta(gr, 4, 3, 8);

    imprime(gr);

    // Escolha três vértices para cálculo do menor caminho
    int vertice1 = 0; // Substitua pelo vértice desejado
    int vertice2 = 1; // Substitua pelo vértice desejado
    int vertice3 = 2; // Substitua pelo vértice desejado

    // Execute os algoritmos de Dijkstra e Bellman-Ford para os três vértices escolhidos
    dijkstra(gr, 0, vertice1);
    dijkstra(gr, 0, vertice2);
    dijkstra(gr, 0, vertice3);

    bellmanFord(gr, 0, vertice1);
    bellmanFord(gr, 0, vertice2);
    bellmanFord(gr, 0, vertice3);

    return 0;
}
