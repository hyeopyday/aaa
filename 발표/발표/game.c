// game.c - 완전한 버전 (goto 완전 제거)
#include "game.h"
#include "ui.h"
#include "sound.h"
#include "ai.h"
#include "timer.h"
#include "animation.h"
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

// 함수 전방 선언
static int select_difficulty(void);

void show_intro(void) {
    int selected = 0;
    char key;
    
    play_sound_loop("intro.wav");
    
    while(1) {
        clear_screen();
        draw_intro_screen(selected);
        
        key = getch();
        if(key == 27) break;  // ESC
        
        if(key == 72) {  // UP
            selected = (selected - 1 + 4) % 4;
            play_sound_once("move.wav");
        }
        else if(key == 80) {  // DOWN
            selected = (selected + 1) % 4;
            play_sound_once("move.wav");
        }
        else if(key == 13) {  // ENTER
            play_sound_once("button.wav");
            stop_background_music();
            
            switch(selected) {
                case MENU_VS_COMPUTER: {
                    int diff = select_difficulty();
                    if(diff >= 0) game_control_vs_ai(diff);
                    break;
                }
                case MENU_TWO_PLAYER:
                    game_control_two_player();
                    break;
                case MENU_SETTINGS:
                    show_settings();
                    break;
                case MENU_EXIT:
                    return;
            }
            play_sound_loop("intro.wav");
        }
    }
}

static int select_difficulty(void) {
    int selected = 0;
    char key;
    
    while(1) {
        clear_screen();
        draw_difficulty_screen(selected);
        
        key = getch();
        if(key == 27) return -1;  // ESC
        
        if(key == 72) {  // UP
            selected = (selected - 1 + 3) % 3;
            play_sound_once("move.wav");
        }
        else if(key == 80) {  // DOWN
            selected = (selected + 1) % 3;
            play_sound_once("move.wav");
        }
        else if(key == 13) {  // ENTER
            play_sound_once("button.wav");
            return selected;
        }
    }
}

void game_control_vs_ai(Difficulty diff) {
    GameState state;
    int cursor_x = 1, cursor_y = 1;
    char key;
    HANDLE timer_handle;
    
    reset_game_state(&state);
    state.game_mode = 0;
    state.difficulty = diff;
    
    full_clear();
    Sleep(200);
    
    play_sound_loop("bgm_game.wav");
    timer_handle = start_timer(&state);
    
    while(!state.game_over) {
        // 타임오버 체크
        if(state.time_left[state.current_player] <= 0 && !state.timeout) {
            state.game_over = TRUE;
            state.winner = 1 - state.current_player;
            state.timeout = TRUE;
            
            stop_timer(timer_handle);
            stop_background_music();
            Sleep(300);
            
            play_sound_once("timeup.wav");
            Sleep(500);
            show_timeout_animation();
            break;
        }
        
        gotoxy(1, 1);
        draw_game_screen(&state, cursor_x, cursor_y);
        
        if(state.current_player == PLAYER_WHITE) {
            show_ai_thinking();
            Sleep(500);
            
            int ai_x, ai_y;
            ai_make_move(&state, &ai_x, &ai_y);
            
            state.board[ai_y][ai_x] = PLAYER_WHITE + 1;
            play_sound_once("place.wav");
            
            clear_ai_thinking();
            
            if(check_winner(&state, ai_x, ai_y)) {
                state.game_over = TRUE;
                state.winner = PLAYER_WHITE;
            } else {
                state.current_player = PLAYER_BLACK;
                reset_timer(&state);
            }
            continue;
        }
        
        if(kbhit()) {
            key = getch();
            
            if(key == 27) {
                state.game_over = TRUE;
                break;
            }
            else if(key == 72) {
                cursor_y = (cursor_y > 1) ? cursor_y - 1 : 1;
                play_sound_once("move.wav");
            }
            else if(key == 80) {
                cursor_y = (cursor_y < BOARD_SIZE) ? cursor_y + 1 : BOARD_SIZE;
                play_sound_once("move.wav");
            }
            else if(key == 75) {
                cursor_x = (cursor_x > 1) ? cursor_x - 1 : 1;
                play_sound_once("move.wav");
            }
            else if(key == 77) {
                cursor_x = (cursor_x < BOARD_SIZE) ? cursor_x + 1 : BOARD_SIZE;
                play_sound_once("move.wav");
            }
            else if(key == 32) {
                if(state.board[cursor_y - 1][cursor_x - 1] == 0) {
                    state.board[cursor_y - 1][cursor_x - 1] = PLAYER_BLACK + 1;
                    play_sound_once("place.wav");
                    
                    if(check_winner(&state, cursor_x - 1, cursor_y - 1)) {
                        state.game_over = TRUE;
                        state.winner = PLAYER_BLACK;
                    } else {
                        state.current_player = PLAYER_WHITE;
                        reset_timer(&state);
                    }
                } else {
                    play_sound_once("warning.wav");
                }
            }
        }
        
        Sleep(50);
    }
    
    if(!state.timeout) {
        stop_timer(timer_handle);
        stop_background_music();
    }
    
    if(state.timeout) {
        play_sound_once("lose_bgm.wav");
        show_defeat_animation();
    }
    else if(state.winner == PLAYER_BLACK) {
        play_sound_once("win_fireworks.wav");
        show_victory_animation();
    } 
    else {
        play_sound_once("lose_bgm.wav");
        show_defeat_animation();
    }
    
    Sleep(1000);
}

void game_control_two_player(void) {
    GameState state;
    int cursor_x = 1, cursor_y = 1;
    char key;
    HANDLE timer_handle;
    
    reset_game_state(&state);
    state.game_mode = 1;
    
    full_clear();
    Sleep(200);
    
    play_sound_loop("bgm_game.wav");
    timer_handle = start_timer(&state);
    
    while(!state.game_over) {
        // 타임오버 체크
        if(state.time_left[state.current_player] <= 0 && !state.timeout) {
            state.game_over = TRUE;
            state.winner = 1 - state.current_player;
            state.timeout = TRUE;
            
            stop_timer(timer_handle);
            stop_background_music();
            Sleep(300);
            
            play_sound_once("timeup.wav");
            Sleep(500);
            show_timeout_animation();
            break;
        }
        
        gotoxy(1, 1);
        draw_game_screen(&state, cursor_x, cursor_y);
        
        if(kbhit()) {
            key = getch();
            
            if(key == 27) {
                state.game_over = TRUE;
                break;
            }
            else if(key == 72) {
                cursor_y = (cursor_y > 1) ? cursor_y - 1 : 1;
                play_sound_once("move.wav");
            }
            else if(key == 80) {
                cursor_y = (cursor_y < BOARD_SIZE) ? cursor_y + 1 : BOARD_SIZE;
                play_sound_once("move.wav");
            }
            else if(key == 75) {
                cursor_x = (cursor_x > 1) ? cursor_x - 1 : 1;
                play_sound_once("move.wav");
            }
            else if(key == 77) {
                cursor_x = (cursor_x < BOARD_SIZE) ? cursor_x + 1 : BOARD_SIZE;
                play_sound_once("move.wav");
            }
            else if(key == 32) {
                if(state.board[cursor_y - 1][cursor_x - 1] == 0) {
                    state.board[cursor_y - 1][cursor_x - 1] = state.current_player + 1;
                    play_sound_once("place.wav");
                    
                    if(check_winner(&state, cursor_x - 1, cursor_y - 1)) {
                        state.game_over = TRUE;
                        state.winner = state.current_player;
                    } else {
                        state.current_player = 1 - state.current_player;
                        reset_timer(&state);
                    }
                } else {
                    play_sound_once("warning.wav");
                }
            }
        }
        
        Sleep(50);
    }
    
    if(!state.timeout) {
        stop_timer(timer_handle);
        stop_background_music();
    }
    
    if(state.timeout) {
        play_sound_once("win_fireworks.wav");
        if(state.winner == PLAYER_BLACK) {
            show_black_win_animation();
        } else {
            show_white_win_animation();
        }
    }
    else if(state.winner == PLAYER_BLACK) {
        play_sound_once("win_fireworks.wav");
        show_black_win_animation();
    } 
    else if(state.winner == PLAYER_WHITE) {
        play_sound_once("win_fireworks.wav");
        show_white_win_animation();
    }
    
    Sleep(1000);
}

void show_settings(void) {
    int volume = get_volume();
    char key;
    
    while(1) {
        clear_screen();
        draw_settings_screen(volume);
        
        key = getch();
        if(key == 27) break;
        
        if(key == 75) {
            volume = (volume > 0) ? volume - 10 : 0;
            set_volume(volume);
            play_sound_once("button.wav");
        }
        else if(key == 77) {
            volume = (volume < 100) ? volume + 10 : 100;
            set_volume(volume);
            play_sound_once("button.wav");
        }
        else if(key == 13) {
            break;
        }
    }
}

int check_winner(GameState* state, int x, int y) {
    int player = state->board[y][x];
    int dx[] = {1, 0, 1, 1};
    int dy[] = {0, 1, 1, -1};
    
    for(int dir = 0; dir < 4; dir++) {
        int count = 1;
        
        for(int i = 1; i < 5; i++) {
            int nx = x + dx[dir] * i;
            int ny = y + dy[dir] * i;
            if(nx < 0 || nx >= BOARD_SIZE || ny < 0 || ny >= BOARD_SIZE) break;
            if(state->board[ny][nx] != player) break;
            count++;
        }
        
        for(int i = 1; i < 5; i++) {
            int nx = x - dx[dir] * i;
            int ny = y - dy[dir] * i;
            if(nx < 0 || nx >= BOARD_SIZE || ny < 0 || ny >= BOARD_SIZE) break;
            if(state->board[ny][nx] != player) break;
            count++;
        }
        
        if(count >= 5) return 1;
    }
    
    return 0;
}

void reset_game_state(GameState* state) {
    memset(state->board, 0, sizeof(state->board));
    state->current_player = PLAYER_BLACK;
    state->time_left[0] = TIMER_SECONDS;
    state->time_left[1] = TIMER_SECONDS;
    state->game_over = FALSE;
    state->winner = -1;
    state->timeout = FALSE;
}