#ifndef SISTEMA_H

#define SISTEMA_H

#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #include <conio.h> //para ler o teclado sem t ravar o SO
    #define LIMPAR_TELA() system("cls")
    #define PAUSAR_MS(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define LIMPAR_TELA() system("clear")
    #define PAUSAR_MS(ms) usleep((ms) * 1000)
#endif

#endif