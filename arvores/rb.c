#include <stdio.h>
#include <stdlib.h>

int contadorRB = 0;
long long totalComparacoesInsercaoRB = 0;
long long totalComparacoesRemocaoRB  = 0;

// Estruturas do código 

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
    struct no_RB *nulo;
} Arvore;

// Protótipos

void rotacionarEsquerda(Arvore *arvore, No_RB *no_RB);
void rotacionarDireita(Arvore *arvore, No_RB *no_RB);
void balancear(Arvore *arvore, No_RB *no_RB);
void balancear_remocao(Arvore *arvore, No_RB *no_RB);
void transplantar(Arvore *arvore, No_RB *u, No_RB *v);
No_RB  *minimo(Arvore *arvore, No_RB *no_RB);

// Criação

Arvore *cria() {
    Arvore *arvore = malloc(sizeof(Arvore));
    arvore->nulo = malloc(sizeof(No_RB));
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

void rotacionarEsquerda(Arvore *arvore, No_RB *no_RB) {
    No_RB *direita = no_RB->direita;
    no_RB->direita = direita->esquerda;

    if (direita->esquerda != arvore->nulo)
        direita->esquerda->pai = no_RB;

    direita->pai = no_RB->pai;

    if (no_RB->pai == arvore->nulo)
        arvore->raiz = direita;
    else if (no_RB == no_RB->pai->esquerda)
        no_RB->pai->esquerda = direita;
    else
        no_RB->pai->direita = direita;

    direita->esquerda = no_RB;
    no_RB->pai = direita;
}

void rotacionarDireita(Arvore *arvore, No_RB *no_RB) {
    No_RB *esquerda = no_RB->esquerda;
    no_RB->esquerda = esquerda->direita;

    if (esquerda->direita != arvore->nulo)
        esquerda->direita->pai = no_RB;

    esquerda->pai = no_RB->pai;

    if (no_RB->pai == arvore->nulo)
        arvore->raiz = esquerda;
    else if (no_RB == no_RB->pai->esquerda)
        no_RB->pai->esquerda = esquerda;
    else
        no_RB->pai->direita = esquerda;

    esquerda->direita = no_RB;
    no_RB->pai = esquerda;
}

void balancear(Arvore *arvore, No_RB *no_RB) {
    while (no_RB->pai != arvore->nulo && no_RB->pai->cor == Vermelho) {
        if (no_RB->pai == no_RB->pai->pai->esquerda) {
            No_RB *tio = no_RB->pai->pai->direita;
            if (tio->cor == Vermelho) {
                tio->cor = Preto;
                no_RB->pai->cor = Preto;
                no_RB->pai->pai->cor = Vermelho;
                no_RB = no_RB->pai->pai;
            } else {
                if (no_RB == no_RB->pai->direita) {
                    no_RB = no_RB->pai;
                    rotacionarEsquerda(arvore, no_RB);
                }
                no_RB->pai->cor = Preto;
                no_RB->pai->pai->cor = Vermelho;
                rotacionarDireita(arvore, no_RB->pai->pai);
            }
        } else {
            No_RB *tio = no_RB->pai->pai->esquerda;
            if (tio->cor == Vermelho) {
                tio->cor = Preto;
                no_RB->pai->cor = Preto;
                no_RB->pai->pai->cor = Vermelho;
                no_RB = no_RB->pai->pai;
            } else {
                if (no_RB == no_RB->pai->esquerda) {
                    no_RB = no_RB->pai;
                    rotacionarDireita(arvore, no_RB);
                }
                no_RB->pai->cor      = Preto;
                no_RB->pai->pai->cor = Vermelho;
                rotacionarEsquerda(arvore, no_RB->pai->pai);
            }
        }
    }
    arvore->raiz->cor = Preto;
}

void adiciona(Arvore *arvore, int valor) {
    No_RB *no_RBvo = malloc(sizeof(No_RB));
    no_RBvo->valor = valor;
    no_RBvo->cor = Vermelho;
    no_RBvo->esquerda = arvore->nulo;
    no_RBvo->direita = arvore->nulo;
    no_RBvo->pai = arvore->nulo;

    No_RB *pai  = arvore->nulo;
    No_RB *atual = arvore->raiz;

    while (atual != arvore->nulo) {
        pai = atual;
        contadorRB++;                          
        if (no_RBvo->valor < atual->valor)
            atual = atual->esquerda;
        else
            atual = atual->direita;
    }

    no_RBvo->pai = pai;

    if (pai == arvore->nulo)
        arvore->raiz = no_RBvo;
    else if (no_RBvo->valor < pai->valor)
        pai->esquerda = no_RBvo;
    else
        pai->direita = no_RBvo;

    balancear(arvore, no_RBvo);
}

void transplantar(Arvore *arvore, No_RB *u, No_RB *v) {
    if (u->pai == arvore->nulo)
        arvore->raiz = v;
    else if (u == u->pai->esquerda)
        u->pai->esquerda = v;
    else
        u->pai->direita = v;
    v->pai = u->pai;
}

No_RB *minimo(Arvore *arvore, No_RB *no_RB) {
    while (no_RB->esquerda != arvore->nulo)
        no_RB = no_RB->esquerda;
    return no_RB;
}

void balancear_remocao(Arvore *arvore, No_RB *no_RB) {
    while (no_RB != arvore->raiz && no_RB->cor == Preto) {
        if (no_RB == no_RB->pai->esquerda) {
            No_RB *irmao = no_RB->pai->direita;
            if (irmao->cor == Vermelho) {
                irmao->cor = Preto;
                no_RB->pai->cor = Vermelho;
                rotacionarEsquerda(arvore, no_RB->pai);
                irmao = no_RB->pai->direita;
            }
            if (irmao->esquerda->cor == Preto && irmao->direita->cor == Preto) {
                irmao->cor = Vermelho;
                no_RB = no_RB->pai;
            } else {
                if (irmao->direita->cor == Preto) {
                    irmao->esquerda->cor = Preto;
                    irmao->cor = Vermelho;
                    rotacionarDireita(arvore, irmao);
                    irmao = no_RB->pai->direita;
                }
                irmao->cor = no_RB->pai->cor;
                no_RB->pai->cor = Preto;
                irmao->direita->cor = Preto;
                rotacionarEsquerda(arvore, no_RB->pai);
                no_RB = arvore->raiz;
            }
        } else {
            No_RB *irmao = no_RB->pai->esquerda;
            if (irmao->cor == Vermelho) {
                irmao->cor = Preto;
                no_RB->pai->cor = Vermelho;
                rotacionarDireita(arvore, no_RB->pai);
                irmao = no_RB->pai->esquerda;
            }
            if (irmao->direita->cor == Preto && irmao->esquerda->cor == Preto) {
                irmao->cor = Vermelho;
                no_RB = no_RB->pai;
            } else {
                if (irmao->esquerda->cor == Preto) {
                    irmao->direita->cor = Preto;
                    irmao->cor = Vermelho;
                    rotacionarEsquerda(arvore, irmao);
                    irmao = no_RB->pai->esquerda;
                }
                irmao->cor = no_RB->pai->cor;
                no_RB->pai->cor = Preto;
                irmao->esquerda->cor = Preto;
                rotacionarDireita(arvore, no_RB->pai);
                no_RB = arvore->raiz;
            }
        }
    }
    no_RB->cor = Preto;
}

void remove_no(Arvore *arvore, No_RB *z) {
    No_RB *y = z;
    No_RB *x;
    Cor cor_original_y = y->cor;

    if (z->esquerda == arvore->nulo) {
        x = z->direita;
        transplantar(arvore, z, z->direita);
    } else if (z->direita == arvore->nulo) {
        x = z->esquerda;
        transplantar(arvore, z, z->esquerda);
    } else {
        // Busca do sucessor: cada passo à esquerda é uma comparação
        No_RB *cur = z->direita;
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

No_RB *busca(Arvore *arvore, int valor) {
    No_RB *atual = arvore->raiz;
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

void percorrer(Arvore *arvore, No_RB *no_RB) {
    if (no_RB != arvore->nulo) {
        percorrer(arvore, no_RB->esquerda);
        printf("%d(%s) ", no_RB->valor, no_RB->cor == Vermelho ? "V" : "P");
        percorrer(arvore, no_RB->direita);
    }
}

void liberar_nos(Arvore *arvore, No_RB *no_RB) {
    if (no_RB != arvore->nulo) {
        liberar_nos(arvore, no_RB->esquerda);
        liberar_nos(arvore, no_RB->direita);
        free(no_RB);
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
            No_RB *alvo = busca(arvore, amostra[i][j]);
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