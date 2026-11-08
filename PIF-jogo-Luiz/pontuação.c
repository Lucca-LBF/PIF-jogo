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