#!/usr/bin/env bash
# Item 4: escalabilidade com Pthreads (1, 2, 4, 8, 16 threads).
# Uso: ./scripts/04_item4_threads.sh [N] [BS]   (padrão N=1536, BS=64)
N=${1:-1536}
BS=${2:-64}
tabela() { if command -v column >/dev/null; then column -t -s, "$1"; else tr "," "\t" < "$1"; fi; }
mkdir -p resultados
CSV=resultados/04_item4_threads.csv
TXT=resultados/04_item4_threads.txt
echo "versao,N,threads,tempo,gflops,speedup,eficiencia" > "$CSV"
{
  echo "Núcleos disponíveis nesta máquina (nproc): $(nproc)"
  echo
  for V in pthreads pthreads_bloco; do
    echo "================ matmul_$V (-O3) ================"
    T1=""
    for T in 1 2 4 8 16; do
      if [ "$V" = "pthreads" ]; then
        R=$(./bin/matmul_pthreads_O3 $N $T)
      else
        R=$(./bin/matmul_pthreads_bloco_O3 $N $T $BS)
      fi
      echo "$R"
      TP=$(echo "$R" | awk -F'Tempo: ' '{print $2}' | awk '{print $1}')
      G=$(echo "$R" | awk -F'GFLOPS: ' '{print $2}' | awk '{print $1}')
      [ -z "$T1" ] && T1=$TP
      SP=$(awk -v a="$T1" -v b="$TP" 'BEGIN{printf "%.2f", a/b}')
      EF=$(awk -v s="$SP" -v p="$T" 'BEGIN{printf "%.2f", s/p}')
      echo "$V,$N,$T,$TP,$G,$SP,$EF" >> "$CSV"
    done
    echo
  done
  echo "================ Tabela 3 ================"
  tabela "$CSV"
} 2>&1 | tee "$TXT"
