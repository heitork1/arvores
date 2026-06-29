#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "includes/util.h"
#include "includes/avl.h"
#include "includes/b.h"

//gcc arvores\avl.c arvores\b.c arvores\rb.c utilitarios\util.c trabalho_final.c -o trabalho_final.exe ;; .\trabalho_final.exe

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

    fprintf(arquivo, "Tamanho, AVL_Ins, AVL_Rem, RN_Ins, RN_Rem, B1_Splits, B1_Fusoes, B1_Emp, B5_Splits, B5_Fusoes, B5_Emp, B10_Splits, B10_Fusoes, B10_Emp\n");

    int passo = tamanho_maximo / 10; 
    
    printf("Iniciando testes...\n");

    for (int tam = passo; tam <= tamanho_maximo; tam += passo) {
        printf("\n--- Testando tamanho: %d ---\n", tam);

        ResultadoTeste res_avl = {tam, 0, 0};
        ResultadoTeste res_rn = {tam, 0, 0};
        ResultadoB res_b1 = {tam, 0, 0, 0};
        ResultadoB res_b5 = {tam, 0, 0, 0};
        ResultadoB res_b10 = {tam, 0, 0, 0};

        if(avl_pronta) {
            res_avl = insercaoRemocaoAVL(amostra, tam);
        }

        if(rn_pronta) {
        }

        res_b1 = insercaoRemocaoB(amostra, tam, num_amostras, 1);
        res_b5 = insercaoRemocaoB(amostra, tam, num_amostras, 5);
        res_b10 = insercaoRemocaoB(amostra, tam, num_amostras, 10);

        fprintf(arquivo, "%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
                tam,
                res_avl.mediaInsercao, res_avl.mediaRemocao,
                res_rn.mediaInsercao, res_rn.mediaRemocao,
                res_b1.mediaSplits, res_b1.mediaFusoes, res_b1.mediaEmprestimos,
                res_b5.mediaSplits, res_b5.mediaFusoes, res_b5.mediaEmprestimos,
                res_b10.mediaSplits, res_b10.mediaFusoes, res_b10.mediaEmprestimos);
    }

    fclose(arquivo);
    liberar_memoria(amostra, num_amostras);
    
    printf("\nTestes finalizados! Resultados salvos em 'resultados_arvores.csv'.\n");
    return 0;
}
