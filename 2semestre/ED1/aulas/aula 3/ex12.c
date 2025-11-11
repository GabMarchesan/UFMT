//Bibliotecas 

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//Definicao de um novo tipo de dados (registro)
struct st_produto {

    float custo;
    float lucro;
    float preco;
};
//Apelido
typedef struct st_produto tipo_produto;

//Prototipo de funcoes
tipo_produto calculaPreco(tipo_produto);
void imprimeProduto(tipo_produto);
void dobraProduto(tipo_produto *);

//Funcao principal

int main(){

    tipo_produto p1,p2;

    printf("Informe o custo: ");
    scanf("%f",&p1.custo); //Escreve no enderedo de p1
    printf("Informe o lucro: ");
    scanf("%f",&p1.lucro);

    p2 = p1;
    dobraProduto(&p2); //pega o valor que esta armazenado no endereco(&) de p2 e dobra ele 

    p1 = calculaPreco(p1);
    p2 = calculaPreco(p2);

    imprimeProduto(p1);
    imprimeProduto(p2);

    return 1;

}

//Funcao que calcula o preco de um produto e retorna o regsistro 

tipo_produto calculaPreco(tipo_produto prod){
    prod.preco = prod.custo + (prod.custo * prod.lucro / 100);
    return prod; // retorna como uma copia e sobrescreve
}

//Funcao que dobra o valor do custo e lucro
void dobraProduto(tipo_produto *prod){
    (*prod).custo = (*prod).custo * 2; //o ponteiro vai no local de p2 para modificar
    (*prod).lucro = (*prod).lucro * 2;
}

//Funcao que imprime os detalhes de um produto 
void imprimeProduto(tipo_produto prod){
    printf("Custo do produto = R$ %.2f\n", prod.custo);
    printf("Lucro = R$ %.2f\n", prod.lucro);
    printf("Preco do produto = R$ %.2f\n", prod.preco);
    printf("---------------------------------------------------");
}