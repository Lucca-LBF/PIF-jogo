#ifndef NOTAS_H
#define NOTAS_H
#define NUM_TECLAS 4
extern char teclas[NUM_TECLAS];

typedef struct Nota {
    char tecla;
    int duracao_ms;   
    struct Nota *prox;
} Nota;

Nota* criar_nota(char tecla, int duracao_ms);
Nota* enfileirar_nota(Nota *cabeca, char tecla, int duracao_ms);
Nota* gerar_sequencia_aleatoria(int tamanho_sequencia);
void liberar_sequencia(Nota *cabeca);
void imprimir_sequencia_debug(Nota *cabeca);

#endif