#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "notas.h"

Nota* criar_nota(char tecla, int duracao_ms) {
    Nota *nova = (Nota*) malloc(sizeof(Nota));
    if (nova == NULL) {
        printf("Erro fatal: Falha ao alocar memória para a Nota!\n");
        exit(1);
    }
    nova->tecla = tecla;
    nova->duracao_ms = duracao_ms;
    nova->prox = NULL;
    return nova;
}

Nota* enfileirar_nota(Nota *cabeca, char tecla, int duracao_ms) {
    Nota *nova = criar_nota(tecla, duracao_ms);
    if (cabeca == NULL) {
        return nova; 
    }

    Nota *atual = cabeca;
    while (atual->prox != NULL) {
        atual = atual->prox; 
    }
    atual->prox = nova;
    return cabeca;
}

Nota* gerar_sequencia_aleatoria(int tamanho_sequencia) {
    char opcoes_teclas[] = {'A', 'S', 'J', 'K'};
    int num_opcoes = sizeof(opcoes_teclas) / sizeof(opcoes_teclas[0]);

    Nota *gabarito = NULL;

    for (int i = 0; i < tamanho_sequencia; i++) {
        int indice_sorteado = rand() % num_opcoes;
        char tecla_sorteada = opcoes_teclas[indice_sorteado];
        gabarito = enfileirar_nota(gabarito, tecla_sorteada, 500); 
    }

    return gabarito;
}

void liberar_sequencia(Nota *cabeca) {
    Nota *atual = cabeca;
    while (atual != NULL) {
        Nota *proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
}

void imprimir_sequencia_debug(Nota *cabeca) {
    Nota *atual = cabeca;
    printf("Gabarito: ");
    while (atual != NULL) {
        printf("[%c] -> ", atual->tecla);
        atual = atual->prox;
    }
    printf("NULL\n");
}