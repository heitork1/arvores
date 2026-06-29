#ifndef RB_H
#define RB_H

enum coloracao { Vermelho, Preto };
typedef enum coloracao Cor;

typedef struct no_RB {
    struct no_RB *pai;
    struct no_RB *esquerda;
    struct no_RB *direita;
    Cor cor;
    int valor;
} No_RB;

typedef struct arvore {
    struct no_RB *raiz;
    struct no_RB *nulo; // sentinela: nó folha universal (sempre Preto)
} Arvore;

typedef struct {
    int tamanho;
    double mediaInsercao;
    double mediaRemocao;
} ResultadoRB;

void rotacionarEsquerda(Arvore *arvore, No_RB *no_RB);
void rotacionarDireita(Arvore *arvore, No_RB *no_RB);
void balancear(Arvore *arvore, No_RB *no_RB);
void balancear_remocao(Arvore *arvore, No_RB *no_RB);
void transplantar(Arvore *arvore, No_RB *u, No_RB *v);
No_RB *minimo(Arvore *arvore, No_RB *no_RB);
Arvore *cria();
int vazia(Arvore *arvore);
No_RB *adiciona(Arvore *arvore, int valor);
void transplantar(Arvore *arvore, No_RB *u, No_RB *v);
No_RB *busca(Arvore *arvore, int valor);
void percorrer(Arvore *arvore, No_RB *no_RB);
void remove_no_RB(Arvore *arvore, No_RB *z);
void liberar_no_RBs(Arvore *arvore, No_RB *no_RB);
void liberar(Arvore *arvore);
ResultadoRB insercaoRemocaoRB(int **amostra, int tamanho);


#endif