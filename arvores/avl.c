#include <stdio.h>
#include <stdlib.h>
#include "../includes/avl.h"
#include <time.h>

long long comparacoesInsercaoAVL = 0;
long long comparacoesRemocaoAVL = 0;
long long contadorAVL = 0;

typedef struct {
    int tamanho;
    double mediaInsercao;
    double mediaRemocao;
} ResultadoAVL;

/* ===================== ALTURA ===================== */

int altura(No *n)
{
    contadorAVL++;
    if (n == NULL)
        return -1;

    return n->altura;
}

int maior(int a, int b)
{
    contadorAVL++;
    return (a > b) ? a : b;
}

void atualizarAltura(No *n)
{
    contadorAVL++;
    if (n != NULL)
    {
        n->altura = 1 + maior(
                            altura(n->esquerda),
                            altura(n->direita));
    }
}

int fatorBalanceamento(No *n)
{
    contadorAVL++;
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

    contadorAVL++;
    /* Esquerda pesada */
    if (fb > 1)
    {

        contadorAVL++;
        /* Caso LL */
        if (fatorBalanceamento(n->esquerda) >= 0)
            return rotacaoDireita(n);

        /* Caso LR */
        n->esquerda = rotacaoEsquerda(n->esquerda);
        return rotacaoDireita(n);
    }

    contadorAVL++;
    /* Direita pesada */
    if (fb < -1)
    {

        contadorAVL++;
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

    contadorAVL++;
    if (raiz == NULL)
        return criarNo(valor);
        
    contadorAVL++;
    if (valor < raiz->valor)
        raiz->esquerda = inserir(raiz->esquerda, valor);


    else if (valor > raiz->valor){
        raiz->direita = inserir(raiz->direita, valor);
        contadorAVL++;
    }
    else {
        contadorAVL++;
        return raiz; 
    }

    return balancear(raiz);
}

/* ===================== MENOR NÓ ===================== */

No *menorNo(No *raiz)
{

    No *atual = raiz;
    contadorAVL++;
    while (atual->esquerda != NULL){
        contadorAVL++;
        atual = atual->esquerda;
    }
        

    return atual;
}

/* ===================== REMOÇÃO ===================== */

No *removerNo(No *raiz, int valor)
{
    
    contadorAVL++;
    if (raiz == NULL)
        return NULL;

    contadorAVL++;
    if (valor < raiz->valor)
    {

        raiz->esquerda =
            removerNo(raiz->esquerda, valor);
    }
    else if (valor > raiz->valor)
    {
        contadorAVL++;
        raiz->direita =
            removerNo(raiz->direita, valor);
    }
    else
    {
        contadorAVL++;

        /* Encontrou */

        contadorAVL++;
        if (raiz->esquerda == NULL ||
            raiz->direita == NULL)
        {

            No *temp;

            contadorAVL++;
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

/* ====================== AMOSTRA ======================*/

ResultadoTeste insercaoRemocaoAVL(int **amostra, int tamanho)
{
    long long totalComparacoesInsercaoAVL = 0;
    long long totalComparacoesRemocaoAVL = 0;
    
    for (int i = 0; i < 10; i++)
    {
        No *raiz = NULL;

        contadorAVL = 0;
        for (int j = 0; j < tamanho; j++)
        {
            raiz = inserir(raiz, amostra[i][j]);
        }
        totalComparacoesInsercaoAVL += contadorAVL;

        contadorAVL = 0;
        for (int j = 0; j < tamanho; j++)
        {
            raiz = removerNo(raiz, amostra[i][j]);
        }
        totalComparacoesRemocaoAVL += contadorAVL;

        destruir(raiz);
    }

    ResultadoTeste res;
    res.tamanho = tamanho;
    res.mediaInsercao = (double)totalComparacoesInsercaoAVL / 10.0;
    res.mediaRemocao = (double)totalComparacoesRemocaoAVL / 10.0;

    printf("AVL -> Media Ins: %.2f | Media Rem: %.2f\n", res.mediaInsercao, res.mediaRemocao);

    return res;
}