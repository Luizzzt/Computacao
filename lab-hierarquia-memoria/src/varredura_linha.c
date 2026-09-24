/*
 * varredura_linha.c - Item 2: conta elementos pares percorrendo a matriz
 * em ordem de linha (Row-Major). Boa localidade espacial.
 * Uso: ./varredura_linha N
 */
#include "common.h"

#define REPETICOES 3

int main(int argc, char **argv)
{
    int N = le_argumento(argc, argv, 1, 4096);
    double *A = aloca_matriz(N);
    inicializa(A, NULL, N);

    long pares = 0;
    double melhor = 1e30;

    for (int r = 0; r < REPETICOES; r++) {
        double t0 = agora();
        pares = 0;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if ((int)A[i * N + j] % 2 == 0) {
                    pares++;
                }
            }
        }
        double t = agora() - t0;
        if (t < melhor) {
            melhor = t;
        }
    }

    printf("[Varredura Linha ] N: %5d | Pares: %10ld | Tempo: %.6f s\n", N, pares, melhor);
    free(A);
    return 0;
}
