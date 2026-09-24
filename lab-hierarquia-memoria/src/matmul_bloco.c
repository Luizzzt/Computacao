/*
 * matmul_bloco.c - Item 3: multiplicação de matrizes com blocagem (tiling).
 * Dentro de cada bloco os laços seguem a ordem i, k, j, de modo que
 * B[k][j] e C[i][j] são percorridos de forma contígua (Row-Major).
 * Uso: ./matmul_bloco N BS     (BS = tamanho do bloco, padrão 64)
 */
#include "common.h"

static void matmul_bloco(const double *A, const double *B, double *C, int N, int BS)
{
    for (int ii = 0; ii < N; ii += BS) {
        int i_fim = min_int(ii + BS, N);
        for (int jj = 0; jj < N; jj += BS) {
            int j_fim = min_int(jj + BS, N);
            for (int kk = 0; kk < N; kk += BS) {
                int k_fim = min_int(kk + BS, N);
                /* Multiplicação do bloco BS x BS */
                for (int i = ii; i < i_fim; i++) {
                    for (int k = kk; k < k_fim; k++) {
                        double r = A[i * N + k];
                        for (int j = jj; j < j_fim; j++) {
                            C[i * N + j] += r * B[k * N + j];
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char **argv)
{
    int N  = le_argumento(argc, argv, 1, 512);
    int BS = le_argumento(argc, argv, 2, 64);

    double *A = aloca_matriz(N);
    double *B = aloca_matriz(N);
    double *C = aloca_matriz_zerada(N);   /* C precisa começar zerada (acumulação) */
    inicializa(A, B, N);

    double t0 = agora();
    matmul_bloco(A, B, C, N, BS);
    double t = agora() - t0;

    int ok = verifica(C, N);
    printf("[Matmul Blocado] N: %d | Bloco B: %d | Tempo: %.4f s | GFLOPS: %.2f | Verificação: %s\n",
           N, BS, t, gflops(N, t), ok ? "OK" : "FALHOU");

    free(A);
    free(B);
    free(C);
    return ok ? 0 : 1;
}
