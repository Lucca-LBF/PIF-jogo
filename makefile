CC     = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
FONTES = Main.c notas.c ritmo.c pontuacao.c menu.c sistema.c

ifeq ($(OS),Windows_NT)
    ALVO = jogo.exe
else
    ALVO = jogo
endif

$(ALVO): $(FONTES) notas.h ritmo.h pontuacao.h menu.h sistema.h
	$(CC) $(CFLAGS) -o $(ALVO) $(FONTES)

run: $(ALVO)
	./$(ALVO)

clean:
	rm -f jogo jogo.exe

.PHONY:	run	clean