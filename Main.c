#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sistema.h"
#include "notas.h"
#include "pontuacao.h"
#include "ritmo.h"
#include "menu.h"

#define TOTAL_RODADAS 10

static int tamanho_da_rodada(int rodada) {
    int n = 2 + (rodada - 1) / 2;
    return n > MAX_NOTAS_POR_RODADA ? MAX_NOTAS_POR_RODADA : n;
}

static int passo_da_rodada(int rodada) {
    int ms = PASSO_PADRAO_MS - (rodada - 1) * 5;
    return ms < PASSO_MINIMO_MS ? PASSO_MINIMO_MS : ms;
}

static void jogar_partida(void) {
    Placar placar;

    placar_iniciar(&placar);
    ritmo_reiniciar();

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
}

int main(void) {
    srand((unsigned) time(NULL));
    iniciar_sistema();

    for (;;) {
        int opcao = menu_principal();
        if (opcao == MENU_SAIR) break;
        if (opcao == MENU_COMO_JOGAR) menu_como_jogar();
        if (opcao == MENU_TECLAS) menu_teclas();
        if (opcao == MENU_JOGAR) jogar_partida();
    }

    LIMPAR_TELA();
    return 0;
}