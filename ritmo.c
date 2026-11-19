#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"
#include "notas.h"
#include "pontuacao.h"
#include "ritmo.h"

static char cabecalho[128] = "";

// Qual linha tem a seta ">": 0 = TEMPO, 1 = OUÇA, 2 = REPITA
static int linha_ativa = 0;

static char resultado[96] = "";
static char ultimo_gabarito[PASSOS_POR_COMPASSO + 1] = "";
static char ultima_resposta[PASSOS_POR_COMPASSO + 1] = "";

static int largura_visual_utf8(const char *texto) {
    int largura = 0;
    for (int i = 0; texto[i] != '\0'; ++i) {
        if (((unsigned char) texto[i] & 0xC0) != 0x80) {
            largura++;
        }
    }
    return largura;
}

void ritmo_centralizar(const char *texto) {
    int colunas = largura_tela();
    int len = largura_visual_utf8(texto);
    int espacos = (colunas - len) / 2;
    if (espacos < 0) espacos = 0;

    for (int i = 0; i < espacos; ++i) putchar(' ');
    printf("%s\n", texto);
}

void ritmo_reiniciar(void) {
    resultado[0] = '\0';
}

void ritmo_definir_cabecalho(const char *texto) {
    if (texto == NULL) texto = "";
    strncpy(cabecalho, texto, sizeof(cabecalho) - 1);
    cabecalho[sizeof(cabecalho) - 1] = '\0';
}

void efeito_digitacao(const char *texto, int velocidade_ms) {
    int colunas = largura_tela();
    int len = largura_visual_utf8(texto);
    int espacos = (colunas - len) / 2;
    if (espacos < 0) espacos = 0;

    for (int i = 0; i < espacos; ++i) putchar(' ');

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
    putchar('\n');
}

static int frequencia_da_tecla(char tecla) {
    int frequencias[NUM_TECLAS] = {261, 329, 392, 523};
    for (int i=0; i<NUM_TECLAS; ++i){
        if ( tecla==teclas[i] ) return frequencias[i];
    };
    return 261;
}

void tocar_nota_musical(char tecla) {
    tocar_tom(frequencia_da_tecla(tecla), 150);
}

static void formatar_faixa_grande(char *destino, size_t tam, const char *titulo, const char *conteudo) {
    char faixa[128] = "";
    size_t pos = 0;

    for (int i = 0; i < PASSOS_POR_COMPASSO; ++i) {
        char c = conteudo[i];
        int escrito = 0;
        if (i % 2 == 0) {
            escrito = snprintf(faixa + pos, sizeof(faixa) - pos, "[ %c ] ", c == '-' ? ' ' : c);
        } else if (c == '*') {
            escrito = snprintf(faixa + pos, sizeof(faixa) - pos, " °  ");
        } else {
            escrito = snprintf(faixa + pos, sizeof(faixa) - pos, " %c  ", c == '-' ? '.' : c);
        }
        if (escrito > 0) pos += (size_t) escrito;
    }

    snprintf(destino, tam, "%-10s %s", titulo, faixa);
}

static void desenhar_tela(int passo_atual, const char *ouca, const char *repita, const char *aviso) {
    char linha[256];
    char cursor[PASSOS_POR_COMPASSO + 1];

    LIMPAR_TELA();
    printf("\n\n\n");

    if (cabecalho[0] != '\0') {
        ritmo_centralizar(cabecalho);
        printf("\n");
    }

    ritmo_centralizar("==========================================================");
    printf("\n");

    for (int i = 0; i < PASSOS_POR_COMPASSO; ++i) {
        cursor[i] = (i == passo_atual) ? (i % 2 == 0 ? 'O' : '*') : '-';
    }
    cursor[PASSOS_POR_COMPASSO] = '\0';

    formatar_faixa_grande(linha, sizeof(linha), linha_ativa == 0 ? "> TEMPO :" : "  TEMPO :", cursor);
    ritmo_centralizar(linha);
    printf("\n");

    formatar_faixa_grande(linha, sizeof(linha), linha_ativa == 1 ? "> OUÇA   :" : "  OUÇA   :", ouca);
    ritmo_centralizar(linha);
    printf("\n");

    formatar_faixa_grande(linha, sizeof(linha), linha_ativa == 2 ? "> REPITA:" : "  REPITA:", repita);
    ritmo_centralizar(linha);

    printf("\n");
    ritmo_centralizar("==========================================================");
    printf("\n");

    if (aviso != NULL && aviso[0] != '\0') {
        ritmo_centralizar(aviso);
    }

    fflush(stdout);
}

static void linha_vazia(char *linha) {
    memset(linha, '-', PASSOS_POR_COMPASSO);
    linha[PASSOS_POR_COMPASSO] = '\0';
}

static void esperar_ate(long long momento_ms) {
    while (tempo_ms() < momento_ms) PAUSAR_MS(1);
}

void animar_sequencia(Nota *gabarito, int tempo_passo_ms) {
    Nota *atual = gabarito;
    char ouca[PASSOS_POR_COMPASSO + 1];
    char repita[PASSOS_POR_COMPASSO + 1];

    linha_vazia(ouca);
    linha_vazia(repita);

    char aviso[96];
    snprintf(aviso, sizeof(aviso), "No ritmo... (%c) (%c) (%c) (%c)", teclas[0], teclas[1], teclas[2], teclas[3]);

    if (resultado[0] != '\0') {
        strcpy(ouca, ultimo_gabarito);
        strcpy(repita, ultima_resposta);
        snprintf(aviso, sizeof(aviso), "%s", resultado);
    }
    linha_ativa = 0;

    // metrônomo:
    long long inicio = tempo_ms();
    for (int passo = 0; passo < PASSOS_POR_COMPASSO; ++passo) {
        desenhar_tela(passo, ouca, repita, aviso);
        if (passo % 2 == 0) tocar_tom(1000, 30);
        esperar_ate(inicio + (long long) (passo + 1) * tempo_passo_ms);
    }
    inicio = tempo_ms();
    linha_vazia(ouca);
    linha_vazia(repita);
    linha_ativa = 1;

    const char *contagem[] = {"3...", "2...", "1...", "!"};

    for (int passo = 0; passo < PASSOS_POR_COMPASSO; ++passo) {
        char tecla_da_vez = '\0';
        if (atual != NULL && passo % 2 == 0) {
            tecla_da_vez = atual->tecla;
            atual = atual->prox;
        }

        if (tecla_da_vez != '\0') ouca[passo] = tecla_da_vez;

        desenhar_tela(passo, ouca, repita, contagem[passo / 2]);

        if (tecla_da_vez != '\0')  tocar_nota_musical(tecla_da_vez);
        else if (passo % 2 == 0)   tocar_tom(1000, 30);

        esperar_ate(inicio + (long long) (passo + 1) * tempo_passo_ms);
    }
}

static int tecla_valida(int c) {
    for( int i=0; i<NUM_TECLAS; ++i ){
        if(c==teclas[i]) return 1;
    }
    return 0;
}

int turno_jogador(Nota *gabarito, int tempo_passo_ms, Placar *placar) {
    char esperado[MAX_NOTAS_POR_RODADA];
    char resposta[MAX_NOTAS_POR_RODADA];
    char ouca[PASSOS_POR_COMPASSO + 1];
    char repita[PASSOS_POR_COMPASSO + 1];
    char linha_gabarito[PASSOS_POR_COMPASSO + 1];
    char aviso[64] = "";
    int total = 0;
    int acertos = 0;

    memset(esperado, 0, sizeof(esperado));
    memset(resposta, 0, sizeof(resposta));
    for (Nota *n = gabarito; n != NULL && total < MAX_NOTAS_POR_RODADA; n = n->prox) {
        esperado[total++] = n->tecla;
    }

    linha_vazia(ouca);
    linha_vazia(repita);

    limpar_buffer_teclado();
    linha_ativa = 2;

    long long preparo_ms = (long long) PASSOS_DE_PREPARO * tempo_passo_ms;
    long long total_ms = preparo_ms + (long long) PASSOS_POR_COMPASSO * tempo_passo_ms;
    long long inicio = tempo_ms();
    int passo_exibido = -1000;
    int redesenhar = 1;
    int em_contagem = 0;

    for (;;) {
        long long t = tempo_ms() - inicio;
        if (t >= total_ms) break;

        int passo = (int) (t / tempo_passo_ms) - PASSOS_DE_PREPARO;
        if (passo != passo_exibido) {
            passo_exibido = passo;
            redesenhar = 1;
            if (passo % 2 == 0) tocar_tom(1000, 30);
        }

        int c;
        while ((c = tecla_lida()) >= 0) {
            if (!tecla_valida(c)) continue;

            long long agora = tempo_ms() - inicio;
            long long relativo = agora - preparo_ms + tempo_passo_ms;
            if (relativo < 0) continue;

            int nota = (int) (relativo / (2LL * tempo_passo_ms));
            if (nota >= 0 && nota < total && resposta[nota] == 0) {
                long long alvo = preparo_ms + (long long) nota * 2LL * tempo_passo_ms;
                int atraso_ms = (int) (agora - alvo);
                int tecla_certa = (c == esperado[nota]);

                resposta[nota] = (char) c;
                repita[nota * 2] = (char) c;
                tocar_nota_musical((char) c);

                Julgamento j = julgar_nota(atraso_ms, tempo_passo_ms, tecla_certa);
                int ganho = placar_registrar_nota(placar, j);

                if (j != JULGAMENTO_ERROU) {
                    acertos++;
                    snprintf(aviso, sizeof(aviso), "%s (+%d pts | Combo %dx)",
                             julgamento_texto(j), ganho, placar->combo);
                } else {
                    snprintf(aviso, sizeof(aviso), "ERROU! Esperado: [ %c ]", esperado[nota]);
                }
                redesenhar = 1;
            }
        }

        if (redesenhar) {
            if (passo < 0) {
                int contagem = (-passo + 1 ) / 2 - 1;
                if (contagem > 0) snprintf(aviso, sizeof(aviso), "%d...", contagem);
                else              snprintf(aviso, sizeof(aviso), "!");
                em_contagem = 1;
            } else if (em_contagem) {
                aviso[0] = '\0'; 
                em_contagem = 0; 
            }
            desenhar_tela(passo < 0 ? passo + PASSOS_DE_PREPARO : passo, ouca, repita, aviso);
            redesenhar = 0;
        }
        PAUSAR_MS(5);
    }

    for (int k = 0; k < total; ++k) {
        if (resposta[k] == 0) {
            placar_registrar_nota(placar, JULGAMENTO_ERROU);
        }
    }

    linha_vazia(linha_gabarito);
    for (int k = 0; k < total; ++k) linha_gabarito[k * 2] = esperado[k];

    snprintf(resultado, sizeof(resultado), "Acertos: %d/%d   |   Pontos: %d",
             acertos, total, placar->pontos);
    strcpy(ultimo_gabarito, linha_gabarito);
    strcpy(ultima_resposta, repita);
    return acertos;
}