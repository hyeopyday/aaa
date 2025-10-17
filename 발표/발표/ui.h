// ui.h
#ifndef UI_H
#define UI_H

#include "game.h"

void gotoxy(int x, int y);
void clear_screen(void);
void full_clear(void);
void set_color(int color);
void draw_intro_screen(int selected);
void draw_difficulty_screen(int selected);
void draw_game_screen(GameState* state, int cursor_x, int cursor_y);
void draw_settings_screen(int volume);
void draw_game_over_menu(int selected);
void draw_board(void);
void reset_game_screen(void);
void show_ai_thinking(void);
void clear_ai_thinking(void);

#endif