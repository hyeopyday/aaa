// sound.h
#ifndef SOUND_H
#define SOUND_H

#include <windows.h>

void init_sound_system(void);
void cleanup_sound_system(void);
void play_sound_once(const char* filename);
void play_sound_loop(const char* filename);
void stop_background_music(void);
void set_volume(int vol);
int get_volume(void);

#endif