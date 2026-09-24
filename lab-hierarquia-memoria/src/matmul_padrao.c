/*
 * matmul_padrao.c - Item 3: multiplicação de matrizes canônica (laços i, j, k).
 * Uso: ./matmul_padrao N
 */
#include "common.h"

static void matmul_padrao(const double *A, const double *B, double *C, int N)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double soma = 0.0;
            for (int k = 0; k < N; k++) {
                soma += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = soma;
        }
    }
}

int main(int argc, char **argv)
{
    int N = le_argumento(argc, argv, 1, 512);

    double *A = aloca_matriz(N);
    double *B = aloca_matriz(N);
    double *C = aloca_matriz_zerada(N);
    inicializa(A, B, N);

    double t0 = agora();
    matmul_padrao(A, B, C, N);
    double t = agora() - t0;

    int ok = verifica(C, N);
    printf("[Matmul Padrão] N: %d | Tempo: %.4f s | GFLOPS: %.2f | Verificação: %s\n",
           N, t, gflops(N, t), ok ? "OK" : "FALHOU");

    free(A);
    free(B);
    free(C);
    return ok ? 0 : 1;
}
