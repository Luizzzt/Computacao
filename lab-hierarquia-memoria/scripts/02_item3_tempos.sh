#!/usr/bin/env bash
# Item 3: tempos da versão padrão x blocada, -O0 x -O3, N = 512, 1024, 1536.
# Também testa vários tamanhos de bloco (ajuste do B).
BS=${1:-64}
tabela() { if command -v column >/dev/null; then column -t -s, "$1"; else tr "," "\t" < "$1"; fi; }
mkdir -p resultados
CSV=resultados/02_item3_tempos.csv
TXT=resultados/02_item3_tempos.txt
echo "versao,otimizacao,N,bloco,tempo,gflops" > "$CSV"
{
  for OPT in O0 O3; do
    echo "================ Compilação -$OPT ================"
    for N in 512 1024 1536; do
      P=$(./bin/matmul_padrao_$OPT $N);      echo "$P"
      B=$(./bin/matmul_bloco_$OPT $N $BS);   echo "$B"
      TP=$(echo "$P" | awk -F'Tempo: ' '{print $2}' | awk '{print $1}')
      GP=$(echo "$P" | awk -F'GFLOPS: ' '{print $2}' | awk '{print $1}')
      TB=$(echo "$B" | awk -F'Tempo: ' '{print $2}' | awk '{print $1}')
      GB=$(echo "$B" | awk -F'GFLOPS: ' '{print $2}' | awk '{print $1}')
      echo "padrao,$OPT,$N,-,$TP,$GP" >> "$CSV"
      echo "bloco,$OPT,$N,$BS,$TB,$GB" >> "$CSV"
    done
    echo
  done

  echo "================ Ajuste do tamanho do bloco (-O3, N=1024) ================"
  CSVB=resultados/02_item3_ajuste_bloco.csv
  echo "N,bloco,tempo,gflops" > "$CSVB"
  for B in 8 16 32 64 128 256 512; do
    R=$(./bin/matmul_bloco_O3 1024 $B); echo "$R"
    T=$(echo "$R" | awk -F'Tempo: ' '{print $2}' | awk '{print $1}')
    G=$(echo "$R" | awk -F'GFLOPS: ' '{print $2}' | awk '{print $1}')
    echo "1024,$B,$T,$G" >> "$CSVB"
  done
  echo
  echo "================ Resumo ================"
  tabela "$CSV"
} 2>&1 | tee "$TXT"
