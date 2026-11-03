#include "sistema.h"
#include "notas.h"
#include <stdio.h>
#include <stdlib.h>

void efeito_digitacao(const char* texto, int velocidade_ms){
    int i=0;
    while (texto[i] != '\0'){
        printf("%c", texto[i]);
        fflush(stdout);
        PAUSAR_MS(velocidade_ms);
        ++i;
    }
}

void tocar_nota_musical(char tecla){
    int freq = 261; // Dó
    if (tecla == 'S') freq = 329; // Mi
    else if (tecla == 'J') freq = 392; // Sol
    else if (tecla == 'K') freq = 523; // Dó agudo

    #if defined (_WIN32) || defined (_WIND64)
        Beep(freq, 150); // Windows: Frequência e Duração em ms
    #elif defined (__APPLE__)
        char cmd[64]; // Mac: AppleScript nativo
        sprintf(cmd, "osascript -e 'beep f %d'", freq);
        system(cmd);
    #else
        char cmd[128]; // Linux: bruxaria com senos concenos e demais palhaçadas
        sprintf(cmd, "speaker-test -t sine -f %d -l 1 > /dev/null 2>&1 & sleep 0.15 && killall speaker-test > /dev/null 2>&1", freq);
        system(cmd);
    #endif
}

void desenhar_grid_ritmo(int passo_atual, char tecla_computador, int fase){
    printf("\r");
    //metrônomo 4/4 120bpm
    printf("Veja...\t");
    for(int i=0;i<8;++i){
        if(i==passo_atual) printf("O");
        else if(i%2 == 0)  printf("x");
        else               printf("-");
    }
    printf("\n");

    printf("Ouça...\t");
    for(int i=0;i<8;++i){
        if(fase ==1 && i==passo_atual && tecla_computador != '\0'){
            printf("%c", tecla_computador);
        } else {
            printf("-");
        }
    }
    printf("\n");
    printf("Repita...\t");
    for(int i=0; i<8; ++i){
        printf("-");
    }
    printf("\n====================================");
}

void animar_sequencia(Nota* lista_gabarito){
    Nota* atual = lista_gabarito;
    efeito_digitacao("No ritmo... 3, 2, 1...", 40);
    PAUSAR_MS(1000);

    int tempo_passo_ms = 250; //250 ms = 120 BPM

    for(int passo =0; passo<8; ++passo){
        char tecla_da_vez = '\0';
        if(atual != NULL && passo %2 ==0){
            tecla_da_vez = atual->tecla;
            atual = atual->prox;
        }

        LIMPAR_TELA();
        desenhar_grid_ritmo(passo, tecla_da_vez, 1);
        fflush(stdout);
        
        if (tecla_da_vez != '\0')   tocar_nota_musical(tecla_da_vez);
        else if (passo%2 ==0)      {printf("\a"); fflush(stdout);}

        PAUSAR_MS(tempo_passo_ms);
    }

    LIMPAR_TELA();
    desenhar_grid_ritmo(-1, '\0', 2);

    printf("\n");
    efeito_digitacao("GO!\n", 40);
    fflush(stdout);

    /*TO-DO ========================
    --LUIZ--
    Loop de validação aqui!
    Roda um loop de 8 passos igual o meu acima,
    capturar tecla do jogador com conio.h/read() e
    comparar com gabarito
    ================================
    */
}