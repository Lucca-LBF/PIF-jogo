#ifndef PONTUACAO_H
#define PONTUACAO_H

#define PONTOS_PERFEITO 100
#define PONTOS_OK 50
#define PONTOS_ATRASADO 25

#define BONUS_COMBO 5
#define COMBO_MAX 10
#define VIDA_MAX 100
#define DANO_ERRO 15

#define JANELA_PERFEITO_PCT 25
#define JANELA_LIMITE_PCT 70

typedef enum {
    JULGAMENTO_ERROU = 0,
    JULGAMENTO_ATRASADO,
    JULGAMENTO_OK,
    JULGAMENTO_PERFEITO,
    NUM_JULGAMENTOS
} Julgamento;

typedef struct {
    int pontos;
    int combo;
    int vidas;
    int melhor_combo;
    int notas_total;
    int contagem[NUM_JULGAMENTOS];
} Placar;

Julgamento julgar_nota(int atraso_ms, int tempo_passo_ms, int telca_certa);
const char *julgamento_texto(Julgamento j);

void placar_inicar(Placar *p);
int placar_registrar_nota(Placar *p, Julgamento j);
int placar_vivo(const Placar *p);

void pontuação_tela_final(const Placar *p);

#endif