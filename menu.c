#include <stdio.h>
#include "sistema.h"
#include "ritmo.h"
#include "pontuacao.h"
#include "menu.h"

static void desenhar_menu(void) {
    LIMPAR_TELA();
    printf("====================================\n");
    printf("               PONGUE\n");
    printf("        Jogo de ritmo  \n");
    printf("====================================\n\n");
    printf("   [1] Jogar\n");
    printf("   [2] Como jogar\n");
    printf("   [3] Sair\n\n");
    printf("====================================\n");
    fflush(stdout);
}

int menu_principal(void) {
    desenhar_menu();
    efeito_digitacao("Escolha uma opção (1-3): ", 25);
    limpar_buffer_teclado();

    for (;;) {
        int c = ler_tecla_bloqueante();
        if (c < 0) return MENU_SAIR;

        if (c >= '1' && c <= '3') {
            tocar_tom(523, 80);
            return c - '0';
        }
    }
}

void menu_como_jogar(void) {
    LIMPAR_TELA();
    printf("============ COMO JOGAR ============\n\n");
    printf("   1. O computador toca uma sequência de notas.\n");
    printf("      Olhe a linha \"Ouça...\" e memorize.\n\n");
    printf("   2. Na sua vez, repita no MESMO RITMO\n");
    printf("      usando as teclas  A  S  J  K\n\n");
    printf("   3. Cada nota é julgada pelo tempo:\n");
    printf("      PERFEITO ... %3d pts  (apertou na hora)\n", PONTOS_PERFEITO);
    printf("      OK ......... %3d pts  (um pouco antes)\n", PONTOS_OK);
    printf("      ATRASADO ... %3d pts  (um pouco depois)\n", PONTOS_ATRASADO);
    printf("      ERROU ......   0 pts  e perde %d de vida\n\n", DANO_ERRO);
    printf("   4. PERFEITO e OK seguidos formam combo e dão bônus.\n");
    printf("      Acabou a vida? GAME OVER.\n\n");
    printf("====================================\n");
    printf("Aperte qualquer tecla para voltar...\n");
    fflush(stdout);
    limpar_buffer_teclado();
    ler_tecla_bloqueante();
}