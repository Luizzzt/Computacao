#!/usr/bin/env bash
# Roda todas as etapas rápidas (tudo menos o cachegrind).
set -e
make
./scripts/00_hardware.sh
./scripts/01_item2.sh
./scripts/02_item3_tempos.sh
./scripts/04_item4_threads.sh
echo
echo ">> Pronto. Agora rode o cachegrind: ./scripts/03_item3_cachegrind.sh"
