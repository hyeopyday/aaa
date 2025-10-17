// game.h
#ifndef GAME_H
#define GAME_H

#include <windows.h>

#define BOARD_SIZE 19
#define TIMER_SECONDS 30

typedef enum {
    MENU_VS_COMPUTER,
    MENU_TWO_PLAYER,
    MENU_SETTINGS,
    MENU_EXIT
} MenuOption;

typedef enum {
    DIFF_EASY,
    DIFF_MEDIUM,
    DIFF_HARD
} Difficulty;

typedef enum {
    PLAYER_BLACK = 0,
    PLAYER_WHITE = 1
} Player;

typedef struct {
    int board[BOARD_SIZE][BOARD_SIZE];
    int current_player;
    int time_left[2];
    int game_mode;  // 0: vs AI, 1: 2 player
    Difficulty difficulty;
    BOOL game_over;
    int winner;  // -1: none, 0: black, 1: white
    BOOL timeout;  // 타임오버로 끝났는지 플래그 추가!
} GameState;

// Public functions
void show_intro(void);
void game_control_vs_ai(Difficulty diff);
void game_control_two_player(void);
void show_settings(void);
int check_winner(GameState* state, int x, int y);
void reset_game_state(GameState* state);

#endif