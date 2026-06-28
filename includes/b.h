#ifndef ARVORE_B_H
#define ARVORE_B_H


// Struct de Resultados 
typedef struct {
    int tamanho;
    double mediaSplits;
    double mediaFusoes;
    double mediaEmprestimos;
} ResultadoB;

// Struct do Nó da Árvore B
typedef struct no {
    int total;
    int* chaves;
    struct no** filhos;
    struct no* pai;
} No;

// Struct de Controle da Árvore B
typedef struct arvoreB {
    No* raiz;
    int ordem;
    
    long count_splits;
    long count_fusoes;
    long count_emprestimos;
} ArvoreB;

// Funções necessarias da Árvore B
ArvoreB* criaArvore(int ordem);
void liberaArvore(ArvoreB* arvore);
void adicionaChave(ArvoreB* arvore, int chave);
void removerChave(ArvoreB* arvore, int chave);
ResultadoB insercaoRemocaoB(int **amostras, int tamanho_amostra, int num_amostras, int ordem);

#endif 
