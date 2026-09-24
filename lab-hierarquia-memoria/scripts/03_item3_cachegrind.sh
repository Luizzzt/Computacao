#!/usr/bin/env bash
# Item 3: profiling de cache com valgrind/cachegrind.
# Roda padrão e blocado, -O0 e -O3, para N = 512, 1024, 1536.
# ATENÇÃO: o cachegrind deixa o programa ~50x mais lento. Pode levar uns 30 a 40 minutos.
# Para rodar só alguns N:  ./scripts/03_item3_cachegrind.sh 512 1024
BS=64
LISTA_N=${*:-"512 1024 1536"}
tabela() { if command -v column >/dev/null; then column -t -s, "$1"; else tr "," "\t" < "$1"; fi; }
mkdir -p resultados/cachegrind
CSV=resultados/03_item3_cachegrind.csv
[ -f "$CSV" ] || echo "versao,otimizacao,N,tempo_sob_valgrind,D1_misses,D1_miss_rate,LLd_misses,LLd_miss_rate,I_refs,D_refs" > "$CSV"

extrai() {  # $1 = arquivo, $2 = rótulo da linha do cachegrind
  grep "$2" "$1" | head -1 | sed 's/.*: *//' | awk '{print $1}' | tr -d ','
}

for N in $LISTA_N; do
  for OPT in O0 O3; do
    for V in padrao bloco; do
      if [ "$V" = "bloco" ]; then ARGS="$N $BS"; else ARGS="$N"; fi
      ARQ=resultados/cachegrind/${V}_${OPT}_N${N}.txt
      echo "############################################################"
      echo ">> valgrind --tool=cachegrind --cache-sim=yes ./bin/matmul_${V}_${OPT} $ARGS"
      echo ">> Início: $(date +%H:%M:%S)"
      valgrind --tool=cachegrind --cache-sim=yes --cachegrind-out-file=/dev/null \
        ./bin/matmul_${V}_${OPT} $ARGS 2>&1 | tee "$ARQ"
      T=$(grep "Tempo:" "$ARQ" | awk -F'Tempo: ' '{print $2}' | awk '{print $1}')
      echo "$V,$OPT,$N,$T,$(extrai $ARQ 'D1  misses'),$(extrai $ARQ 'D1  miss rate'),$(extrai $ARQ 'LLd misses'),$(extrai $ARQ 'LLd miss rate'),$(extrai $ARQ 'I *refs'),$(extrai $ARQ 'D *refs')" >> "$CSV"
    done
  done
done
echo
echo "================ Tabela 2 (métricas de cache) ================"
tabela "$CSV" | tee resultados/03_item3_cachegrind_tabela.txt
