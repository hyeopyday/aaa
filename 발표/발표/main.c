// main.c
#include "game.h"
#include "sound.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    system("chcp 65001 > nul");  // UTF-8 설정
    init_sound_system();
    show_intro();
    cleanup_sound_system();
    return 0;
}