#include  <stdio.h>
#include  <ctype.h>
#include  <jogo.h>


void jogador_iniciar(jogador *j) {
    j->pontos = 0;
    j->vidas = VIDAS_INICIAIS;
    j->combo = 0;
    j->rodades = 1;

}