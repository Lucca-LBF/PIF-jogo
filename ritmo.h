#ifndef RITMO_H
#define RITMO_H

#include "notas.h"

#define PASSOS_POR_COMPASSO   8
#define MAX_NOTAS_POR_RODADA  4
#define PASSO_PADRAO_MS       250

void ritmo_definir_cabecalho(const char *texto);

void efeito_digitacao(const char *texto, int velocidade_ms);
void tocar_nota_musical(char tecla);

void animar_sequencia(Nota *gabarito, int tempo_passo_ms);
int turno_jogador(Nota *gabarito, int tempo_passo_ms);

#endif
