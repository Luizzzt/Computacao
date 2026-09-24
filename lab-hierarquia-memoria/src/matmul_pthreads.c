/*
 * matmul_pthreads.c - Item 4: multiplicação canônica paralela com Pthreads.
 * As N linhas de C são divididas em faixas contíguas, uma por thread.
 * Cada thread escreve só na sua faixa, então não há mutex.
 * Uso: ./matmul_pthreads N T    (T = número de threads)
 */
#include <pthread.h>
#include "common.h"

typedef struct {
    const double *A;
    const double *B;
    double *C;
    int N;
    int linha_ini;   /* primeira linha (inclusiva) */
    int linha_fim;   /* última linha (exclusiva)   */
} tarefa_t;

static void *trabalho(void *arg)
{
    tarefa_t *t = (tarefa_t *)arg;
    const double *A = t->A;
    const double *B = t->B;
    double *C = t->C;
    int N = t->N;

    for (int i = t->linha_ini; i < t->linha_fim; i++) {
        for (int j = 0; j < N; j++) {
            double soma = 0.0;
            for (int k = 0; k < N; k++) {
                soma += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = soma;
        }
    }
    return NULL;
}

int main(int argc, char **argv)
{
    int N = le_argumento(argc, argv, 1, 1024);
    int T = le_argumento(argc, argv, 2, 4);
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

    /* Divisão em faixas contíguas; as primeiras 'resto' threads pegam 1 linha a mais */
    int base = N / T;
    int resto = N % T;

    double t0 = agora();
    int inicio = 0;
    for (int id = 0; id < T; id++) {
        int qtd = base + (id < resto ? 1 : 0);
        tarefas[id] = (tarefa_t){A, B, C, N, inicio, inicio + qtd};
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
    printf("[Matmul Pthreads] N: %d | Threads: %2d | Tempo: %.4f s | GFLOPS: %.2f | Verificação: %s\n",
           N, T, t, gflops(N, t), ok ? "OK" : "FALHOU");

    free(threads);
    free(tarefas);
    free(A);
    free(B);
    free(C);
    return ok ? 0 : 1;
}
