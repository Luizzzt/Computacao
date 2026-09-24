# Makefile - Lab Hierarquia de Memória, Blocagem e Pthreads
# Gera duas versões de cada programa: _O0 (sem otimização) e _O3 (otimização máxima)

CC      = gcc
CFLAGS  = -Wall -Wextra -std=gnu11
LDLIBS  = -lpthread
SRC     = src
BIN     = bin

PROGS   = varredura_linha varredura_coluna matmul_padrao matmul_bloco \
          matmul_pthreads matmul_pthreads_bloco

ALVOS   = $(foreach p,$(PROGS),$(BIN)/$(p)_O0 $(BIN)/$(p)_O3)

all: $(ALVOS)

$(BIN):
	mkdir -p $(BIN)

$(BIN)/%_O0: $(SRC)/%.c $(SRC)/common.h | $(BIN)
	$(CC) -O0 $(CFLAGS) $< -o $@ $(LDLIBS)

$(BIN)/%_O3: $(SRC)/%.c $(SRC)/common.h | $(BIN)
	$(CC) -O3 $(CFLAGS) $< -o $@ $(LDLIBS)

clean:
	rm -rf $(BIN) cachegrind.out.*

.PHONY: all clean
