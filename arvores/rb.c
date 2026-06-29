#include <stdio.h>
#include <stdlib.h>

int contadorRB = 0;
long long totalComparacoesInsercaoRB = 0;
long long totalComparacoesRemocaoRB  = 0;

// Estruturas do código 

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
    struct no *nulo;
} Arvore;

// Protótipos

void rotacionarEsquerda(Arvore *arvore, No *no);
void rotacionarDireita(Arvore *arvore, No *no);
void balancear(Arvore *arvore, No *no);
void balancear_remocao(Arvore *arvore, No *no);
void transplantar(Arvore *arvore, No *u, No *v);
No  *minimo(Arvore *arvore, No *no);

// Criação

Arvore *cria() {
    Arvore *arvore = malloc(sizeof(Arvore));
    arvore->nulo = malloc(sizeof(No));
    arvore->nulo->cor = Preto;
    arvore->nulo->esquerda = arvore->nulo;
    arvore->nulo->direita = arvore->nulo;
    arvore->nulo->pai = arvore->nulo;
    arvore->nulo->valor = 0;
    arvore->raiz = arvore->nulo;
    return arvore;
}

int vazia(Arvore *arvore) {
    return (arvore->raiz == arvore->nulo);
}

void rotacionarEsquerda(Arvore *arvore, No *no) {
    No *direita = no->direita;
    no->direita = direita->esquerda;

    if (direita->esquerda != arvore->nulo)
        direita->esquerda->pai = no;

    direita->pai = no->pai;

    if (no->pai == arvore->nulo)
        arvore->raiz = direita;
    else if (no == no->pai->esquerda)
        no->pai->esquerda = direita;
    else
        no->pai->direita = direita;

    direita->esquerda = no;
    no->pai = direita;
}

void rotacionarDireita(Arvore *arvore, No *no) {
    No *esquerda = no->esquerda;
    no->esquerda = esquerda->direita;

    if (esquerda->direita != arvore->nulo)
        esquerda->direita->pai = no;

    esquerda->pai = no->pai;

    if (no->pai == arvore->nulo)
        arvore->raiz = esquerda;
    else if (no == no->pai->esquerda)
        no->pai->esquerda = esquerda;
    else
        no->pai->direita = esquerda;

    esquerda->direita = no;
    no->pai = esquerda;
}

void balancear(Arvore *arvore, No *no) {
    while (no->pai != arvore->nulo && no->pai->cor == Vermelho) {
        if (no->pai == no->pai->pai->esquerda) {
            No *tio = no->pai->pai->direita;
            if (tio->cor == Vermelho) {
                tio->cor = Preto;
                no->pai->cor = Preto;
                no->pai->pai->cor = Vermelho;
                no = no->pai->pai;
            } else {
                if (no == no->pai->direita) {
                    no = no->pai;
                    rotacionarEsquerda(arvore, no);
                }
                no->pai->cor = Preto;
                no->pai->pai->cor = Vermelho;
                rotacionarDireita(arvore, no->pai->pai);
            }
        } else {
            No *tio = no->pai->pai->esquerda;
            if (tio->cor == Vermelho) {
                tio->cor = Preto;
                no->pai->cor = Preto;
                no->pai->pai->cor = Vermelho;
                no = no->pai->pai;
            } else {
                if (no == no->pai->esquerda) {
                    no = no->pai;
                    rotacionarDireita(arvore, no);
                }
                no->pai->cor      = Preto;
                no->pai->pai->cor = Vermelho;
                rotacionarEsquerda(arvore, no->pai->pai);
            }
        }
    }
    arvore->raiz->cor = Preto;
}

void adiciona(Arvore *arvore, int valor) {
    No *novo = malloc(sizeof(No));
    novo->valor = valor;
    novo->cor = Vermelho;
    novo->esquerda = arvore->nulo;
    novo->direita = arvore->nulo;
    novo->pai = arvore->nulo;

    No *pai  = arvore->nulo;
    No *atual = arvore->raiz;

    while (atual != arvore->nulo) {
        pai = atual;
        contadorRB++;                          
        if (novo->valor < atual->valor)
            atual = atual->esquerda;
        else
            atual = atual->direita;
    }

    novo->pai = pai;

    if (pai == arvore->nulo)
        arvore->raiz = novo;
    else if (novo->valor < pai->valor)
        pai->esquerda = novo;
    else
        pai->direita = novo;

    balancear(arvore, novo);
}

void transplantar(Arvore *arvore, No *u, No *v) {
    if (u->pai == arvore->nulo)
        arvore->raiz = v;
    else if (u == u->pai->esquerda)
        u->pai->esquerda = v;
    else
        u->pai->direita = v;
    v->pai = u->pai;
}

No *minimo(Arvore *arvore, No *no) {
    while (no->esquerda != arvore->nulo)
        no = no->esquerda;
    return no;
}

void balancear_remocao(Arvore *arvore, No *no) {
    while (no != arvore->raiz && no->cor == Preto) {
        if (no == no->pai->esquerda) {
            No *irmao = no->pai->direita;
            if (irmao->cor == Vermelho) {
                irmao->cor = Preto;
                no->pai->cor = Vermelho;
                rotacionarEsquerda(arvore, no->pai);
                irmao = no->pai->direita;
            }
            if (irmao->esquerda->cor == Preto && irmao->direita->cor == Preto) {
                irmao->cor = Vermelho;
                no = no->pai;
            } else {
                if (irmao->direita->cor == Preto) {
                    irmao->esquerda->cor = Preto;
                    irmao->cor = Vermelho;
                    rotacionarDireita(arvore, irmao);
                    irmao = no->pai->direita;
                }
                irmao->cor = no->pai->cor;
                no->pai->cor = Preto;
                irmao->direita->cor = Preto;
                rotacionarEsquerda(arvore, no->pai);
                no = arvore->raiz;
            }
        } else {
            No *irmao = no->pai->esquerda;
            if (irmao->cor == Vermelho) {
                irmao->cor = Preto;
                no->pai->cor = Vermelho;
                rotacionarDireita(arvore, no->pai);
                irmao = no->pai->esquerda;
            }
            if (irmao->direita->cor == Preto && irmao->esquerda->cor == Preto) {
                irmao->cor = Vermelho;
                no = no->pai;
            } else {
                if (irmao->esquerda->cor == Preto) {
                    irmao->direita->cor = Preto;
                    irmao->cor = Vermelho;
                    rotacionarEsquerda(arvore, irmao);
                    irmao = no->pai->esquerda;
                }
                irmao->cor = no->pai->cor;
                no->pai->cor = Preto;
                irmao->esquerda->cor = Preto;
                rotacionarDireita(arvore, no->pai);
                no = arvore->raiz;
            }
        }
    }
    no->cor = Preto;
}

void remove_no(Arvore *arvore, No *z) {
    No *y = z;
    No *x;
    Cor cor_original_y = y->cor;

    if (z->esquerda == arvore->nulo) {
        x = z->direita;
        transplantar(arvore, z, z->direita);
    } else if (z->direita == arvore->nulo) {
        x = z->esquerda;
        transplantar(arvore, z, z->esquerda);
    } else {
        // Busca do sucessor: cada passo à esquerda é uma comparação
        No *cur = z->direita;
        while (cur->esquerda != arvore->nulo) {
            contadorRB++;                      // <<< comparação de chave
            cur = cur->esquerda;
        }
        y = cur;

        cor_original_y = y->cor;
        x = y->direita;

        if (y->pai == z) {
            x->pai = y;
        } else {
            transplantar(arvore, y, y->direita);
            y->direita = z->direita;
            y->direita->pai = y;
        }
        transplantar(arvore, z, y);
        y->esquerda = z->esquerda;
        y->esquerda->pai = y;
        y->cor = z->cor;
    }

    free(z);

    if (cor_original_y == Preto)
        balancear_remocao(arvore, x);
}

No *busca(Arvore *arvore, int valor) {
    No *atual = arvore->raiz;
    while (atual != arvore->nulo) {
        contadorRB++;                         
        if (valor == atual->valor)
            return atual;
        else if (valor < atual->valor)
            atual = atual->esquerda;
        else
            atual = atual->direita;
    }
    return NULL;
}

void percorrer(Arvore *arvore, No *no) {
    if (no != arvore->nulo) {
        percorrer(arvore, no->esquerda);
        printf("%d(%s) ", no->valor, no->cor == Vermelho ? "V" : "P");
        percorrer(arvore, no->direita);
    }
}

void liberar_nos(Arvore *arvore, No *no) {
    if (no != arvore->nulo) {
        liberar_nos(arvore, no->esquerda);
        liberar_nos(arvore, no->direita);
        free(no);
    }
}

void liberar(Arvore *arvore) {
    liberar_nos(arvore, arvore->raiz);
    free(arvore->nulo);
    free(arvore);
}

typedef struct {
    int tamanho;
    double mediaInsercao;
    double mediaRemocao;
} ResultadoRB;

ResultadoRB insercaoRemocaoRB(int **amostra, int tamanho) {
    totalComparacoesInsercaoRB = 0;
    totalComparacoesRemocaoRB  = 0;

    for (int i = 0; i < 10; i++) {
        Arvore *arvore = cria();

        // Inserção
        contadorRB = 0;
        for (int j = 0; j < tamanho; j++)
            adiciona(arvore, amostra[i][j]);
        totalComparacoesInsercaoRB += contadorRB;

        // Remoção  
        contadorRB = 0;
        for (int j = 0; j < tamanho; j++) {
            No *alvo = busca(arvore, amostra[i][j]);
            if (alvo)
                remove_no(arvore, alvo);
        }
        totalComparacoesRemocaoRB += contadorRB;

        liberar(arvore);
    }

    ResultadoRB res;
    res.tamanho = tamanho;
    res.mediaInsercao = (double)totalComparacoesInsercaoRB / 10.0;
    res.mediaRemocao = (double)totalComparacoesRemocaoRB  / 10.0;

    printf("RB -> Media Ins: %.2f | Media Rem: %.2f\n",
           res.mediaInsercao, res.mediaRemocao);
    return res;
}