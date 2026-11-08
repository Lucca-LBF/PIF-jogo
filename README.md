# Pongue
repositório para o jogo da cadeira de PiF.

Jogo de ritmo em C, estilo PaRappa the Rapper: o computador toca uma sequência
com as teclas possiveis: `A S J K` 

### *O jogador repete no mesmo ritmo!!!*

## Como compilar e rodar

**Windows** (precisa do gcc no PATH, ex.: MSYS2/MinGW): dê dois cliques em
`compilar.bat`, ou no terminal:

    gcc -Wall -Wextra -std=c11 -g -o jogo.exe Main.c notas.c ritmo.c sistema.c
    jogo.exe

**Linux / macOS:**

    make run

O gcc precisa receber **todos** os `.c` de uma vez. Compilar só o arquivo aberto
no editor (`Main.c` sozinho) dá erro de `undefined reference`.

## Organização

| Arquivo | O que faz |
|---|---|
| `Main.c` | fluxo das rodadas e dificuldade |
| `notas.c/h` | lista encadeada com a sequência sorteada |
| `ritmo.c/h` | tela, animação da sequência e turno do jogador |
| `sistema.c/h` | teclado, tempo, som e tela em Windows/Linux/macOS |
