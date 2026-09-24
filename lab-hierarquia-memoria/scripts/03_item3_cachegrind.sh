#!/usr/bin/env bash
# Item 3: profiling de cache com valgrind/cachegrind.
# Roda padrão e blocado, -O0 e -O3, para N = 512, 1024, 1536.
# O LL simulado é o L3 real da máquina (32 MB, 16-way, linha de 64 B), porque a
# detecção automática do valgrind no Codespace lê um L3 errado (320 MB, 1-way).
# O script CONTINUA de onde parou: execuções já concluídas são puladas.
# Para rodar só alguns N:  ./scripts/03_item3_cachegrind.sh 512 1024
tabela() { if command -v column >/dev/null; then column -t -s, "$1"; else tr "," "\t" < "$1"; fi; }
BS=64
LL="33554432,16,64"
LISTA_N=${*:-"512 1024 1536"}
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
      if [ -f "$ARQ" ] && grep -q "LL miss rate" "$ARQ" && grep -q "^$V,$OPT,$N," "$CSV"; then
        echo ">> Já concluído, pulando: matmul_${V}_${OPT} $ARGS"
        continue
      fi
      echo "############################################################"
      echo ">> valgrind --tool=cachegrind --cache-sim=yes --LL=$LL ./bin/matmul_${V}_${OPT} $ARGS"
      echo ">> Início: $(date +%H:%M:%S)"
      valgrind --tool=cachegrind --cache-sim=yes --LL=$LL --cachegrind-out-file=/dev/null \
        ./bin/matmul_${V}_${OPT} $ARGS > "$ARQ" 2>&1
      cat "$ARQ"
      T=$(grep "Tempo:" "$ARQ" | awk -F'Tempo: ' '{print $2}' | awk '{print $1}')
      echo "$V,$OPT,$N,$T,$(extrai $ARQ 'D1  misses'),$(extrai $ARQ 'D1  miss rate'),$(extrai $ARQ 'LLd misses'),$(extrai $ARQ 'LLd miss rate'),$(extrai $ARQ 'I *refs'),$(extrai $ARQ 'D *refs')" >> "$CSV"
    done
  done
done
echo
echo "================ Tabela 2 (métricas de cache) ================"
tabela "$CSV" | tee resultados/03_item3_cachegrind_tabela.txt
