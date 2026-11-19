#include <stdio.h>
#include <string.h>
#include "sistema.h"
#include "notas.h"
#include "ritmo.h"
#include "pontuacao.h"
#include "menu.h"


static const char *titulo[] = {
    " ____   ___  _   _  ____ _   _ _____ ",
    "|  _ \\ / _ \\| \\ | |/ ___| | | | ____|",
    "| |_) | | | |  \\| | |  _| | | |  _|  ",
    "|  __/| |_| | |\\  | |_| | |_| | |___ ",
    "|_|    \\___/|_| \\_|\\____|\\___/|_____|"
};

static const char *cena_a[] = {
    " .    *     .        *       .     *    . ",
    "   __________________________________     ",
    "  |  *       O            .      *   |    ",
    "  |      ( )      ( )      ( )       |  * ",
    "  |  .                          .    |    ",
    "  |   \\___                   ___/    |    ",
    "  |       \\_______   _______/        |    ",
    "  |_______________| |________________|    "
};

static const char *cena_b[] = {
    " .    +     .        +       .     +    . ",
    "   __________________________________     ",
    "  |  +                    .      +   |    ",
    "  |      ( )      ( )  O   ( )       |  + ",
    "  |  .                          .    |    ",
    "  |   \\___                   ___/    |    ",
    "  |       \\_______   _______/        |    ",
    "  |_______________| |________________|    "
};

#define LINHAS_TITULO (int) (sizeof(titulo) / sizeof(titulo[0]))
#define LINHAS_CENA   (int) (sizeof(cena_a) / sizeof(cena_a[0]))

static void centralizar_bloco(const char *linhas[], int n) {
    int maior = 0;
    for (int i = 0; i < n; ++i) {
        int len = (int) strlen(linhas[i]);
        if (len > maior) maior = len;
    }
    int espacos = (largura_tela() - maior) / 2;
    if (espacos < 0) espacos = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < espacos; ++j) putchar(' ');
        printf("%s\n", linhas[i]);
    }
}

static void desenhar_menu(int quadro) {
    LIMPAR_TELA();
    printf("\n");
    centralizar_bloco(titulo, LINHAS_TITULO);
    printf("\n");
    centralizar_bloco(quadro == 0 ? cena_a : cena_b, LINHAS_CENA);
    printf("\n");
    ritmo_centralizar("[1] START    [2] COMO JOGAR    [3] TECLAS    [4] CLOSE");
    fflush(stdout);
}

int menu_principal(void) {
    int quadro = 0;
    long long proxima_troca = 0;

    limpar_buffer_teclado();
    for (;;) {
        
        if (tempo_ms() >= proxima_troca) {
            desenhar_menu(quadro);
            quadro = 1 - quadro;
            proxima_troca = tempo_ms() + 500;
        }

        int c = tecla_lida();
        if (c == -2) return MENU_SAIR;
        if (c >= '1' && c <= '4') {
            tocar_tom(523, 80);
            return c - '0';
        }
        PAUSAR_MS(10);
    }
}

void menu_como_jogar(void) {
    char linha[128];

    LIMPAR_TELA();
    printf("\n");
    ritmo_centralizar("============ COMO JOGAR ============");
    printf("\n");
    ritmo_centralizar("1. O computador toca uma sequencia de notas na linha OUCA.");
    ritmo_centralizar("Memorize as teclas e o ritmo.");
    printf("\n");
    snprintf(linha, sizeof(linha), "2. Na linha REPITA, toque igual usando  %c  %c  %c  %c",
             teclas[0], teclas[1], teclas[2], teclas[3]);
    ritmo_centralizar(linha);
    printf("\n");
    snprintf(linha, sizeof(linha), "3. Na batida certa: PERFEITO (%d pts). Quase: BOM (%d pts).",
             PONTOS_PERFEITO, PONTOS_BOM);
    ritmo_centralizar(linha);
    ritmo_centralizar("Acertos seguidos formam combo e dao bonus.");
    printf("\n");
    ritmo_centralizar("====================================");
    ritmo_centralizar("Aperte qualquer tecla para voltar...");
    fflush(stdout);
    limpar_buffer_teclado();
    ler_tecla_bloqueante();
}

static void desenhar_teclas(const char *aviso) {
    char linha[128];

    LIMPAR_TELA();
    printf("\n");
    ritmo_centralizar("============== TECLAS ==============");
    printf("\n");
    snprintf(linha, sizeof(linha), "Teclas atuais:   [ %c ]  [ %c ]  [ %c ]  [ %c ]",
             teclas[0], teclas[1], teclas[2], teclas[3]);
    ritmo_centralizar(linha);
    printf("\n");
    ritmo_centralizar("[1] Trocar teclas    [2] Restaurar D F J K    [3] Voltar");
    printf("\n");
    if (aviso != NULL && aviso[0] != '\0') ritmo_centralizar(aviso);
    fflush(stdout);
}

static int ja_escolhida(const char *novas, int quantas, int c) {
    for (int i = 0; i < quantas; ++i) {
        if (novas[i] == c) return 1;
    }
    return 0;
}

static void trocar_teclas(void) {
    char novas[NUM_TECLAS];
    char aviso[96];

    for (int i = 0; i < NUM_TECLAS; ) {
        snprintf(aviso, sizeof(aviso), "Aperte a tecla %d de %d (letras de A a Z)", i + 1, NUM_TECLAS);
        desenhar_teclas(aviso);

        int c = ler_tecla_bloqueante();
        if (c < 0) return; // entrada acabou 
        if (c < 'A' || c > 'Z') continue; // só letras
        if (ja_escolhida(novas, i, c)) continue; // sem repetir

        novas[i] = (char) c;
        tocar_tom(523, 60);
        i++;
    }

    for (int i = 0; i < NUM_TECLAS; ++i) teclas[i] = novas[i];
}

void menu_teclas(void) {
    const char padrao[NUM_TECLAS] = {'D', 'F', 'J', 'K'};
    const char *aviso = "";

    limpar_buffer_teclado();
    for (;;) {
        desenhar_teclas(aviso);
        int c = ler_tecla_bloqueante();
        if (c < 0 || c == '3') return;
        if (c == '1') {
            trocar_teclas();
            aviso = "Teclas trocadas!";
        } else if (c == '2') {
            for (int i = 0; i < NUM_TECLAS; ++i) teclas[i] = padrao[i];
            aviso = "Teclas restauradas para D F J K.";
        }
    }
}
