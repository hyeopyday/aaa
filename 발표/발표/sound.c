// sound.c - 단순하고 확실한 방식
#include "sound.h"
#include <stdio.h>
#include <mmsystem.h>
#include <string.h>

#pragma comment(lib, "winmm.lib")

static int current_volume = 50;

void init_sound_system(void) {
    waveOutSetVolume(0, MAKELONG(0xFFFF * current_volume / 100, 0xFFFF * current_volume / 100));
}

void cleanup_sound_system(void) {
    PlaySound(NULL, NULL, 0);
}

void play_sound_once(const char* filename) {
    char path[256];
    sprintf(path, "resources\\sounds\\%s", filename);
    PlaySound(path, NULL, SND_FILENAME | SND_ASYNC);
}

void play_sound_loop(const char* filename) {
    char path[256];
    sprintf(path, "resources\\sounds\\%s", filename);
    PlaySound(path, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void stop_background_music(void) {
    PlaySound(NULL, NULL, 0);
}

void set_volume(int vol) {
    current_volume = vol;
    DWORD volume = 0xFFFF * vol / 100;
    waveOutSetVolume(0, MAKELONG(volume, volume));
}

int get_volume(void) {
    return current_volume;
}