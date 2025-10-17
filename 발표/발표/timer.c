// timer.c
#include "timer.h"
#include "sound.h"
#include <stdio.h>

typedef struct {
    GameState* state;
    BOOL running;
} TimerData;

DWORD WINAPI timer_thread(LPVOID param) {
    TimerData* data = (TimerData*)param;
    
    while(data->running && !data->state->game_over) {
        Sleep(1000);
        
        if(data->state->time_left[data->state->current_player] > 0) {
            data->state->time_left[data->state->current_player]--;
            
            // 10초 이하일 때 tick 소리
            if(data->state->time_left[data->state->current_player] <= 10 && 
               data->state->time_left[data->state->current_player] > 0) {
                play_sound_once("tick.wav");
            }
        }
    }
    
    return 0;
}

HANDLE start_timer(GameState* state) {
    TimerData* data = (TimerData*)malloc(sizeof(TimerData));
    data->state = state;
    data->running = TRUE;
    
    HANDLE handle = CreateThread(NULL, 0, timer_thread, data, 0, NULL);
    return handle;
}

void stop_timer(HANDLE handle) {
    if(handle) {
        TerminateThread(handle, 0);
        CloseHandle(handle);
    }
}

void reset_timer(GameState* state) {
    state->time_left[state->current_player] = TIMER_SECONDS;
}