// timer.h
#ifndef TIMER_H
#define TIMER_H

#include "game.h"
#include <windows.h>

HANDLE start_timer(GameState* state);
void stop_timer(HANDLE handle);
void reset_timer(GameState* state);

#endif