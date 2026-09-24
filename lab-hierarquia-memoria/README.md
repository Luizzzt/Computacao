# Lab: Hierarquia de Memória, Blocagem (Tiling) e Pthreads

Computação Paralela - Faculdade de Computação e Informática (FCI)

## Estrutura

```
Makefile                      compila tudo em duas versões: _O0 e _O3 (pasta bin/)
src/common.h                  tempo, alocação, inicialização e verificação do resultado
src/varredura_linha.c         Item 2 - varredura por linha
src/varredura_coluna.c        Item 2 - varredura por coluna
src/matmul_padrao.c           Item 3 - multiplicação canônica (i, j, k)
src/matmul_bloco.c            Item 3 - multiplicação com blocagem (i, k, j dentro do bloco)
src/matmul_pthreads.c         Item 4 - canônica paralela (faixas de linhas por thread)
src/matmul_pthreads_bloco.c   Item 4 - Pthreads + blocagem
scripts/00_hardware.sh        coleta CPU, caches, RAM, SO, GCC, Valgrind
scripts/01_item2.sh           Item 2 - N = 512 a 8192, tempos e slowdown
scripts/02_item3_tempos.sh    Item 3 - padrão x bloco, -O0 x -O3, N = 512, 1024, 1536 + ajuste do bloco
scripts/03_item3_cachegrind.sh Item 3 - valgrind/cachegrind (D1 e LLd miss rate)
scripts/04_item4_threads.sh   Item 4 - 1, 2, 4, 8 e 16 threads (speedup e eficiência)
resultados/                   saídas geradas pelos scripts (.txt e .csv)
```

## Como compilar

```bash
make            # gera bin/<programa>_O0 e bin/<programa>_O3
make clean      # apaga bin/
```

Flags: `gcc -O0 -Wall -Wextra -std=gnu11` e `gcc -O3 -Wall -Wextra -std=gnu11`, com `-lpthread`.

## Como executar

```bash
./bin/varredura_linha_O3 4096
./bin/varredura_coluna_O3 4096
./bin/matmul_padrao_O3 1024
./bin/matmul_bloco_O3 1024 64               # N  tamanho_do_bloco
./bin/matmul_pthreads_O3 1536 4             # N  threads
./bin/matmul_pthreads_bloco_O3 1536 4 64    # N  threads  bloco
```

Todo programa de multiplicação confere a matriz C inteira com a fórmula exata
`C[i][j] = j * (i * S1 + S2)`, onde `S1 = soma de k` e `S2 = soma de k²` (k = 0..N-1),
e imprime `Verificação: OK` quando o resultado está correto.

## Rodando os experimentos

```bash
sudo apt-get update && sudo apt-get install -y valgrind
chmod +x scripts/*.sh
make
./scripts/00_hardware.sh
./scripts/01_item2.sh
./scripts/02_item3_tempos.sh
./scripts/04_item4_threads.sh
./scripts/03_item3_cachegrind.sh     # demorado (30 a 40 min)
```
