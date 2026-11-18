@echo off

gcc -Wall -Wextra -std=c11 -g -o jogo.exe Main.c notas.c ritmo.c sistema.c
if errorlevel 1 (
    echo.
    echo *** ERRO NA COMPILACAO ***
    pause
    exit /b 1
)

echo Compilado com sucesso: jogo.exe
echo.
jogo.exe
