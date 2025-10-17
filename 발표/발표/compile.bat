@echo off
echo Compiling Omok Game...
gcc -g main.c game.c ui.c sound.c ai.c timer.c animation.c -o OmokGame.exe -lwinmm

if %errorlevel% equ 0 (
    echo.
    echo ========================================
    echo   Compilation Successful!
    echo ========================================
    echo.
    echo Running the game...
    echo.
    OmokGame.exe
) else (
    echo.
    echo ========================================
    echo   Compilation Failed!
    echo ========================================
    pause
)