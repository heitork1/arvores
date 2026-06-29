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

int altura(NoAvl *n)
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

void atualizarAltura(NoAvl *n)
{
    contadorAVL++;
    if (n != NULL)
    {
        n->altura = 1 + maior(
                            altura(n->esquerda),
                            altura(n->direita));
    }
}

int fatorBalanceamento(NoAvl *n)
{
    contadorAVL++;
    if (n == NULL)
        return 0;

    return altura(n->esquerda) - altura(n->direita);
}

/* ===================== CRIAÇÃO ===================== */

NoAvl *criarNoAvl(int valor)
{
    NoAvl *novo = (NoAvl *)malloc(sizeof(NoAvl));

    novo->valor = valor;
    novo->altura = 0;
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}

/* ===================== ROTAÇÕES ===================== */

NoAvl *rotacaoDireita(NoAvl *y)
{
    NoAvl *x = y->esquerda;
    NoAvl *T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    atualizarAltura(y);
    atualizarAltura(x);

    return x;
}

NoAvl *rotacaoEsquerda(NoAvl *x)
{
    NoAvl *y = x->direita;
    NoAvl *T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    atualizarAltura(x);
    atualizarAltura(y);

    return y;
}

/* ===================== BALANCEAMENTO ===================== */

NoAvl *balancearAvl(NoAvl *n)
{

    atualizarAltura(n);

    int fb = fatorBalanceamento(n);

    contadorAVL++;
    if (fb > 1)
    {

        contadorAVL++;
        if (fatorBalanceamento(n->esquerda) >= 0)
            return rotacaoDireita(n);

        n->esquerda = rotacaoEsquerda(n->esquerda);
        return rotacaoDireita(n);
    }

    contadorAVL++;
    if (fb < -1)
    {

        contadorAVL++;
        if (fatorBalanceamento(n->direita) <= 0)
            return rotacaoEsquerda(n);

        n->direita = rotacaoDireita(n->direita);
        return rotacaoEsquerda(n);
    }

    return n;
}

/* ===================== INSERÇÃO ===================== */

NoAvl *inserir(NoAvl *raiz, int valor)
{

    contadorAVL++;
    if (raiz == NULL)
        return criarNoAvl(valor);
        
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

    return balancearAvl(raiz);
}

/* ===================== MENOR NÓ ===================== */

NoAvl *menorNoAvl(NoAvl *raiz)
{

    NoAvl *atual = raiz;
    contadorAVL++;
    while (atual->esquerda != NULL){
        contadorAVL++;
        atual = atual->esquerda;
    }
        

    return atual;
}

/* ===================== REMOÇÃO ===================== */

NoAvl *removerNoAvl(NoAvl *raiz, int valor)
{
    
    contadorAVL++;
    if (raiz == NULL)
        return NULL;

    contadorAVL++;
    if (valor < raiz->valor)
    {

        raiz->esquerda =
            removerNoAvl(raiz->esquerda, valor);
    }
    else if (valor > raiz->valor)
    {
        contadorAVL++;
        raiz->direita =
            removerNoAvl(raiz->direita, valor);
    }
    else
    {
        contadorAVL++;


        contadorAVL++;
        if (raiz->esquerda == NULL ||
            raiz->direita == NULL)
        {

            NoAvl *temp;

            contadorAVL++;
            if (raiz->esquerda != NULL)
                temp = raiz->esquerda;
            else
                temp = raiz->direita;

            free(raiz);

            return temp;
        }


        NoAvl *sucessor =
            menorNoAvl(raiz->direita);

        raiz->valor = sucessor->valor;

        raiz->direita =
            removerNoAvl(
                raiz->direita,
                sucessor->valor);
    }

    return balancearAvl(raiz);
}

/* ===================== LIBERAR ===================== */

void destruir(NoAvl *raiz)
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
        NoAvl *raiz = NULL;

        contadorAVL = 0;
        for (int j = 0; j < tamanho; j++)
        {
            raiz = inserir(raiz, amostra[i][j]);
        }
        totalComparacoesInsercaoAVL += contadorAVL;

        contadorAVL = 0;
        for (int j = 0; j < tamanho; j++)
        {
            raiz = removerNoAvl(raiz, amostra[i][j]);
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