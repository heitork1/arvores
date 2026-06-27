#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "includes/util.h"
#include "includes/avl.h"

int main (){
    srand(time(NULL));
    int rn_pronta = 0;
    int avl_pronta = 1;
    int tamanho_maximo = 10000;
    int num_amostras = 10;
    int **amostra, i, j;

    amostra = aloca_matriz(num_amostras, tamanho_maximo);
    preencheMatriz(amostra, tamanho_maximo);

FILE *arquivo = fopen("resultados_arvores.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar arquivo de resultados!\n");
        return 1;
    }

    fprintf(arquivo, "Tamanho,AVL_Ins,AVL_Rem,RN_Ins,RN_Rem,B1_Ins,B1_Rem,B2_Ins,B2_Rem,B3_Ins,B3_Rem\n");

    int passo = tamanho_maximo / 10; 
    
    printf("Iniciando testes...\n");

    for (int tam = passo; tam <= tamanho_maximo; tam += passo) {
        printf("\n--- Testando tamanho: %d ---\n", tam);

        ResultadoTeste res_avl = {tam, 0, 0};
        ResultadoTeste res_rn = {tam, 0, 0};
        ResultadoTeste res_b1 = {tam, 0, 0}, res_b2 = {tam, 0, 0}, res_b3 = {tam, 0, 0};

        if(avl_pronta) {
            res_avl = insercaoRemocaoAVL(amostra, tam);
        }

        if(rn_pronta) {
        }

        fprintf(arquivo, "%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
                tam,
                res_avl.mediaInsercao, res_avl.mediaRemocao,
                res_rn.mediaInsercao, res_rn.mediaRemocao,
                res_b1.mediaInsercao, res_b1.mediaRemocao,
                res_b2.mediaInsercao, res_b2.mediaRemocao,
                res_b3.mediaInsercao, res_b3.mediaRemocao);
    }

    fclose(arquivo);
    liberar_memoria(amostra, num_amostras);
    
    printf("\nTestes finalizados! Resultados salvos em 'resultados_arvores.csv'.\n");
    return 0;
}