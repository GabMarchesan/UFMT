#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define INFINITY INT_MAX
#define MAX_LINE 1024

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

    ADJACENCIA* novo1 = criaAdjacencia(w, p);
    ADJACENCIA* novo2 = criaAdjacencia(v, 0);

    novo1->prox = g->adj[v].cab;
    g->adj[v].cab = novo1;

    novo2->prox = g->adj[w].cab;
    g->adj[w].cab = novo2;

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

GRAFO* carregarInstancia(const char* nomeArquivo, int nVertice) {
    FILE* arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        exit(1);
    }

    int vertice1, vertice2;
    char linha[MAX_LINE];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (linha[0] != 'c') {
            break;
        }
    }

    GRAFO* grafo = criaGrafo(nVertice);

    while (!feof(arquivo)) {
        fscanf(arquivo, "%*c %d %d\n", &vertice1, &vertice2);
        criaAresta(grafo, (vertice1 - 1), (vertice2 - 1), 0);
    }

    fclose(arquivo);

    return grafo;
}

int calculaNumeroCromatico(int* colors, int numVertices) {
    int maxColor = -1;
    for (int i = 0; i < numVertices; i++) {
        if (colors[i] > maxColor) {
            maxColor = colors[i];
        }
    }
    return maxColor;
}

void algoritmoDsatur(GRAFO* g, int* chromaticNumber, int* colors) {
    int* saturation = (int*)malloc(g->vertices * sizeof(int));
    int* degree = (int*)malloc(g->vertices * sizeof(int));
    int max_degree = -1;

    for (int i = 0; i < g->vertices; i++) {
        saturation[i] = 0;
        colors[i] = -1;
        degree[i] = 0;
        ADJACENCIA* adj = g->adj[i].cab;
        while (adj) {
            degree[i]++;
            adj = adj->prox;
        }
        if (degree[i] > max_degree) {
            max_degree = degree[i];
        }
    }

    int current_vertex = -1;
    for (int i = 0; i < g->vertices; i++) {
        if (degree[i] == max_degree) {
            current_vertex = i;
            break;
        }
    }
    colors[current_vertex] = 1;

    for (int step = 2; step <= g->vertices; step++) {
        int max_satur = -1;
        current_vertex = -1;

        for (int i = 0; i < g->vertices; i++) {
            if (colors[i] == -1) {
                int satur = saturation[i];
                if (satur > max_satur || (satur == max_satur && degree[i] > degree[current_vertex])) {
                    max_satur = satur;
                    current_vertex = i;
                }
            }
        }

        int available_color = 1;
        bool* used_colors = (bool*)calloc(step, sizeof(bool));
        ADJACENCIA* adj = g->adj[current_vertex].cab;
        while (adj) {
            if (colors[adj->vertice] != -1) {
                used_colors[colors[adj->vertice]] = true;
            }
            adj = adj->prox;
        }
        while (used_colors[available_color]) {
            available_color++;
        }
        colors[current_vertex] = available_color;
        free(used_colors);

        adj = g->adj[current_vertex].cab;
        while (adj) {
            saturation[adj->vertice]++;
            adj = adj->prox;
        }
    }

    *chromaticNumber = calculaNumeroCromatico(colors, g->vertices);

    free(saturation);
    free(degree);
}

int main() {
    // Carregar a instância JEAN
    printf("Jean:\n");
    const char* arquivoJean = "jean.col";
    GRAFO* grafoJean = carregarInstancia(arquivoJean, 80);
    int* colorsJean = (int*)malloc(grafoJean->vertices * sizeof(int));
    int chromaticNumberJean;
    algoritmoDsatur(grafoJean, &chromaticNumberJean, colorsJean);
    printf("Número cromático de Jean: %d\n\n", chromaticNumberJean);

    // Carregar a instância MILES(250)
    printf("Miles(250):\n");
    const char* arquivoMiles250 = "miles250.col";
    GRAFO* grafoMiles250 = carregarInstancia(arquivoMiles250, 128);
    int* colorsMiles250 = (int*)malloc(grafoMiles250->vertices * sizeof(int));
    int chromaticNumberMiles250;
    algoritmoDsatur(grafoMiles250, &chromaticNumberMiles250, colorsMiles250);
    printf("Número cromático de Miles(250): %d\n\n", chromaticNumberMiles250);

    // Carregar a instância MILES(1000)
    printf("Miles(1000):\n");
    const char* arquivoMiles1000 = "miles1000.col";
    GRAFO* grafoMiles1000 = carregarInstancia(arquivoMiles1000, 128);
    int* colorsMiles1000 = (int*)malloc(grafoMiles1000->vertices * sizeof(int));
    int chromaticNumberMiles1000;
    algoritmoDsatur(grafoMiles1000, &chromaticNumberMiles1000, colorsMiles1000);
    printf("Número cromático de Miles(1000): %d\n\n", chromaticNumberMiles1000);

    // Carregar a instância LE(450_25A)
    printf("Le(450_25a):\n");
    const char* arquivoLe450 = "le450_25a.col";
    GRAFO* grafoLe450 = carregarInstancia(arquivoLe450, 450);
    int* colorsLe450 = (int*)malloc(grafoLe450->vertices * sizeof(int));
    int chromaticNumberLe450;
    algoritmoDsatur(grafoLe450, &chromaticNumberLe450, colorsLe450);
    printf("Número cromático de Le(450_25a): %d\n\n", chromaticNumberLe450);

    // Carregar a instância R(1000)
    printf("R(1000):\n");
    const char* arquivoR1000 = "r1000.1.col";
    GRAFO* grafoR1000 = carregarInstancia(arquivoR1000, 1000);
    int* colorsR1000 = (int*)malloc(grafoR1000->vertices * sizeof(int));
    int chromaticNumberR1000;
    algoritmoDsatur(grafoR1000, &chromaticNumberR1000, colorsR1000);
    printf("Número cromático de R(1000): %d\n\n", chromaticNumberR1000);

    // Carregar a instância R(250)
    printf("R(250):\n");
    const char* arquivoR250 = "r250.1c.col";
    GRAFO* grafoR250 = carregarInstancia(arquivoR250, 250);
    int* colorsR250 = (int*)malloc(grafoR250->vertices * sizeof(int));
    int chromaticNumberR250;
    algoritmoDsatur(grafoR250, &chromaticNumberR250, colorsR250);
    printf("Número cromático de R(250): %d\n\n", chromaticNumberR250);

    // Libere a memória alocada para os vetores de cores
    free(colorsJean);
    free(colorsMiles250);
    free(colorsMiles1000);
    free(colorsLe450);
    free(colorsR1000);
    free(colorsR250);

    return 0;
}

