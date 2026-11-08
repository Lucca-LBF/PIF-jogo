#include <stdio.h>
#include <string.h>
#include "sistema.h"
#include "pontuacao.h"

Julgamento julgar_nota(int atraso_ms, int tempo_passo_ms, int tecla_certa) {

    int janela_perfeito = (tempo_passo_ms * JANELA_PERFEITO_PCT) / 100;
    int janela_limite = (tempo_passo_ms * JANELA_LIMITE_PCT) / 100;

    if (!tecla_certa) return JULGAMENTO_ERROU;
    if (atraso_ms < -janela_limite || atraso_ms > janela_limite) return JULGAMENTO_ERROU;
    if (atraso_ms >= -janela_perfeito && atraso_ms <= janela_perfeito) return JULGAMENTO_ATRASADO;
      return (atraso_ms < 0) ? JULGAMENTO_OK : JULGAMENTO_PERFEITO;
}

const chat *julgamento_texto(Julgamento j) {
    switch (j) {
        case JULGAMENTO_ATRASADO: return "ATRASADO";
        case JULGAMENTO_OK: return "OK";
        case JULGAMENTO_PERFEITO: return "PERFEITO";
        default:    return "ERROU!";
    }
}


void placar_inicar(Placar *p) {
    memset(p, 0, sizeof(*p));
    p->vidas = VIDA_MAX;
}

int placar_registrar_nota(Placar *p, Julgamento j) {
    int ganho = 0;

    p->contagem[j]++;
    p->notas_total++;

    if (j == JULGAMENTO_PERFEITO)      ganho = PONTOS_PERFEITO;
    else if (j == JULGAMENTO_OK)       ganho = PONTOS_OK;
    else if (j == JULGAMENTO_ATRASADO) ganho = PONTOS_ATRASADO;
    
    if (j == JULGAMENTO_PERFEITO || j == JULGAMENTO_OK) {
        p->combo++;
        if (p->combo > p->melhor_combo) p->melhor_combo = p->combo;
        ganho += (p->combo > COMBO_MAX ? COMBO_MAX : p->combo) * BONUS_COMBO;
} else {
    p->combo = 0;
}

if (j == JULGAMENTO_ERROU) {
    p->vidas -= DANO_ERRO;
    if (p->vidas < 0) p->vidas = 0;
}

p->pontos += ganho;
return ganho;
}

int placar_vivo(const Placar *p) {
    return p->vidas > 0;
}