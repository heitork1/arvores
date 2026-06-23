#include <stdio.h>
#include <stdlib.h>
#include "../includes/avl.h"
#include <time.h>


/* ===================== ALTURA ===================== */

int altura(No *n)
{
    if (n == NULL)
        return -1;

    return n->altura;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

void atualizarAltura(No *n)
{
    if (n != NULL)
    {
        n->altura = 1 + max(
                            altura(n->esquerda),
                            altura(n->direita));
    }
}

int fatorBalanceamento(No *n)
{
    if (n == NULL)
        return 0;

    return altura(n->esquerda) - altura(n->direita);
}

/* ===================== CRIAÇÃO ===================== */

No *criarNo(int valor)
{
    No *novo = (No *)malloc(sizeof(No));

    novo->valor = valor;
    novo->altura = 0;
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}

/* ===================== ROTAÇÕES ===================== */

No *rotacaoDireita(No *y)
{
    No *x = y->esquerda;
    No *T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    atualizarAltura(y);
    atualizarAltura(x);

    return x;
}

No *rotacaoEsquerda(No *x)
{
    No *y = x->direita;
    No *T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    atualizarAltura(x);
    atualizarAltura(y);

    return y;
}

/* ===================== BALANCEAMENTO ===================== */

No *balancear(No *n)
{

    atualizarAltura(n);

    int fb = fatorBalanceamento(n);

    /* Esquerda pesada */
    if (fb > 1)
    {

        /* Caso LL */
        if (fatorBalanceamento(n->esquerda) >= 0)
            return rotacaoDireita(n);

        /* Caso LR */
        n->esquerda = rotacaoEsquerda(n->esquerda);
        return rotacaoDireita(n);
    }

    /* Direita pesada */
    if (fb < -1)
    {

        /* Caso RR */
        if (fatorBalanceamento(n->direita) <= 0)
            return rotacaoEsquerda(n);

        /* Caso RL */
        n->direita = rotacaoDireita(n->direita);
        return rotacaoEsquerda(n);
    }

    return n;
}

/* ===================== INSERÇÃO ===================== */

No *inserir(No *raiz, int valor)
{

    if (raiz == NULL)
        return criarNo(valor);

    if (valor < raiz->valor)
        raiz->esquerda = inserir(raiz->esquerda, valor);

    else if (valor > raiz->valor)
        raiz->direita = inserir(raiz->direita, valor);

    else
        return raiz; /* não permite repetidos */

    return balancear(raiz);
}



void inserirAmostrasNaAVL(int **amostra, int tamanho)
{
    double somaTempo = 0.0;
    for (int i = 0; i < 10; i++)
    {
        No *raiz = NULL;

        clock_t inicio = clock();
        for (int j = 0; j < tamanho; j++)
        {
            raiz = inserir(raiz, amostra[i][j]);
        }

        clock_t fim = clock();

        double tempo =
            ((double)(fim - inicio)) / CLOCKS_PER_SEC;
        printf("Tempo: %.6f segundos\n", tempo);
        somaTempo += tempo;

        destruir(raiz);
    }
    printf("\n====== MEDIAS ======\n");
    printf("Media Tempo: %.6f segundos\n",
       somaTempo / 10.0);
}

/* ===================== MENOR NÓ ===================== */

No *menorNo(No *raiz)
{

    No *atual = raiz;

    while (atual->esquerda != NULL)
        atual = atual->esquerda;

    return atual;
}

/* ===================== REMOÇÃO ===================== */

No *removerNo(No *raiz, int valor)
{

    if (raiz == NULL)
        return NULL;

    if (valor < raiz->valor)
    {

        raiz->esquerda =
            removerNo(raiz->esquerda, valor);
    }
    else if (valor > raiz->valor)
    {

        raiz->direita =
            removerNo(raiz->direita, valor);
    }
    else
    {

        /* Encontrou */

        if (raiz->esquerda == NULL ||
            raiz->direita == NULL)
        {

            No *temp;

            if (raiz->esquerda != NULL)
                temp = raiz->esquerda;
            else
                temp = raiz->direita;

            free(raiz);

            return temp;
        }

        /* Dois filhos */

        No *sucessor =
            menorNo(raiz->direita);

        raiz->valor = sucessor->valor;

        raiz->direita =
            removerNo(
                raiz->direita,
                sucessor->valor);
    }

    return balancear(raiz);
}

void remocoesAmostrasNaAVL(int **amostra, int tamanho){
    double somaTempo = 0.0;
    for (int i = 0; i < 10; i++)
    {
        No *raiz = NULL;

        clock_t inicio = clock();
        for (int j = 0; j < tamanho; j++)
        {
            raiz = removerNo(raiz, amostra[i][j]);
        }

        clock_t fim = clock();

        double tempo =
            ((double)(fim - inicio)) / CLOCKS_PER_SEC;
        printf("Tempo: %.6f segundos\n", tempo);
        somaTempo += tempo;

        destruir(raiz);
    }
    printf("\n====== MEDIAS ======\n");
    printf("Media Tempo: %.6f segundos\n",
       somaTempo / 10.0);
}

/* ===================== BUSCA ===================== */

No *buscar(No *raiz, int valor)
{

    if (raiz == NULL)
        return NULL;

    if (valor == raiz->valor)
        return raiz;

    if (valor < raiz->valor)
        return buscar(raiz->esquerda, valor);

    return buscar(raiz->direita, valor);
}

/* ===================== PERCURSOS ===================== */

void emOrdem(No *raiz)
{

    if (raiz != NULL)
    {

        emOrdem(raiz->esquerda);

        printf("%d ", raiz->valor);

        emOrdem(raiz->direita);
    }
}

void preOrdem(No *raiz)
{

    if (raiz != NULL)
    {

        printf("%d ", raiz->valor);

        preOrdem(raiz->esquerda);

        preOrdem(raiz->direita);
    }
}

/* ===================== LIBERAR ===================== */

void destruir(No *raiz)
{

    if (raiz != NULL)
    {

        destruir(raiz->esquerda);
        destruir(raiz->direita);

        free(raiz);
    }
}
