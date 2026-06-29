#ifndef RB_H
#define RB_H

enum coloracao { Vermelho, Preto };
typedef enum coloracao Cor;

typedef struct no {
    struct no *pai;
    struct no *esquerda;
    struct no *direita;
    Cor cor;
    int valor;
} No;

typedef struct arvore {
    struct no *raiz;
    struct no *nulo; // sentinela: nó folha universal (sempre Preto)
} Arvore;

typedef struct {
    int tamanho;
    double mediaInsercao;
    double mediaRemocao;
} ResultadoRB;

void rotacionarEsquerda(Arvore *arvore, No *no);
void rotacionarDireita(Arvore *arvore, No *no);
void balancear(Arvore *arvore, No *no);
void balancear_remocao(Arvore *arvore, No *no);
void transplantar(Arvore *arvore, No *u, No *v);
No *minimo(Arvore *arvore, No *no);
Arvore *cria();
int vazia(Arvore *arvore);
No *adiciona(Arvore *arvore, int valor);
void transplantar(Arvore *arvore, No *u, No *v);
void remove(Arvore *arvore, No *z);
No *busca(Arvore *arvore, int valor);
void percorrer(Arvore *arvore, No *no);
void liberar_nos(Arvore *arvore, No *no);
void liberar(Arvore *arvore);
ResultadoRB insercaoRemocaoRB(int **amostra, int tamanho);


#endif