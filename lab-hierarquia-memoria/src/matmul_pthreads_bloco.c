/*
 * matmul_pthreads_bloco.c - Item 4: Pthreads + Blocagem.
 * Cada thread recebe uma faixa contígua de linhas de C e faz a
 * multiplicação dessa faixa usando sub-blocos BS x BS (ordem i, k, j).
 * Uso: ./matmul_pthreads_bloco N T BS
 */
#include <pthread.h>
#include "common.h"

typedef struct {
    const double *A;
    const double *B;
    double *C;
    int N;
    int BS;
    int linha_ini;
    int linha_fim;
} tarefa_t;

static void *trabalho(void *arg)
{
    tarefa_t *t = (tarefa_t *)arg;
    const double *A = t->A;
    const double *B = t->B;
    double *C = t->C;
    int N = t->N;
    int BS = t->BS;

    for (int ii = t->linha_ini; ii < t->linha_fim; ii += BS) {
        int i_fim = min_int(ii + BS, t->linha_fim);
        for (int jj = 0; jj < N; jj += BS) {
            int j_fim = min_int(jj + BS, N);
            for (int kk = 0; kk < N; kk += BS) {
                int k_fim = min_int(kk + BS, N);
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
    return NULL;
}

int main(int argc, char **argv)
{
    int N  = le_argumento(argc, argv, 1, 1024);
    int T  = le_argumento(argc, argv, 2, 4);
    int BS = le_argumento(argc, argv, 3, 64);
    if (T > N) {
        T = N;
    }

    double *A = aloca_matriz(N);
    double *B = aloca_matriz(N);
    double *C = aloca_matriz_zerada(N);
    inicializa(A, B, N);

    pthread_t *threads = malloc((size_t)T * sizeof(pthread_t));
    tarefa_t *tarefas = malloc((size_t)T * sizeof(tarefa_t));
    if (threads == NULL || tarefas == NULL) {
        fprintf(stderr, "Erro: falha de alocação\n");
        return 1;
    }

    int base = N / T;
    int resto = N % T;

    double t0 = agora();
    int inicio = 0;
    for (int id = 0; id < T; id++) {
        int qtd = base + (id < resto ? 1 : 0);
        tarefas[id] = (tarefa_t){A, B, C, N, BS, inicio, inicio + qtd};
        inicio += qtd;
        if (pthread_create(&threads[id], NULL, trabalho, &tarefas[id]) != 0) {
            fprintf(stderr, "Erro ao criar a thread %d\n", id);
            return 1;
        }
    }
    for (int id = 0; id < T; id++) {
        pthread_join(threads[id], NULL);
    }
    double t = agora() - t0;

    int ok = verifica(C, N);
    printf("[Matmul Pthreads+Bloco] N: %d | Threads: %2d | Bloco B: %d | Tempo: %.4f s | GFLOPS: %.2f | Verificação: %s\n",
           N, T, BS, t, gflops(N, t), ok ? "OK" : "FALHOU");

    free(threads);
    free(tarefas);
    free(A);
    free(B);
    free(C);
    return ok ? 0 : 1;
}
