#include  <stdio.h>
#include  <ctype.h>
#include  <jogo.h>
#include  <string.h>
#ifdef _WIN32
#include <windows.h>
#include <unistd.h>



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

// Lê a resposta do jogador, armazenando as teclas válidas na fila resp
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

// Compara as filas de notas do gabarito e da resposta, retornando o número de acertos
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

// Atualiza os pontos, combo e vidas do jogador com base no número de acertos e no total de notas
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

// Limpar e pausar a tela
void apagar_tela(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");    
    #endif
}

void apagar_ms(int ms) {
    #ifdef _WIN32
        Sleep(ms);
    #else
        usleep(ms * 1000);
    #endif
}

// Turno completo
int turno_jogador(Jogador *j, const Fila *gabarito) {
    Fila resposta;
    int acertos;

    LIMPAR_TELA();
    printf("Digite as %d notas e aperter ENTER", gabarito->tamanho);
    fflush(stdout);

    ler_resposta(&resposta, gabarito->tamanho);
    acertos = comparar_filas(gabarito, &resposta);
    atualizar_jogador(j, acertos, gabarito->tamanho);

    printf(acertos ==  gabarito->tamanho ? "\n   PERFEITO!\n" : "\n   Errou...\n");
    fflush(stdout);
    PAUSAR_MS(1500);

    fila_liberar(&resposta);
    return acertos;