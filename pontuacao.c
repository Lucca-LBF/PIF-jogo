#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sistema.h"
#include "ritmo.h"
#include "pontuacao.h"

Julgamento julgar_nota(int atraso_ms, int tempo_passo_ms, int tecla_certa) {
    int distancia = abs(atraso_ms);
    int janela_perfeito = (tempo_passo_ms * JANELA_PERFEITO_PCT) / 100;
    int janela_limite = (tempo_passo_ms * JANELA_LIMITE_PCT) / 100;

    if (!tecla_certa) return JULGAMENTO_ERROU;
    if (distancia > janela_limite) return JULGAMENTO_ERROU;
    if (distancia <= janela_perfeito) return JULGAMENTO_PERFEITO;
    return JULGAMENTO_BOM;
}

const char *julgamento_texto(Julgamento j) {
    switch (j) {
        case JULGAMENTO_BOM:      return "BOM!";
        case JULGAMENTO_PERFEITO: return "PERFEITO!";
        default:                  return "ERROU!";
    }
}

void placar_iniciar(Placar *p) {
    memset(p, 0, sizeof(*p));
}

int placar_registrar_nota(Placar *p, Julgamento j) {
    int ganho = 0;

    p->contagem[j]++;
    p->notas_total++;

    if (j == JULGAMENTO_PERFEITO) ganho = PONTOS_PERFEITO;
    else if (j == JULGAMENTO_BOM) ganho = PONTOS_BOM;

    if (j == JULGAMENTO_PERFEITO || j == JULGAMENTO_BOM) {
        p->combo++;
        if (p->combo > p->melhor_combo) p->melhor_combo = p->combo;
        ganho += (p->combo > COMBO_MAX ? COMBO_MAX : p->combo) * BONUS_COMBO;
    } else {
        p->combo = 0;
    }

    p->pontos += ganho;
    return ganho;
}

void pontuacao_tela_final(const Placar *p) {
    char linha[128];
    int ranking[5][2] = {0};


    FILE *arq = fopen("recordes.txt", "r");
    if (arq != NULL) {
        for (int i = 0; i < 5; ++i) {
            if (fscanf(arq, "%d %d", &ranking[i][0], &ranking[i][1]) != 2) break;
        }
        fclose(arq);
    }

    for (int i = 0; i < 5; ++i) {
        if (p->pontos > ranking[i][0]) {
            for (int j = 4; j > i; --j) {
                ranking[j][0] = ranking[j - 1][0];
                ranking[j][1] = ranking[j - 1][1];
            }
            ranking[i][0] = p->pontos;
            ranking[i][1] = p->melhor_combo;
            break;
        }
    }

    arq = fopen("recordes.txt", "w");
    if (arq != NULL) {
        for (int i = 0; i < 5; ++i) {
            fprintf(arq, "%d %d\n", ranking[i][0], ranking[i][1]);
        }
        fclose(arq);
    }

    LIMPAR_TELA();
    printf("\n\n\n");
    ritmo_centralizar("==============================================");
    ritmo_centralizar("F I M   D A S   R O D A D A S");
    ritmo_centralizar("==============================================");
    printf("\n");

    snprintf(linha, sizeof(linha), "PONTUACAO FINAL: %d   |   Melhor combo: %dx",
             p->pontos, p->melhor_combo);
    ritmo_centralizar(linha);

    snprintf(linha, sizeof(linha), "PERFEITO: %d   |   BOM: %d   |   ERROU: %d",
             p->contagem[JULGAMENTO_PERFEITO],
             p->contagem[JULGAMENTO_BOM],
             p->contagem[JULGAMENTO_ERROU]);
    ritmo_centralizar(linha);
    printf("\n");

    ritmo_centralizar("--- TOP 5 RECORDES ---");
    for (int i = 0; i < 5; ++i) {
        if (ranking[i][0] > 0) {
            snprintf(linha, sizeof(linha), "%d lugar: %d pts (Combo %dx)",
                     i + 1, ranking[i][0], ranking[i][1]);
            ritmo_centralizar(linha);
        }
    }

    printf("\n");
    ritmo_centralizar("==============================================");
    printf("\n");
    ritmo_centralizar("Aperte qualquer tecla para sair...");
    fflush(stdout);
}