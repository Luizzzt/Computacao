/*
 * common.h - Funções auxiliares compartilhadas pelos programas do laboratório.
 * Hierarquia de Memória, Blocagem (Tiling) e Pthreads - Computação Paralela (FCI)
 */
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Tempo atual em segundos (alta precisão, relógio monotônico). */
static inline double agora(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

/* Aloca N*N doubles em um bloco contíguo de memória. */
static inline double *aloca_matriz(int N)
{
    double *M = malloc((size_t)N * (size_t)N * sizeof(double));
    if (M == NULL) {
        fprintf(stderr, "Erro: falha ao alocar matriz %d x %d\n", N, N);
        exit(EXIT_FAILURE);
    }
    return M;
}

/* Aloca N*N doubles zerados (usado para a matriz C). */
static inline double *aloca_matriz_zerada(int N)
{
    double *M = calloc((size_t)N * (size_t)N, sizeof(double));
    if (M == NULL) {
        fprintf(stderr, "Erro: falha ao alocar matriz %d x %d\n", N, N);
        exit(EXIT_FAILURE);
    }
    return M;
}

/* Inicialização determinística definida no enunciado (Item 1). */
static inline void inicializa(double *A, double *B, int N)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = (double)(i + j);
            if (B != NULL) {
                B[i * N + j] = (double)(i * j);
            }
        }
    }
}

/*
 * Verificação de integridade numérica.
 * Com A[i][k] = i + k e B[k][j] = k * j temos:
 *   C[i][j] = soma_k (i + k) * k * j = j * (i * S1 + S2)
 * onde S1 = soma de k (0..N-1) e S2 = soma de k^2 (0..N-1).
 * Todos os valores são inteiros menores que 2^53, portanto o resultado
 * em double é exato e a comparação pode ser feita com igualdade.
 * Retorna 1 se C estiver correta, 0 caso contrário.
 */
static inline int verifica(const double *C, int N)
{
    double n = (double)N;
    double S1 = (n - 1.0) * n / 2.0;
    double S2 = (n - 1.0) * n * (2.0 * n - 1.0) / 6.0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double esperado = (double)j * ((double)i * S1 + S2);
            if (C[i * N + j] != esperado) {
                fprintf(stderr, "Erro em C[%d][%d]: obtido %.1f, esperado %.1f\n",
                        i, j, C[i * N + j], esperado);
                return 0;
            }
        }
    }
    return 1;
}

/* GFLOPS da multiplicação N x N (2*N^3 operações de ponto flutuante). */
static inline double gflops(int N, double tempo)
{
    double n = (double)N;
    return (2.0 * n * n * n) / (tempo * 1e9);
}

/* Lê um inteiro positivo de argv, ou usa o valor padrão. */
static inline int le_argumento(int argc, char **argv, int pos, int padrao)
{
    if (argc > pos) {
        int v = atoi(argv[pos]);
        if (v <= 0) {
            fprintf(stderr, "Erro: argumento %d inválido (%s)\n", pos, argv[pos]);
            exit(EXIT_FAILURE);
        }
        return v;
    }
    return padrao;
}

static inline int min_int(int a, int b)
{
    return a < b ? a : b;
}

#endif /* COMMON_H */
