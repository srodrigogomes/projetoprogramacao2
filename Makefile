# Detecta o sistema operacional
ifdef OS
  OS := $(strip $(OS))
else
  OS := $(strip $(shell uname))
endif

BINNAME = album_copa2026

ifeq ($(OS),Windows_NT)
	INCLUDE = -I./include/ -L./libwin
	EXTRA_FLAGS = -Wall -Werror -Wextra -std=c99 -Wno-missing-braces -lraylib -lm -lopengl32 -lgdi32 -lwinmm -lmsvcrt
	BIN = $(BINNAME).exe
	RM = del /Q /F
else
	INCLUDE = -I./include/ -L./lib
	EXTRA_FLAGS = -Wall -Werror -Wextra -std=c99 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lasound
	BIN = ./$(BINNAME)
	RM = rm -f
endif

CC = gcc

SRC = src/main.c \
      src/arquivo.c \
      src/figurinha.c \
      src/album.c \
      src/pacote.c \
      src/minigame.c \
      src/estatisticas.c \
      src/telas.c \
      src/loja.c \
      src/troca.c

all:
	@echo "Compilando para $(OS)..."
	$(CC) $(SRC) -o $(BIN) $(EXTRA_FLAGS) $(INCLUDE)

run: all
	$(BIN)

clean:
	$(RM) $(BIN)

.PHONY: all run clean
