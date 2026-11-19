# Pongue
Jogo de ritmo em C, estilo PaRappa the Rapper, feito para a cadeira de PiF.

O computador toca uma sequência de teclas e o jogador repete no mesmo ritmo.
As teclas padrão são `D F J K` e podem ser trocadas no menu (Teclas).

## Requisitos
- Compilador **gcc** com suporte a C11.
  - Windows: MSYS2 (msys2.org). No terminal "MSYS2 UCRT64", rode
    `pacman -S mingw-w64-ucrt-x86_64-gcc` e adicione `C:\msys64\ucrt64\bin` ao PATH.
  - Linux: `sudo apt install gcc make`.
  - macOS: `xcode-select --install`.
- Um terminal com pelo menos 80 colunas (cmd, PowerShell, Windows Terminal, ou
  o terminal do Linux/macOS). Não rode pelo "Debug Console" do VS Code, que não
  recebe teclas.

## Como compilar e rodar

**Windows:** dê dois cliques em `compilar.bat` (compila e já abre o jogo), ou no terminal:

    gcc -Wall -Wextra -std=c11 -g -o jogo.exe Main.c notas.c ritmo.c pontuacao.c menu.c sistema.c
    jogo.exe

**Linux / macOS:**

    make run

O gcc precisa receber **todos** os `.c` de uma vez. Compilar só um arquivo
(por exemplo `Main.c` sozinho) dá erro de `undefined reference`.

## Problemas comuns
- `undefined reference to ...`: faltou algum `.c` na compilação (veja a lista acima).
- `gcc não é reconhecido`: o gcc não está no PATH. Feche e abra o terminal depois de adicioná-lo.
- Acentos estranhos: salve os arquivos em UTF-8 e use o Windows Terminal.

## Organização
| Arquivo | O que faz |
|---|---|
| `Main.c` | fluxo do jogo: menu, rodadas e dificuldade |
| `notas.c/h` | sequência de notas sorteadas e teclas do jogo |
| `ritmo.c/h` | tela do ritmo, animação da sequência e turno do jogador |
| `pontuacao.c/h` | julgamento das notas e pontuação |
| `menu.c/h` | menu principal, teclas e instruções em ASCII |
| `sistema.c/h` | teclado, tempo, som e tela no Windows/Linux/macOS |
