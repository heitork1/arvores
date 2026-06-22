#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int random_number(){
    return (rand() % 10000) + 1;
}

int **aloca_matriz(int linhas, int colunas){
    int **matriz;
    matriz = malloc(linhas * sizeof(int*));

    for(int i=0; i < linhas; i++){
        matriz[i] = malloc(colunas * sizeof(int));
    }
    return matriz;
}

int liberar_memoria(int **matriz, int linhas){
    for(int i=0; i < linhas;i++){
        free(matriz[i]);
    }
    free(matriz);
    return 0;
}

//=================================================================================
// Estruturas para a Árvore AVL
//=================================================================================

// ==============================================================================
// Estruturas para a Árvore Rubro Negra
// ==============================================================================

/*
// Atributo exclusivo das árvores rubro negra
enum coloracao { Vermelho, Preto };
typedef enum coloracao Cor;

// Eu nomeei esse no como no_rn para não interferir com as outras estruturas no futuro
// no_RubroNegro -> no_rn

typedef struct no_rn{
    struct no_rn* pai;
    struct no_rn* esquerda;
    struct no_rn* direita;
    Cor cor; 
    int valor;
} No_rn;

// Eu nomeei essa arvore como arvore_rn para não interferir com as outras estruturas no futuro
// arvore_RubroNegro -> arvore_rn

typedef struct arvore_rn {
    struct no_rn* raiz;
} Arvore_rn;

Arvore_rn* cria_rn() {
    Arvore_rn *arvore_rn;
    arvore_rn = malloc(sizeof(Arvore_rn));
    arvore_rn->raiz = NULL;

    return arvore_rn;
}

int vazia_rn(Arvore_rn* arvore_rn) {
    return (arvore_rn->raiz == NULL);
}

No_rn* adiciona_rn(Arvore_rn* arvore_rn, No_rn* pai, float valor) {
    No_rn *no_rn = malloc(sizeof(No_rn));
    no_rn->pai = pai;
    no_rn->esquerda = NULL;
    no_rn->direita = NULL;
    no_rn->valor = valor;

    if (pai == NULL) {
        arvore_rn->raiz = no_rn;
    }
    return no_rn;
}

void remove_rn(Arvore_rn* arvore_rn, No_rn* no_rn) {
    if (no_rn->esquerda != NULL){
        remove_rn(arvore_rn, no_rn->esquerda);
    }
    

    if (no_rn->direita != NULL){
        remove_rn(arvore_rn, no_rn->direita);
    }
    

    if (no_rn->pai == NULL) {
        arvore_rn->raiz = NULL;
    } else {
        if (no_rn->pai->esquerda == no_rn){
            no_rn->pai->esquerda = NULL;
        } else {
            no_rn->pai->direita = NULL;
        }
    }
    free(no_rn);
}

void percorrer_rn(No_rn* no_rn) {
    if (no_rn != NULL) {
        printf("%f", no_rn->valor);
        percorrer_rn(no_rn->esquerda);
        percorrer_rn(no_rn->direita);
    }
}

void balancear_rn(Arvore_rn* arvore_rn, No_rn* no_rn) {
    while (no_rn->pai->cor == Vermelho) { //Garante que todos os níveis foram balanceados
        if (no_rn->pai == no_rn->pai->pai->esquerda) {
        No_rn *tio = no_rn->pai->pai->direita;

        if (tio->cor == Vermelho) {
            tio->cor = Preto; //Resolve caso 2
            no_rn->pai->cor = Preto;
            no_rn->pai->pai->cor = Vermelho;
            no_rn = no_rn->pai->pai; //Vai para o nível anterior
            } else {
                if (no_rn == no_rn->pai->direita) {
                    no_rn = no_rn->pai; //Vai para o nível anterior
                    rotacionarEsquerda(arvore_rn, no_rn); //Resolve caso 3
                } else {
                    no_rn->pai->cor = Preto; //Resolve caso 4
                    no_rn->pai->pai->cor = Vermelho;
                    rotacionarDireita(arvore_rn, no_rn->pai->pai);
                }
            }
        } else {
            //Repete o mesmo código do bloco if, invertendo o lado dos direita e esquerda
        }
    }
    arvore_rn->raiz->cor = Preto; //Resolve caso 1
}


void rotacionarEsquerda_rn(Arvore_rn* arvore_rn, No_rn* no_rn) {
    No_rn* direita = no_rn->direita;
    no_rn->direita = direita->esquerda;
    
    if (direita->esquerda != arvore_rn->nulo){
        direita->esquerda->pai = no_rn; //Se houver filho à esquerda em direita, ele será pai do nó
    }
    direita->pai = no_rn->pai; //Ajusta no_rn pai do nó à direita

    if (no_rn->pai == arvore_rn->nulo){
        arvore_rn->raiz = direita; //Se nó for raiz, o nó direita será a nova raiz da árvore
    }
    else if (no_rn == no_rn->pai->esquerda){
        no_rn->pai->esquerda = direita; //Corrige relação pai-filho do novo pai (esquerda)
    } else {
        no_rn->pai->direita = direita; //Corrige relação pai-filho do novo pai (direita)
    }
    
    direita->esquerda = no_rn; //Corrige relação pai-filho entre o nó pivô e o nó à direita
    no_rn->pai = direita;
}

void rotacionarDireita_rn(Arvore_rn* arvore_rn, No_rn* no_rn) {
    No_rn* esquerda = no_rn->esquerda;
    no_rn->esquerda = esquerda>direita;
    if (esquerda->direita != arvore_rn->nulo){
        esquerda->direita->pai = no_rn; //Se houver filho à direita em esquerda, ele será pai do nó
    }
    esquerda->pai = no_rn->pai; //Ajusta no_rn pai do nó à esquerda

    if (no_rn->pai == arvore_rn->nulo){
        arvore_rn->raiz = esquerda; //Se nó for raiz, o nó esquerda será a nova raiz da árvore
    }
    else if (no_rn == no_rn->pai->esquerda){
        no_rn->pai->esquerda = esquerda; //Corrige relação pai-filho do novo pai (esquerda)
    }
    else{
        no_rn->pai->direita = esquerda; //Corrige relação pai-filho do novo pai (direita)
    }
    esquerda->direita = no_rn; //Corrige relação pai-filho entre o nó pivô e o nó à esquerda
    no_rn->pai = esquerda;
}

*/

//=================================================================================
// Estruturas para a Árvore B
//=================================================================================


int main (){
    srand(time(NULL));

    int tamanho = 10000;
    int **amostra;
    amostra = aloca_matriz(10, tamanho);
    int comparacoes = 0;

    arvore_rn = cria_rn();

    
    //No tree;
    //tree = new_no(10, tree);
    

    liberar_memoria(amostra, 10);
    return 0;
}