#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "notas.h"


int main() {
    srand(time(NULL));
    printf("=== TESTE DE LUCCA: TESTE DE GABARITO ===\n\n");
    int tamanho_rodada = 5;

    Nota *gabarito = gerar_sequencia_aleatoria(tamanho_rodada);
    imprimir_sequencia_debug(gabarito);

    /* 
    1. Pegar o Gabarito: Receber a lista encadeada de lucca
    2. Capturar a Entrada: Rodar um loop de 8 passos (no mesmo tempo de 120 BPM) usando o getch() ou kbhit()
    3. Validar Nó por Nó: Comparar se a tecla que o jogador apertou no passo X é igual à tecla que estava armazenada na struct daquela posição.
    4. Gerenciar o Estado do Jogo 
    */
    liberar_sequencia(gabarito);
    printf("\nMemoria liberada com sucesso!\n");
    return 0;
}