//Bibliotecas
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Definicao de um novo tipo de dados (registro)
struct st_aluno{
    char nome[50];
    float n1;
    float n2;
    float media;
    float situacao;
};

//Apelido 
typedef struct st_aluno tipo_aluno;

//Prototipacao de funcoes 
tipo_aluno lerDados();
void imprimeAlunos(tipo_aluno);
float CalculaMedia(tipo_aluno);
void Situacao(tipo_aluno);

//funcao principal

int main(){

    tipo_aluno aluno1, aluno2;

    //ler dados 
    aluno1 = lerDados();
    aluno2 = lerDados();

    //media 

    aluno1.media = CalculaMedia(aluno1);
    aluno2.media = CalculaMedia(aluno2);

    //imprime

    imprimeAlunos(aluno1);
    Situacao(aluno1);
    imprimeAlunos(aluno2);
    Situacao(aluno2);

    return 1;

}

//Funcao ler dados  
tipo_aluno lerDados(){

    tipo_aluno aluno;

    printf("Informe o nome do aluno: ");
    scanf("%s", aluno.nome);
    printf("Informe a primeira nota: ");
    scanf("%f", &(aluno.n1));
    printf("Informe a segunda nota: ");
    scanf("%f", &(aluno.n2));

    return aluno;
}


//Funcao calcula media

float CalculaMedia(tipo_aluno aluno){
    aluno.media = ( aluno.n1 + aluno.n2 ) /2;
    return aluno.media;
}

//Funcao situacao
 void Situacao(tipo_aluno aluno){
    if (aluno.media < 7){
        printf("Reprovado(a)\n\n");
        printf("-------------------------------\n");
    }else {
        printf("Aprovado(a)\n");
        printf("-------------------------------\n");
    }
}

//Funcao imprime 

void imprimeAlunos(tipo_aluno aluno){
    printf("\n\nO nome do aluno eh: %s\n", aluno.nome);
    printf("A primeira nota eh: %.2f\n", aluno.n1);
    printf("A segunda nota eh: %.2f\n", aluno.n2);
    printf("A media do aluno eh: %.2f\n\n", aluno.media);
}
