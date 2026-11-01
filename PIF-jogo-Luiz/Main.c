#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "notas.h"

int main() {
    srand(time(NULL));
    printf("=== TESTE DE LUCCA: TESTE DE GABARITO ===\n\n");
    int tamanho_rodada = 5;
    Nota *gabarito_rodada1 = gerar_sequencia_aleatoria(tamanho_rodada);
    imprimir_sequencia_debug(gabarito_rodada1);
    liberar_sequencia(gabarito_rodada1);
    printf("\nMemoria liberada com sucesso!\n");
    return 0;
}