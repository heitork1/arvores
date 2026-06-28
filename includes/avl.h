#ifndef AVL_H
#define AVL_H
#include "resultados.h"

typedef struct NoAvl {
    int valor;
    int altura;
    struct NoAvl *esquerda;
    struct NoAvl *direita;
} NoAvl;


int altura(NoAvl *n);
int maior(int a, int b);
int fatorBalanceamento(NoAvl *n);
void atualizarAltura(NoAvl *n);
void destruir(NoAvl *raiz);
ResultadoTeste insercaoRemocaoAVL(int **amostra, int tamanho);
NoAvl* criarNoAvl(int valor);
NoAvl* rotacaoDireita(NoAvl *y);
NoAvl* rotacaoEsquerda(NoAvl *x);
NoAvl* balancear(NoAvl *n);
NoAvl* inserir(NoAvl *raiz, int valor);
NoAvl* menorNoAvl(NoAvl *raiz);
NoAvl* removerNoAvl(NoAvl *raiz, int valor);

#endif
