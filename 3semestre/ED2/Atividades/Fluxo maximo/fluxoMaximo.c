#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_VERTICES 1000

// Estrutura para representar uma aresta com capacidade e fluxo
typedef struct {
    int u, v, capacity, flow;
} Edge;

// Estrutura para representar o grafo
typedef struct {
    int vertices, edges;
    Edge edge[MAX_VERTICES * 2]; // Cada aresta de capacidade c é representada por duas arestas (capacidade c e capacidade residual 0)
} Graph;

// Função para criar um grafo com V vértices
Graph* createGraph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->vertices = vertices;
    graph->edges = 0;
    return graph;
}

// Função para adicionar uma aresta ao grafo
void addEdge(Graph* graph, int u, int v, int capacity) {
    Edge* e = &graph->edge[graph->edges++];
    e->u = u;
    e->v = v;
    e->capacity = capacity;
    e->flow = 0;
}

// Função para realizar uma busca em largura (BFS) para encontrar um caminho de aumento
int bfs(Graph* graph, int parent[], int source, int sink) {
    int visited[graph->vertices];
    memset(visited, 0, sizeof(visited));

    visited[source] = 1;
    parent[source] = -1;

    // Crie uma fila para a BFS
    int queue[graph->vertices];
    int front = 0, rear = 0;
    queue[rear++] = source;

    while (front != rear) {
        int u = queue[front++];
        
        for (int i = 0; i < graph->edges; i++) {
            int v = graph->edge[i].v;
            int capacity = graph->edge[i].capacity;
            int flow = graph->edge[i].flow;

            if (!visited[v] && capacity - flow > 0) {
                queue[rear++] = v;
                parent[v] = u;
                visited[v] = 1;
            }
        }
    }

    return (visited[sink] == 1);
}

// Função para encontrar o fluxo máximo usando o algoritmo de Ford-Fulkerson
int fordFulkerson(Graph* graph, int source, int sink) {
    int parent[graph->vertices];

    int maxFlow = 0;

    while (bfs(graph, parent, source, sink)) {
        int pathFlow = INT_MAX;
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            
            // Encontre a capacidade residual mínima ao longo do caminho
            for (int i = 0; i < graph->edges; i++) {
                if (graph->edge[i].u == u && graph->edge[i].v == v) {
                    int residual = graph->edge[i].capacity - graph->edge[i].flow;
                    if (residual < pathFlow) {
                        pathFlow = residual;
                    }
                }
            }
        }

        // Atualize os fluxos e as capacidades residuais ao longo do caminho
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            for (int i = 0; i < graph->edges; i++) {
                if (graph->edge[i].u == u && graph->edge[i].v == v) {
                    graph->edge[i].flow += pathFlow;
                }
                if (graph->edge[i].u == v && graph->edge[i].v == u) {
                    graph->edge[i].flow -= pathFlow;
                }
            }
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}

// Função para carregar uma instância DIMACS e construir o grafo
Graph* loadDIMACS(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    char line[256];
    int vertices, edges;
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "p max %d %d", &vertices, &edges) == 2)
            break;
    }

    Graph* graph = createGraph(vertices);

    while (fgets(line, sizeof(line), file)) {
        int u, v, capacity;
        if (sscanf(line, "a %d %d %d", &u, &v, &capacity) == 3) {
            addEdge(graph, u - 1, v - 1, capacity); // DIMACS usa índices baseados em 1
        }
    }

    fclose(file);

    return graph;
}

// Função para salvar o resultado do fluxo máximo
void saveResult(const char* filename, int maxFlow, Graph* graph) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Erro ao criar o arquivo de saída.\n");
        exit(1);
    }

    fprintf(file, "Fluxo Máximo: %d\n", maxFlow);
    fprintf(file, "Configuração do Grafo com Uso de Folgas:\n");

    for (int i = 0; i < graph->edges; i++) {
        fprintf(file, "Aresta %d-%d: Capacidade %d, Fluxo %d\n", graph->edge[i].u + 1, graph->edge[i].v + 1, graph->edge[i].capacity, graph->edge[i].flow);
    }

    fclose(file);
}

int main() {
    const char* instanceFile = "grafoFM-25-25-50.txt"; // Substitua pelo nome do arquivo de instância
    const char* outputFile = "output.txt"; // Nome do arquivo de saída

    Graph* graph = loadDIMACS(instanceFile);

    int source = 0; // Defina a origem
    int sink = graph->vertices - 1; // Defina o destino

    int maxFlow = fordFulkerson(graph, source, sink);

    printf("Fluxo Máximo: %d\n", maxFlow);

    saveResult(outputFile, maxFlow, graph);

    free(graph);

    return 0;
}
