#!/usr/bin/env bash
# Coleta as informações de hardware e software pedidas na Seção 2 do relatório.
mkdir -p resultados
OUT=resultados/00_hardware.txt
{
  echo "===== CPU (lscpu) ====="
  lscpu
  echo
  echo "===== Topologia de caches (lscpu -C) ====="
  lscpu -C 2>/dev/null || echo "lscpu -C não disponível"
  echo
  echo "===== Caches detalhados (/sys) ====="
  for d in /sys/devices/system/cpu/cpu0/cache/index*; do
    echo "L$(cat $d/level) $(cat $d/type): tamanho=$(cat $d/size) | assoc=$(cat $d/ways_of_associativity)-way | linha=$(cat $d/coherency_line_size) bytes"
  done
  echo
  echo "===== Núcleos disponíveis (nproc) ====="
  nproc
  echo
  echo "===== Memória RAM (free -h) ====="
  free -h
  echo
  echo "===== Sistema operacional ====="
  cat /etc/os-release | head -4
  uname -a
  echo
  echo "===== GCC ====="
  gcc --version | head -1
  echo
  echo "===== Valgrind ====="
  valgrind --version 2>/dev/null || echo "valgrind não instalado"
} 2>&1 | tee "$OUT"
echo
echo ">> Salvo em $OUT"
