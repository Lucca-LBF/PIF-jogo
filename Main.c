#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sistema.h"
#include "notas.h"
#include "pontuacao.h"
#include "ritmo.h"

#define TOTAL_RODADAS 10

static int tamanho_da_rodada(int rodada) {
    int n = 2 + (rodada - 1) / 2;
    return n > MAX_NOTAS_POR_RODADA ? MAX_NOTAS_POR_RODADA : n;
}

static int passo_da_rodada(int rodada) {
    int ms = PASSO_PADRAO_MS - (rodada - 1) * 5;
    return ms < PASSO_MINIMO_MS ? PASSO_MINIMO_MS : ms;
}

int main(void) {
    Placar placar;

    srand((unsigned) time(NULL));
    iniciar_sistema();
    placar_iniciar(&placar);

    for (int rodada = 1; rodada <= TOTAL_RODADAS; ++rodada) {
        char texto[96];
        int tamanho = tamanho_da_rodada(rodada);
        int passo_ms = passo_da_rodada(rodada);
        Nota *gabarito = gerar_sequencia_aleatoria(tamanho);

        snprintf(texto, sizeof(texto), "Rodada %d/%d   |   Pontos: %d   |   Combo: %dx",
                 rodada, TOTAL_RODADAS, placar.pontos, placar.combo);
        ritmo_definir_cabecalho(texto);

        animar_sequencia(gabarito, passo_ms);
        turno_jogador(gabarito, passo_ms, &placar);

        liberar_sequencia(gabarito);
    }

    pontuacao_tela_final(&placar);
    limpar_buffer_teclado();
    ler_tecla_bloqueante();
    return 0;
}