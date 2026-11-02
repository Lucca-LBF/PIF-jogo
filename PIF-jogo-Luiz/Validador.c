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
int comparar_filas(const Fila *gabarito, const Fila *resposta) {
    const Nota *gabarito = gabarito->inicio;
    const Nota *resposta = resposta->inicio;
    int acertos = 0;
    while (gabarito != NULL && resposta != NULL) {
        if (gabarito->tecla == resposta->tecla) {
            acertos++;
        }
        gabarito = gabarito->proximo;
        resposta = resposta->proximo;
    }
    return acertos;
}
void atualizar_jogador(Jogador *j, int acertos, int total) {
    if (acertos == total) {
        j->combo++;
        j->pontos += acertos + j->combo;
    } else {
        j->combo = 0;
        j->vidas--;
        j->pontos += acertos;
    }
    
}