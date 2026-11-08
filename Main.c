#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sistema.h"
#include "notas.h"
#include "ritmo.h"

#define TOTAL_RODADAS 10

static int tamanho_da_rodada(int rodada) {
    int n = 2 + (rodada - 1) / 2;
    return n > MAX_NOTAS_POR_RODADA ? MAX_NOTAS_POR_RODADA : n;
}

static int passo_da_rodada(int rodada) {
    int ms = PASSO_PADRAO_MS - (rodada - 1) * 8;
    return ms < 180 ? 180 : ms;
}

int main(void) {
    srand((unsigned) time(NULL));
    iniciar_sistema();

    for (int rodada = 1; rodada <= TOTAL_RODADAS; ++rodada) {
        char texto[64];
        int tamanho = tamanho_da_rodada(rodada);
        int passo_ms = passo_da_rodada(rodada);
        Nota *gabarito = gerar_sequencia_aleatoria(tamanho);

        snprintf(texto, sizeof(texto), "Rodada %d/%d", rodada, TOTAL_RODADAS);
        ritmo_definir_cabecalho(texto);

        animar_sequencia(gabarito, passo_ms);
        turno_jogador(gabarito, passo_ms);

        liberar_sequencia(gabarito);
    }

    LIMPAR_TELA();
    printf("Fim das rodadas!\n");
    printf("Aperte qualquer tecla para sair...\n");
    fflush(stdout);
    limpar_buffer_teclado();
    ler_tecla_bloqueante();
    return 0;
}
