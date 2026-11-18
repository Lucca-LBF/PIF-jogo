#ifndef PONTUACAO_H
#define PONTUACAO_H

#define PONTOS_PERFEITO 100
#define PONTOS_BOM      50

#define BONUS_COMBO     5
#define COMBO_MAX       10

#define JANELA_PERFEITO_PCT 30
#define JANELA_LIMITE_PCT   100

typedef enum {
    JULGAMENTO_ERROU = 0,
    JULGAMENTO_BOM,
    JULGAMENTO_PERFEITO,
    NUM_JULGAMENTOS
} Julgamento;

typedef struct {
    int pontos;
    int combo;
    int melhor_combo;
    int notas_total;
    int contagem[NUM_JULGAMENTOS];
} Placar;

Julgamento julgar_nota(int atraso_ms, int tempo_passo_ms, int tecla_certa);
const char *julgamento_texto(Julgamento j);

void placar_iniciar(Placar *p);
int placar_registrar_nota(Placar *p, Julgamento j);

void pontuacao_tela_final(const Placar *p);

#endif