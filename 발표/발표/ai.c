// ai.c
#include "ai.h"
#include <stdlib.h>
#include <time.h>

static int is_valid_move(GameState* state, int x, int y) {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && state->board[y][x] == 0;
}

static int count_line(GameState* state, int x, int y, int dx, int dy, int player) {
    int count = 0;
    for(int i = 1; i < 5; i++) {
        int nx = x + dx * i;
        int ny = y + dy * i;
        if(nx < 0 || nx >= BOARD_SIZE || ny < 0 || ny >= BOARD_SIZE) break;
        if(state->board[ny][nx] != player) break;
        count++;
    }
    return count;
}

static int evaluate_position(GameState* state, int x, int y, int player) {
    int score = 0;
    int dx[] = {1, 0, 1, 1};
    int dy[] = {0, 1, 1, -1};
    
    for(int dir = 0; dir < 4; dir++) {
        int forward = count_line(state, x, y, dx[dir], dy[dir], player);
        int backward = count_line(state, x, y, -dx[dir], -dy[dir], player);
        int total = forward + backward + 1;
        
        if(total >= 5) score += 100000;
        else if(total == 4) score += 10000;
        else if(total == 3) score += 1000;
        else if(total == 2) score += 100;
    }
    
    return score;
}

void ai_make_move(GameState* state, int* out_x, int* out_y) {
    static int initialized = 0;
    if(!initialized) {
        srand(time(NULL));
        initialized = 1;
    }
    
    if(state->difficulty == DIFF_EASY) {
        // Easy: Random near player
        int attempts = 0;
        do {
            int px = -1, py = -1;
            // Find any player stone
            for(int y = 0; y < BOARD_SIZE && px == -1; y++) {
                for(int x = 0; x < BOARD_SIZE && px == -1; x++) {
                    if(state->board[y][x] == 1) {
                        px = x;
                        py = y;
                    }
                }
            }
            
            if(px == -1) {
                // No stones yet, place in center
                *out_x = BOARD_SIZE / 2;
                *out_y = BOARD_SIZE / 2;
                return;
            }
            
            *out_x = px + (rand() % 3 - 1);
            *out_y = py + (rand() % 3 - 1);
            attempts++;
        } while(!is_valid_move(state, *out_x, *out_y) && attempts < 100);
        
        if(attempts >= 100) {
            // Fallback: any empty spot
            for(int y = 0; y < BOARD_SIZE; y++) {
                for(int x = 0; x < BOARD_SIZE; x++) {
                    if(state->board[y][x] == 0) {
                        *out_x = x;
                        *out_y = y;
                        return;
                    }
                }
            }
        }
    }
    else if(state->difficulty == DIFF_MEDIUM) {
        // Medium: Block opponent's 4-in-a-row
        int best_x = -1, best_y = -1;
        int max_score = -1;
        
        for(int y = 0; y < BOARD_SIZE; y++) {
            for(int x = 0; x < BOARD_SIZE; x++) {
                if(state->board[y][x] == 0) {
                    int score = evaluate_position(state, x, y, 1);  // Check opponent
                    if(score > max_score) {
                        max_score = score;
                        best_x = x;
                        best_y = y;
                    }
                }
            }
        }
        
        if(best_x != -1) {
            *out_x = best_x;
            *out_y = best_y;
        } else {
            // Fallback to easy mode
            state->difficulty = DIFF_EASY;
            ai_make_move(state, out_x, out_y);
            state->difficulty = DIFF_MEDIUM;
        }
    }
    else {
        // Hard: Both attack and defend
        int best_x = -1, best_y = -1;
        int max_score = -1;
        
        for(int y = 0; y < BOARD_SIZE; y++) {
            for(int x = 0; x < BOARD_SIZE; x++) {
                if(state->board[y][x] == 0) {
                    int attack_score = evaluate_position(state, x, y, 2);  // AI
                    int defense_score = evaluate_position(state, x, y, 1);  // Player
                    int total_score = attack_score * 2 + defense_score;  // Prefer attack
                    
                    if(total_score > max_score) {
                        max_score = total_score;
                        best_x = x;
                        best_y = y;
                    }
                }
            }
        }
        
        if(best_x != -1) {
            *out_x = best_x;
            *out_y = best_y;
        } else {
            *out_x = BOARD_SIZE / 2;
            *out_y = BOARD_SIZE / 2;
        }
    }
}