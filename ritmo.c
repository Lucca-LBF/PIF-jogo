#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"
#include "notas.h"
#include "ritmo.h"

static char cabecalho[128] = "";

void ritmo_definir_cabecalho(const char *texto) {
    if (texto == NULL) texto = "";
    strncpy(cabecalho, texto, sizeof(cabecalho) - 1);
    cabecalho[sizeof(cabecalho) - 1] = '\0';
}

void efeito_digitacao(const char *texto, int velocidade_ms) {
    int i = 0;
    while (texto[i] != '\0') {
        unsigned char c = (unsigned char) texto[i];
        int tamanho = 1;
        if      (c >= 0xF0) tamanho = 4;
        else if (c >= 0xE0) tamanho = 3;
        else if (c >= 0xC0) tamanho = 2;

        for (int k = 0; k < tamanho && texto[i] != '\0'; ++k, ++i) {
            putchar(texto[i]);
        }
        fflush(stdout);
        PAUSAR_MS(velocidade_ms);
    }
}

static int frequencia_da_tecla(char tecla) {
    if (tecla == 'S') return 329;
    if (tecla == 'J') return 392;
    if (tecla == 'K') return 523;
    return 261;
}

void tocar_nota_musical(char tecla) {
    tocar_tom(frequencia_da_tecla(tecla), 150);
}

static void desenhar_tela(int passo_atual, const char *ouca, const char *repita) {
    LIMPAR_TELA();

    if (cabecalho[0] != '\0') printf("%s\n\n", cabecalho);

    printf("Veja...\t");
    for (int i = 0; i < PASSOS_POR_COMPASSO; ++i) {
        if (i == passo_atual) printf("O");
        else if (i % 2 == 0)  printf("x");
        else                  printf("-");
    }
    printf("\n");

    printf("Ouça...\t");
    printf("%s\n", ouca);

    printf("Repita...\t");
    printf("%s\n", repita);

    printf("====================================\n");
    fflush(stdout);
}

static void linha_vazia(char *linha) {
    memset(linha, '-', PASSOS_POR_COMPASSO);
    linha[PASSOS_POR_COMPASSO] = '\0';
}

void animar_sequencia(Nota *gabarito, int tempo_passo_ms) {
    Nota *atual = gabarito;
    char ouca[PASSOS_POR_COMPASSO + 1];
    char repita[PASSOS_POR_COMPASSO + 1];

    linha_vazia(ouca);
    linha_vazia(repita);

    LIMPAR_TELA();
    if (cabecalho[0] != '\0') printf("%s\n\n", cabecalho);
    efeito_digitacao("No ritmo... 3, 2, 1...", 40);
    PAUSAR_MS(1000);

    for (int passo = 0; passo < PASSOS_POR_COMPASSO; ++passo) {
        char tecla_da_vez = '\0';
        if (atual != NULL && passo % 2 == 0) {
            tecla_da_vez = atual->tecla;
            atual = atual->prox;
        }

        linha_vazia(ouca);
        if (tecla_da_vez != '\0') ouca[passo] = tecla_da_vez;

        desenhar_tela(passo, ouca, repita);

        if (tecla_da_vez != '\0')  tocar_nota_musical(tecla_da_vez);
        else if (passo % 2 == 0)   tocar_tom(1000, 30);

        PAUSAR_MS(tempo_passo_ms);
    }
}

static int tecla_valida(int c) {
    return c == 'A' || c == 'S' || c == 'J' || c == 'K';
}

int turno_jogador(Nota *gabarito, int tempo_passo_ms) {
    char esperado[MAX_NOTAS_POR_RODADA];
    char resposta[MAX_NOTAS_POR_RODADA];
    char ouca[PASSOS_POR_COMPASSO + 1];
    char repita[PASSOS_POR_COMPASSO + 1];
    char linha_gabarito[PASSOS_POR_COMPASSO + 1];
    int total = 0;
    int acertos = 0;

    memset(esperado, 0, sizeof(esperado));
    memset(resposta, 0, sizeof(resposta));
    for (Nota *n = gabarito; n != NULL && total < MAX_NOTAS_POR_RODADA; n = n->prox) {
        esperado[total++] = n->tecla;
    }

    linha_vazia(ouca);
    linha_vazia(repita);

    desenhar_tela(-1, ouca, repita);
    efeito_digitacao("Sua vez! (teclas A S J K)", 30);
    PAUSAR_MS(1000);
    limpar_buffer_teclado();

    long long inicio = tempo_ms();
    int passo_exibido = -2;
    int redesenhar = 1;

    for (;;) {
        long long t = tempo_ms() - inicio;
        if (t >= (long long) PASSOS_POR_COMPASSO * tempo_passo_ms) break;

        int passo = (int) (t / tempo_passo_ms);
        if (passo != passo_exibido) {
            passo_exibido = passo;
            redesenhar = 1;
        }

        int c;
        while ((c = tecla_lida()) >= 0) {
            if (!tecla_valida(c)) continue;

            long long quando = tempo_ms() - inicio;
            int nota = (int) ((quando + tempo_passo_ms) / (2LL * tempo_passo_ms));
            if (nota >= 0 && nota < MAX_NOTAS_POR_RODADA && resposta[nota] == 0) {
                resposta[nota] = (char) c;
                repita[nota * 2] = (char) c;
                tocar_nota_musical((char) c);
                redesenhar = 1;
            }
        }

        if (redesenhar) {
            desenhar_tela(passo_exibido, ouca, repita);
            redesenhar = 0;
        }
        PAUSAR_MS(5);
    }

    for (int k = 0; k < total; ++k) {
        if (resposta[k] == esperado[k]) acertos++;
    }

    linha_vazia(linha_gabarito);
    for (int k = 0; k < total; ++k) linha_gabarito[k * 2] = esperado[k];

    desenhar_tela(-1, ouca, repita);
    printf("\nGabarito...\t%s\n", linha_gabarito);
    printf("Você......\t%s\n\n", repita);

    if (acertos == total)  printf("PERFEITO!  (%d/%d)\n", acertos, total);
    else if (acertos > 0)  printf("Quase!  (%d/%d)\n", acertos, total);
    else                   printf("Errou...  (%d/%d)\n", acertos, total);
    fflush(stdout);

    PAUSAR_MS(2000);
    return acertos;
}