#ifndef SISTEMA_H
#define SISTEMA_H

#include <stdio.h>
#include <stdlib.h>

#define LIMPAR_TELA()   limpar_tela()
#define PAUSAR_MS(ms)   pausar_ms(ms)

void iniciar_sistema(void);
void encerrar_sistema(void);

void limpar_tela(void);
void pausar_ms(int ms);
long long tempo_ms(void);

int tecla_lida(void);
int ler_tecla_bloqueante(void);
void limpar_buffer_teclado(void);

void tocar_tom(int frequencia_hz, int duracao_ms);

#endif
