#include "../includes/util.h"

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

void preencheMatriz(int **amostra, int tamanho){
    for(int i=0; i<10; i++){
        for(int j = 0; j < tamanho; j++){
            amostra[i][j] = random_number();
        }
    }
}
