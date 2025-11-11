//Bibliotecas
#include <stdlib.h>
#include <string.h>

//Definicao da capacidadse do noh
T 3;

//Definicao do  noh da arvore
struct est_no_m_filhos{
    int valores[T];
    struct est_no_m_filhos *filhos[t+1];
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

//

