#if !defined(_WIN32) && !defined(_WIN64)
    #define _POSIX_C_SOURCE 200809L
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "sistema.h"

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>
#include <conio.h>

#define MODO_ANSI 0x0004

static int usa_ansi = 0;
static int iniciado = 0;

void iniciar_sistema(void) {
    HANDLE saida;
    DWORD modo = 0;

    if (iniciado) return;
    iniciado = 1;

    SetConsoleOutputCP(CP_UTF8);

    saida = GetStdHandle(STD_OUTPUT_HANDLE);
    if (saida != INVALID_HANDLE_VALUE && GetConsoleMode(saida, &modo)) {
        if (SetConsoleMode(saida, modo | MODO_ANSI)) usa_ansi = 1;
    }
}

void encerrar_sistema(void) {
}

void limpar_tela(void) {
    if (usa_ansi) {
        printf("\033[2J\033[H");
        fflush(stdout);
    } else {
        system("cls");
    }
}

void pausar_ms(int ms) {
    if (ms > 0) Sleep((DWORD) ms);
}

long long tempo_ms(void) {
    return (long long) GetTickCount64();
}

int tecla_lida(void) {
    int c;
    if (!_kbhit()) return -1;
    c = _getch();
    if (c == 0 || c == 0xE0) {
        _getch();
        return -1;
    }
    return toupper((unsigned char) c);
}

typedef struct { int freq; int dur; } ParamTom;

static DWORD WINAPI thread_tom(LPVOID arg) {
    ParamTom p = *(ParamTom *) arg;
    free(arg);
    Beep((DWORD) p.freq, (DWORD) p.dur);
    return 0;
}

void tocar_tom(int frequencia_hz, int duracao_ms) {
    HANDLE h;
    ParamTom *p = (ParamTom *) malloc(sizeof(ParamTom));
    if (p == NULL) return;
    p->freq = frequencia_hz;
    p->dur  = duracao_ms;
    h = CreateThread(NULL, 0, thread_tom, p, 0, NULL);
    if (h != NULL) CloseHandle(h);
    else free(p);
}

#else

#include <signal.h>
#include <sys/select.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

static struct termios termios_original;
static int termios_salvo = 0;
static int iniciado = 0;

static void tratar_sigint(int sinal) {
    (void) sinal;
    encerrar_sistema();
    _exit(130);
}

void iniciar_sistema(void) {
    if (iniciado) return;
    iniciado = 1;

    if (isatty(STDIN_FILENO) && tcgetattr(STDIN_FILENO, &termios_original) == 0) {
        struct termios cru = termios_original;
        cru.c_lflag &= (tcflag_t) ~(ICANON | ECHO);
        cru.c_cc[VMIN]  = 0;
        cru.c_cc[VTIME] = 0;
        if (tcsetattr(STDIN_FILENO, TCSANOW, &cru) == 0) termios_salvo = 1;
    }
    atexit(encerrar_sistema);
    signal(SIGINT, tratar_sigint);
}

void encerrar_sistema(void) {
    if (termios_salvo) {
        tcsetattr(STDIN_FILENO, TCSANOW, &termios_original);
        termios_salvo = 0;
    }
}

void limpar_tela(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
}

void pausar_ms(int ms) {
    struct timespec t;
    if (ms <= 0) return;
    t.tv_sec  = ms / 1000;
    t.tv_nsec = (long) (ms % 1000) * 1000000L;
    nanosleep(&t, NULL);
}

long long tempo_ms(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (long long) t.tv_sec * 1000LL + t.tv_nsec / 1000000LL;
}

static int ler_byte(void) {
    fd_set conj;
    struct timeval zero = {0, 0};
    unsigned char c;
    ssize_t n;

    FD_ZERO(&conj);
    FD_SET(STDIN_FILENO, &conj);
    if (select(STDIN_FILENO + 1, &conj, NULL, NULL, &zero) <= 0) return -1;
    n = read(STDIN_FILENO, &c, 1);
    if (n == 0) return -2;
    if (n < 0)  return -1;
    return c;
}

int tecla_lida(void) {
    int c = ler_byte();
    if (c < 0) return c;
    if (c == 27) {
        ler_byte();
        ler_byte();
        return -1;
    }
    return toupper(c);
}

void tocar_tom(int frequencia_hz, int duracao_ms) {
    (void) frequencia_hz;
    (void) duracao_ms;
    putchar('\a');
    fflush(stdout);
}

#endif

int ler_tecla_bloqueante(void) {
    for (;;) {
        int c = tecla_lida();
        if (c != -1) return c;
        pausar_ms(10);
    }
}

void limpar_buffer_teclado(void) {
    while (tecla_lida() >= 0) {
    }
}
