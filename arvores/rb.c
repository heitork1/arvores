#include <stdio.h>
#include <stdlib.h>

// Definições de tipo

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


// Protótipos (necessários pois balancear chama as rotações 
// antes delas serem definidas no arquivo original)

void rotacionarEsquerda(Arvore *arvore, No *no);
void rotacionarDireita(Arvore *arvore, No *no);
void balancear(Arvore *arvore, No *no);
void balancear_remocao(Arvore *arvore, No *no);
void transplantar(Arvore *arvore, No *u, No *v);
No *minimo(Arvore *arvore, No *no);

// Criação da árvore

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

// Verifica se a árvore está vazia

int vazia(Arvore *arvore) {
    return (arvore->raiz == arvore->nulo);
}

// Rotação à esquerda

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

// Rotação à direita

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

// Balanceamento pós-inserção

void balancear(Arvore *arvore, No *no) {
    while (no->pai != arvore->nulo && no->pai->cor == Vermelho) {

        if (no->pai == no->pai->pai->esquerda) {
            // Pai é filho ESQUERDO do avô
            No *tio = no->pai->pai->direita;

            if (tio->cor == Vermelho) {
                // Caso 2 — tio vermelho: recolorir e subir
                tio->cor = Preto;
                no->pai->cor = Preto;
                no->pai->pai->cor = Vermelho;
                no = no->pai->pai;
            } else {
                if (no == no->pai->direita) {
                    // Caso 3 — nó é filho direito: rotação à esquerda no pai
                    no = no->pai;
                    rotacionarEsquerda(arvore, no);
                }
                // Caso 4 — nó é filho esquerdo: rotação à direita no avô
                no->pai->cor = Preto;
                no->pai->pai->cor = Vermelho;
                rotacionarDireita(arvore, no->pai->pai);
            }
        } else {
            // Pai é filho DIREITO do avô (caso simétrico)
            No *tio = no->pai->pai->esquerda;

            if (tio->cor == Vermelho) {
                // Caso 2 simétrico
                tio->cor = Preto;
                no->pai->cor = Preto;
                no->pai->pai->cor = Vermelho;
                no = no->pai->pai;
            } else {
                if (no == no->pai->esquerda) {
                    // Caso 3 simétrico
                    no = no->pai;
                    rotacionarDireita(arvore, no);
                }
                // Caso 4 simétrico
                no->pai->cor      = Preto;
                no->pai->pai->cor = Vermelho;
                rotacionarEsquerda(arvore, no->pai->pai);
            }
        }
    }
    // Caso 1 — garante que a raiz seja sempre Preta
    arvore->raiz->cor = Preto;
}

No *adiciona(Arvore *arvore, int valor) {
    No *novo = malloc(sizeof(No));
    novo->valor = valor;
    novo->cor = Vermelho; // todo nó novo é Vermelho
    novo->esquerda = arvore->nulo;
    novo->direita = arvore->nulo;
    novo->pai = arvore->nulo;

    // Busca BST para encontrar a posição correta
    No *pai  = arvore->nulo;
    No *atual = arvore->raiz;

    while (atual != arvore->nulo) {
        pai = atual;
        if (novo->valor < atual->valor)
            atual = atual->esquerda;
        else
            atual = atual->direita;
    }

    novo->pai = pai;

    if (pai == arvore->nulo)
        arvore->raiz = novo; // árvore estava vazia
    else if (novo->valor < pai->valor)
        pai->esquerda = novo;
    else
        pai->direita = novo;

    balancear(arvore, novo);
    return novo;
}

// Funções auxiliares para remoção

// Substitui a subárvore enraizada em `u` pela subárvore enraizada em `v`
void transplantar(Arvore *arvore, No *u, No *v) {
    if (u->pai == arvore->nulo)
        arvore->raiz = v;
    else if (u == u->pai->esquerda)
        u->pai->esquerda = v;
    else
        u->pai->direita = v;
    v->pai = u->pai;
}

// Retorna o nó com menor valor na subárvore enraizada em `no`
No *minimo(Arvore *arvore, No *no) {
    while (no->esquerda != arvore->nulo)
        no = no->esquerda;
    return no;
}

// Balanceamento pós-remoção

void balancear_remocao(Arvore *arvore, No *no) {
    while (no != arvore->raiz && no->cor == Preto) {

        if (no == no->pai->esquerda) {
            No *irmao = no->pai->direita;

            // Caso 1 — irmão é Vermelho
            if (irmao->cor == Vermelho) {
                irmao->cor      = Preto;
                no->pai->cor = Vermelho;
                rotacionarEsquerda(arvore, no->pai);
                irmao = no->pai->direita;
            }
            // Caso 2 — irmão e seus dois filhos são Pretos
            if (irmao->esquerda->cor == Preto && irmao->direita->cor == Preto) {
                irmao->cor = Vermelho;
                no      = no->pai;
            } else {
                // Caso 3 — filho direito do irmão é Preto
                if (irmao->direita->cor == Preto) {
                    irmao->esquerda->cor = Preto;
                    irmao->cor           = Vermelho;
                    rotacionarDireita(arvore, irmao);
                    irmao = no->pai->direita;
                }
                // Caso 4 — filho direito do irmão é Vermelho
                irmao->cor           = no->pai->cor;
                no->pai->cor      = Preto;
                irmao->direita->cor  = Preto;
                rotacionarEsquerda(arvore, no->pai);
                no = arvore->raiz; // encerra o loop
            }
        } else {
            // Caso simétrico: no é filho DIREITO
            No *irmao = no->pai->esquerda;

            if (irmao->cor == Vermelho) {
                irmao->cor      = Preto;
                no->pai->cor = Vermelho;
                rotacionarDireita(arvore, no->pai);
                irmao = no->pai->esquerda;
            }
            if (irmao->direita->cor == Preto && irmao->esquerda->cor == Preto) {
                irmao->cor = Vermelho;
                no      = no->pai;
            } else {
                if (irmao->esquerda->cor == Preto) {
                    irmao->direita->cor = Preto;
                    irmao->cor          = Vermelho;
                    rotacionarEsquerda(arvore, irmao);
                    irmao = no->pai->esquerda;
                }
                irmao->cor           = no->pai->cor;
                no->pai->cor      = Preto;
                irmao->esquerda->cor = Preto;
                rotacionarDireita(arvore, no->pai);
                no = arvore->raiz;
            }
        }
    }
    no->cor = Preto;
}

// Remoção de um nó específico

void remove(Arvore *arvore, No *z) {
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
        // Nó tem dois filhos: substitui pelo sucessor (mínimo da subárvore direita)
        y = minimo(arvore, z->direita);
        cor_original_y = y->cor;
        x = y->direita;

        if (y->pai == z) {
            x->pai = y;
        } else {
            transplantar(arvore, y, y->direita);
            y->direita      = z->direita;
            y->direita->pai = y;
        }
        transplantar(arvore, z, y);
        y->esquerda      = z->esquerda;
        y->esquerda->pai = y;
        y->cor           = z->cor;
    }

    free(z);

    // Rebalanceia apenas se um nó Preto foi removido (altura-negra comprometida)
    if (cor_original_y == Preto)
        balancear_remocao(arvore, x);
}

No *busca(Arvore *arvore, int valor) {
    No *atual = arvore->raiz;
    while (atual != arvore->nulo) {
        if (valor == atual->valor)
            return atual;
        else if (valor < atual->valor)
            atual = atual->esquerda;
        else
            atual = atual->direita;
    }
    return NULL; // não encontrado
}

// Percurso em-ordem (exibe valores em ordem crescente)

void percorrer(Arvore *arvore, No *no) {
    if (no != arvore->nulo) {
        percorrer(arvore, no->esquerda);
        printf("%d(%s) ", no->valor, no->cor == Vermelho ? "V" : "P");
        percorrer(arvore, no->direita);
    }
}

// Liberação de memória

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

// main — demonstração

int main(void) {
    Arvore *arvore = cria();

    int valores[] = {10, 20, 30, 15, 5, 25, 1, 18};
    int n = sizeof(valores) / sizeof(valores[0]);

    printf("=== Inserindo valores ===\n");
    for (int i = 0; i < n; i++) {
        adiciona(arvore, valores[i]);
        printf("Inserido %d → em-ordem: ", valores[i]);
        percorrer(arvore, arvore->raiz);
        printf("\n");
    }

    printf("\n=== Buscando 15 ===\n");
    No *encontrado = busca(arvore, 15);
    if (encontrado)
        printf("Encontrado: %d\n", encontrado->valor);
    else
        printf("Não encontrado.\n");

    printf("\n=== Removendo 20 ===\n");
    No *para_remover = busca(arvore, 20);
    if (para_remover) {
        remove(arvore, para_remover);
        printf("Após remoção, em-ordem: ");
        percorrer(arvore, arvore->raiz);
        printf("\n");
    }

    printf("\n=== Removendo raiz (%d) ===\n", arvore->raiz->valor);
    remove(arvore, arvore->raiz);
    printf("Após remoção, em-ordem: ");
    percorrer(arvore, arvore->raiz);
    printf("\n");

    liberar(arvore);
    return 0;
}