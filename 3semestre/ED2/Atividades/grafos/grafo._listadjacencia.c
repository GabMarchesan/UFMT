#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0

typedef int bool;
typedef int TIPOPESO;

typedef struct adjacencia{
    int vertice;
    TIPOPESO peso;
    struct adjacencia *prox;
}ADJACENCIA;

typedef struct vertice{
    /* dados serão armazenado aqui*/
    ADJACENCIA *cab;
}VERTICE;

typedef struct grafo{

    int vertices;
    int arestas;
    VERTICE *adj;
}GRAFO;

// Função que cria o grafo
GRAFO *criaGrafo(int v){
    GRAFO *g = (GRAFO *) malloc (sizeof(GRAFO));

    g->vertices = v;
    g->arestas  = 0;
    g->adj      = (VERTICE *) malloc (v*sizeof(VERTICE));

    for(int i =0; i < v; i++)
        g->adj[i].cab = NULL;

    return g;
}

ADJACENCIA *criaAdjacencia(int w, int peso){

    ADJACENCIA *tmp = (ADJACENCIA *) malloc (sizeof(ADJACENCIA));

    tmp->vertice = w;
    tmp->peso   = peso;
    tmp->prox   = NULL;

    return (tmp);
}

int criaAresta(GRAFO *g, int v, int w,TIPOPESO p){

    if(!g)
        return (false);
    if( (v<0) || (v >= g->vertices))
        return (false);
    if( (w<0) || (w >= g->vertices))
        return (false);

    ADJACENCIA *novo = criaAdjacencia(w,p);

    novo->prox = g->adj[v].cab;
    g->adj[v].cab = novo;

     ADJACENCIA *novo2 = criaAdjacencia(v,p);

    novo2->prox = g->adj[w].cab;
    g->adj[w].cab = novo2;
    g->arestas++;

    return (true);


}

void imprime(GRAFO *g){

    printf("Vertices: %d, Arestas: %d, \n", g->vertices, g->arestas);

    for(int i=0; i< g->vertices; i++)
    {
        printf("v%d: ",i);
        ADJACENCIA *ad = g->adj[i].cab;
        while(ad){

            printf("v%d(%d) ",ad->vertice,ad->peso);
            ad = ad->prox;
        }
        printf("\n");
    }
}

int main(){

    // Inicializa o grafo
    GRAFO *gr = criaGrafo(5);

    // Criando função para adicionar
    // arestas ao grafo
    criaAresta(gr,0,1,2);
    criaAresta(gr,1,2,4);
    criaAresta(gr,2,0,12);
    criaAresta(gr,2,4,40);
    criaAresta(gr,3,1,3);
    criaAresta(gr,4,3,8);

    imprime(gr);

    return 0;


}
