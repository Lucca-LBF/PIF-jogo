#include  <stdio.h>
#include  <ctype.h>
#include  <jogo.h>


void jogador_iniciar(jogador *j) {
    j->pontos = 0;
    j->vidas = VIDAS_INICIAIS;
    j->combo = 0;
    j->rodades = 1;

}


static int tecla_validar(char tecla) {
    int i;
    for (i = 0; i < NUM_TECLAS; i++) {
        if (TECLAS == tecla[i]) {
            return 1;
        }
    }   return 0;
}
int ler_resposta(Fila *resp, int max) {
    char buffer[TAM_BUF];
    int i, lidas = 0;

    fila_iniciar(resp);

    if (fgets(buferr, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    for (i = 0; buffer[i] != '\0' && lidas < max; i++) {
        char tecla = (char) toupper((unsigned char)buffer[i]);
        if (tecla_validar(tecla)) {
            if fila_inserir(resp, tecla) lidas++;
        }
        
        
    }
}   return lidas;
