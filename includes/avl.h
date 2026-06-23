typedef struct No {
    int valor;
    int altura;
    struct No *esquerda;
    struct No *direita;
} No;


int altura(No *n);
int max(int a, int b);
int fatorBalanceamento(No *n);
void atualizarAltura(No *n);
void emOrdem(No *raiz);
void preOrdem(No *raiz);
void destruir(No *raiz);
void inserirAmostrasNaAVL(int **amostra, int tamanho);
No* criarNo(int valor);
No* rotacaoDireita(No *y);
No* rotacaoEsquerda(No *x);
No* balancear(No *n);
No* inserir(No *raiz, int valor);
No* menorNo(No *raiz);
No* removerNo(No *raiz, int valor);
No* buscar(No *raiz, int valor);
