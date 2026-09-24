#!/usr/bin/env bash
# Item 2: varredura por linha x por coluna para N = 512 ... 8192.
# Cada programa roda 3 vezes internamente e mostra o melhor tempo.
tabela() { if command -v column >/dev/null; then column -t -s, "$1"; else tr "," "\t" < "$1"; fi; }
mkdir -p resultados
CSV=resultados/01_item2.csv
TXT=resultados/01_item2.txt
echo "otimizacao,N,pares,t_linha,t_coluna,slowdown" > "$CSV"
{
  for OPT in O0 O3; do
    echo "================ Compilação -$OPT ================"
    for N in 512 1024 2048 4096 8192; do
      L=$(./bin/varredura_linha_$OPT $N)
      C=$(./bin/varredura_coluna_$OPT $N)
      echo "$L"
      echo "$C"
      TL=$(echo "$L" | awk -F'Tempo: ' '{print $2}' | awk '{print $1}')
      TC=$(echo "$C" | awk -F'Tempo: ' '{print $2}' | awk '{print $1}')
      P=$(echo "$L" | awk -F'Pares: ' '{print $2}' | awk '{print $1}')
      S=$(awk -v a="$TC" -v b="$TL" 'BEGIN{printf "%.2f", a/b}')
      echo "   -> Slowdown (T_coluna / T_linha) = ${S}x"
      echo "$OPT,$N,$P,$TL,$TC,$S" >> "$CSV"
    done
    echo
  done
  echo "================ Tabela 1 ================"
  tabela "$CSV"
} 2>&1 | tee "$TXT"
