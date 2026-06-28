#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- ESTRUTURAS DE DADOS ---
typedef struct no {
    int total;
    int* chaves;
    struct no** filhos;
    struct no* pai;
} No;

typedef struct arvoreB {
    No* raiz;
    int ordem;
    // Contadores estatísticos
    long count_splits;
    long count_fusoes;
    long count_emprestimos;
} ArvoreB;

// --- PROTÓTIPOS ---
No* criaNo(ArvoreB* arvore);
void balancearRemocao(ArvoreB* arvore, No* no);

// --- FUNÇÕES DE CRIAÇÃO ---
ArvoreB* criaArvore(int ordem) {
    ArvoreB* a = (ArvoreB*) malloc(sizeof(ArvoreB));
    a->ordem = ordem;
    a->count_splits = 0;
    a->count_fusoes = 0;
    a->count_emprestimos = 0;
    a->raiz = criaNo(a);
    return a;
}

No* criaNo(ArvoreB* arvore) {
    int max = arvore->ordem * 2;
    No* no = (No*) malloc(sizeof(No));
    no->pai = NULL;
    no->chaves = (int*) malloc(sizeof(int) * (max + 1));
    no->filhos = (No**) malloc(sizeof(No*) * (max + 2)); 
    no->total = 0;
    for (int i = 0; i < max + 2; i++) {
        no->filhos[i] = NULL;
    }
    return no;
}

// --- FUNÇÕES DE BUSCA ---
int pesquisaBinaria(No* no, int chave) {
    int inicio = 0, fim = no->total - 1, meio;
    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        if (no->chaves[meio] == chave) {
            return meio; // encontrou
        } else if (no->chaves[meio] > chave) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
    }
    return inicio; // não encontrou
}

No* localizaNo(ArvoreB* arvore, int chave) {
    No *no = arvore->raiz;
    while (no != NULL) {
        int i = pesquisaBinaria(no, chave);
        if (i < no->total && no->chaves[i] == chave)
            return no; // encontrou nó
        
        if (no->filhos[i] == NULL)
            return no; // retorna nó folha onde deveria estar
        else
            no = no->filhos[i];
    }
    return NULL; 
}

// --- FUNÇÕES DE INSERÇÃO ---
void adicionaChaveNo(No* no, No* direita, int chave) {
    int i = pesquisaBinaria(no, chave);
    for (int j = no->total - 1; j >= i; j--) {
        no->chaves[j + 1] = no->chaves[j];
        no->filhos[j + 2] = no->filhos[j + 1];
    }
    no->chaves[i] = chave;
    no->filhos[i + 1] = direita;
    no->total++;
}

int transbordo(ArvoreB *arvore, No *no) {
    return no->total > arvore->ordem * 2;
}

No* divideNo(ArvoreB* arvore, No* no) {
    arvore->count_splits++; // Incrementa contador de splits
    
    int meio = no->total / 2;
    No* novo = criaNo(arvore);
    novo->pai = no->pai;
    
    for (int i = meio + 1; i < no->total; i++) {
        novo->filhos[novo->total] = no->filhos[i];
        novo->chaves[novo->total] = no->chaves[i];
        if (novo->filhos[novo->total] != NULL) 
            novo->filhos[novo->total]->pai = novo;
        novo->total++;
    }
    novo->filhos[novo->total] = no->filhos[no->total];
    if (novo->filhos[novo->total] != NULL) 
        novo->filhos[novo->total]->pai = novo;
    
    no->total = meio;
    return novo;
}

void adicionaChaveRecursivo(ArvoreB* arvore, No* no, No* novo, int chave) {
    adicionaChaveNo(no, novo, chave);
    if (transbordo(arvore, no)) {
        int promovido = no->chaves[arvore->ordem];
        No* noDireita = divideNo(arvore, no);
        
        if (no->pai == NULL) {
            No* raiz = criaNo(arvore);
            raiz->filhos[0] = no;
            adicionaChaveNo(raiz, noDireita, promovido); 
            no->pai = raiz;
            noDireita->pai = raiz;
            arvore->raiz = raiz;
        } else {
            adicionaChaveRecursivo(arvore, no->pai, noDireita, promovido);
        }
    }
}

void adicionaChave(ArvoreB* arvore, int chave) {
    No* no = localizaNo(arvore, chave);
    int i = pesquisaBinaria(no, chave);
    if (i < no->total && no->chaves[i] == chave) return; 
    
    adicionaChaveRecursivo(arvore, no, NULL, chave);
}

// --- FUNÇÕES DE REMOÇÃO ---
void removeChaveNo(No* no, int index) {
    for (int i = index; i < no->total - 1; i++) {
        no->chaves[i] = no->chaves[i + 1];
        no->filhos[i + 1] = no->filhos[i + 2];
    }
    no->total--;
}

void removerChaveRecursivo(ArvoreB* arvore, No* no, int chave) {
    int i = pesquisaBinaria(no, chave);
    
    if (i < no->total && no->chaves[i] == chave) {
        if (no->filhos[i] == NULL) { // É folha
            removeChaveNo(no, i);
            balancearRemocao(arvore, no);
        } else { // É interno (substitui pelo predecessor)
            No* predecessor = no->filhos[i];
            while (predecessor->filhos[predecessor->total] != NULL) {
                predecessor = predecessor->filhos[predecessor->total];
            }
            int chavePredecessora = predecessor->chaves[predecessor->total - 1];
            no->chaves[i] = chavePredecessora;
            removerChaveRecursivo(arvore, predecessor, chavePredecessora);
        }
    } else if (no->filhos[i] != NULL) {
        removerChaveRecursivo(arvore, no->filhos[i], chave);
    }
}

void balancearRemocao(ArvoreB* arvore, No* no) {
    if (no == arvore->raiz) {
        if (no->total == 0 && no->filhos[0] != NULL) {
            arvore->raiz = no->filhos[0];
            arvore->raiz->pai = NULL;
            free(no->chaves); free(no->filhos); free(no);
        }
        return;
    }
    
    if (no->total >= arvore->ordem) return; // Não sofreu underflow
    
    No* pai = no->pai;
    int idx = 0;
    while (idx <= pai->total && pai->filhos[idx] != no) idx++;
    
    // Tenta emprestar do irmão esquerdo
    if (idx > 0 && pai->filhos[idx - 1]->total > arvore->ordem) {
        arvore->count_emprestimos++; 
        
        No* irmaoEsq = pai->filhos[idx - 1];
        for (int i = no->total; i > 0; i--) {
            no->chaves[i] = no->chaves[i - 1];
            no->filhos[i + 1] = no->filhos[i];
        }
        no->filhos[1] = no->filhos[0];
        no->chaves[0] = pai->chaves[idx - 1];
        no->filhos[0] = irmaoEsq->filhos[irmaoEsq->total];
        if (no->filhos[0] != NULL) no->filhos[0]->pai = no;
        no->total++;
        
        pai->chaves[idx - 1] = irmaoEsq->chaves[irmaoEsq->total - 1];
        irmaoEsq->total--;
        return;
    }
    
    // Tenta emprestar do irmão direito
    if (idx < pai->total && pai->filhos[idx + 1]->total > arvore->ordem) {
        arvore->count_emprestimos++; // Conta empréstimo
        
        No* irmaoDir = pai->filhos[idx + 1];
        no->chaves[no->total] = pai->chaves[idx];
        no->filhos[no->total + 1] = irmaoDir->filhos[0];
        if (no->filhos[no->total + 1] != NULL) no->filhos[no->total + 1]->pai = no;
        no->total++;
        
        pai->chaves[idx] = irmaoDir->chaves[0];
        
        // Shift manual no irmão direito
        for (int i = 0; i < irmaoDir->total - 1; i++) {
            irmaoDir->chaves[i] = irmaoDir->chaves[i + 1];
            irmaoDir->filhos[i] = irmaoDir->filhos[i + 1];
        }
        irmaoDir->filhos[irmaoDir->total - 1] = irmaoDir->filhos[irmaoDir->total];
        irmaoDir->total--;
        
        return;
    }
    
    // Fusão (Merge)
    arvore->count_fusoes++; // Conta fusão
    
    if (idx > 0) { // Fundo com irmão esquerdo
        No* irmaoEsq = pai->filhos[idx - 1];
        irmaoEsq->chaves[irmaoEsq->total] = pai->chaves[idx - 1];
        irmaoEsq->total++;
        for (int i = 0; i < no->total; i++) {
            irmaoEsq->chaves[irmaoEsq->total + i] = no->chaves[i];
            irmaoEsq->filhos[irmaoEsq->total + i] = no->filhos[i];
            if (irmaoEsq->filhos[irmaoEsq->total + i] != NULL)
                irmaoEsq->filhos[irmaoEsq->total + i]->pai = irmaoEsq;
        }
        irmaoEsq->filhos[irmaoEsq->total + no->total] = no->filhos[no->total];
        if (irmaoEsq->filhos[irmaoEsq->total + no->total] != NULL)
            irmaoEsq->filhos[irmaoEsq->total + no->total]->pai = irmaoEsq;
        irmaoEsq->total += no->total;
        
        removeChaveNo(pai, idx - 1);
        pai->filhos[idx - 1] = irmaoEsq;
        free(no->chaves); free(no->filhos); free(no);
    } else { // Fundo com irmão direito
        No* irmaoDir = pai->filhos[idx + 1];
        no->chaves[no->total] = pai->chaves[idx];
        no->total++;
        for (int i = 0; i < irmaoDir->total; i++) {
            no->chaves[no->total + i] = irmaoDir->chaves[i];
            no->filhos[no->total + i] = irmaoDir->filhos[i];
            if (no->filhos[no->total + i] != NULL)
                no->filhos[no->total + i]->pai = no;
        }
        no->filhos[no->total + irmaoDir->total] = irmaoDir->filhos[irmaoDir->total];
        if (no->filhos[no->total + irmaoDir->total] != NULL)
            no->filhos[no->total + irmaoDir->total]->pai = no;
        no->total += irmaoDir->total;
        
        removeChaveNo(pai, idx);
        pai->filhos[idx] = no;
        free(irmaoDir->chaves); free(irmaoDir->filhos); free(irmaoDir);
    }
    balancearRemocao(arvore, pai);
}

void removerChave(ArvoreB* arvore, int chave) {
    if (arvore->raiz != NULL) {
        removerChaveRecursivo(arvore, arvore->raiz, chave);
    }
}

// --- FUNÇÃO PARA LIBERAR MEMÓRIA ---
void liberaNo(No* no) {
    if (no == NULL) return;
    for (int i = 0; i <= no->total; i++) {
        liberaNo(no->filhos[i]);
    }
    free(no->chaves);
    free(no->filhos);
    free(no);
}

void liberaArvore(ArvoreB* arvore) {
    liberaNo(arvore->raiz);
    free(arvore);
}

// Estrutura específica para armazenar os resultados da Árvore B
typedef struct {
    int tamanho;
    double mediaSplits;       // Ocorrem durante a inserção
    double mediaFusoes;       // Ocorrem durante a remoção
    double mediaEmprestimos;  // Ocorrem durante a remoção
} ResultadoB;

ResultadoB insercaoRemocaoB(int **amostras, int tamanho_amostra, int num_amostras, int ordem) {
    ResultadoB resultado;
    resultado.tamanho = tamanho_amostra;
    
    long total_splits = 0;
    long total_fusoes = 0;
    long total_emprestimos = 0;
    
    // Itera sobre as 10 amostras pré-geradas na matriz
    for (int a = 0; a < num_amostras; a++) {
        ArvoreB* arvore = criaArvore(ordem);
        
        // --- Loop de Inserção ---
        for (int i = 0; i < tamanho_amostra; i++) {
            adicionaChave(arvore, amostras[a][i]);
        }
        
        // --- Loop de Remoção ---
        for (int i = 0; i < tamanho_amostra; i++) {
            removerChave(arvore, amostras[a][i]);
        }
        
        // Coleta os contadores acumulados por esta árvore específica
        total_splits += arvore->count_splits;
        total_fusoes += arvore->count_fusoes;
        total_emprestimos += arvore->count_emprestimos;
        
        liberaArvore(arvore);
    }
    
    // Calcula as médias das operações estruturais
    resultado.mediaSplits = (double)total_splits / num_amostras;
    resultado.mediaFusoes = (double)total_fusoes / num_amostras;
    resultado.mediaEmprestimos = (double)total_emprestimos / num_amostras;
    
    return resultado;
}
