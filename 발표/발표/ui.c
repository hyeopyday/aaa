// ui.c - 완전히 새로운 버퍼 방식
#include "ui.h"
#include <stdio.h>
#include <windows.h>
#include <string.h>

void gotoxy(int x, int y) {
    COORD pos = {x - 1, y - 1};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void clear_screen(void) {
    system("cls");
}

void full_clear(void) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = {0, 0};
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    SetConsoleCursorPosition(hConsole, coordScreen);
}

void set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// 정사각형 바둑판 그리기 (19x19)
void draw_board(void) {
    set_color(11); // 밝은 청록색
    
    // 상단
    printf("┏");
    for(int i = 0; i < 18; i++) printf("━━┳");
    printf("━━┓\n");
    
    // 중간 줄들
    for(int row = 0; row < 18; row++) {
        // 돌 놓는 줄
        printf("┃");
        for(int i = 0; i < 19; i++) printf("  ┃");
        printf("\n");
        
        // 구분선
        if(row < 17) {
            printf("┣");
            for(int i = 0; i < 18; i++) printf("━━╋");
            printf("━━┫\n");
        }
    }
    
    // 마지막 돌 놓는 줄
    printf("┃");
    for(int i = 0; i < 19; i++) printf("  ┃");
    printf("\n");
    
    // 하단
    printf("┗");
    for(int i = 0; i < 18; i++) printf("━━┻");
    printf("━━┛\n");
    
    set_color(7);
}

// 화려한 인트로 애니메이션
void draw_intro_screen(int selected) {
    static int frame = 0;
    frame++;
    
    // 배경 효과
    for(int i = 0; i < 5; i++) {
        int x = 10 + (frame + i * 15) % 70;
        int y = 2 + i;
        gotoxy(x, y);
        set_color(8 + (frame / 10 + i) % 8);
        printf("*");
    }
    
    set_color(15);
    
    // 타이틀
    gotoxy(25, 6);
    set_color(11);
    printf("╔═══════════════════════════════════╗");
    gotoxy(25, 7);
    printf("║");
    gotoxy(59, 7);
    printf("║");
    
    gotoxy(25, 8);
    printf("║");
    set_color(14);
    gotoxy(35, 8);
    printf("◆◆ ");
    set_color(15);
    printf("오  목");
    set_color(14);
    printf(" ◆◆");
    set_color(11);
    gotoxy(59, 8);
    printf("║");
    
    gotoxy(25, 9);
    printf("║");
    gotoxy(59, 9);
    printf("║");
    gotoxy(25, 10);
    printf("╚═══════════════════════════════════╝");
    
    set_color(8);
    gotoxy(35, 11);
    printf("~ Five in a Row ~");
    
    // 메뉴
    const char* menu[] = {
        "🎮 vs 컴퓨터",
        "👥 2인 게임",
        "⚙️  설정",
        "🚪 게임 종료"
    };
    
    for(int i = 0; i < 4; i++) {
        gotoxy(32, 14 + i * 3);
        
        if(i == selected) {
            set_color(15 | (14 << 4));
            printf("  ► %s ◄  ", menu[i]);
            set_color(7);
        } else {
            set_color(7);
            printf("    %s    ", menu[i]);
        }
    }
    
    set_color(8);
    gotoxy(25, 28);
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    gotoxy(28, 29);
    set_color(11);
    printf("↑↓");
    set_color(7);
    printf(": 이동  ");
    set_color(11);
    printf("Enter");
    set_color(7);
    printf(": 선택  ");
    set_color(11);
    printf("ESC");
    set_color(7);
    printf(": 종료");
    
    set_color(7);
}

void draw_difficulty_screen(int selected) {
    set_color(15);
    
    gotoxy(28, 8);
    set_color(11);
    printf("╔═══════════════════════════════╗");
    gotoxy(28, 9);
    printf("║");
    set_color(14);
    gotoxy(35, 9);
    printf("난이도를 선택하세요");
    set_color(11);
    gotoxy(60, 9);
    printf("║");
    gotoxy(28, 10);
    printf("╚═══════════════════════════════╝");
    
    const char* levels[] = {
        "😊 초급 - 랜덤 플레이",
        "🤔 중급 - 수비 전략",
        "😈 고급 - 공격+수비"
    };
    
    for(int i = 0; i < 3; i++) {
        gotoxy(30, 13 + i * 3);
        
        if(i == selected) {
            set_color(15 | (12 << 4));
            printf("  【 %s 】  ", levels[i]);
            set_color(7);
        } else {
            set_color(7);
            printf("     %s     ", levels[i]);
        }
    }
    
    set_color(8);
    gotoxy(32, 23);
    printf("ESC: 돌아가기");
    set_color(7);
}

// 완전히 새로운 게임 화면 그리기 방식
void draw_game_screen(GameState* state, int cursor_x, int cursor_y) {
    // 커서를 숨겨서 깜빡임 방지
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    
    // 화면 전체를 매번 다시 그립니다
    gotoxy(1, 1);
    
    // 바둑판 그리기
    set_color(11);
    
    // 상단
    printf("┏");
    for(int i = 0; i < 18; i++) printf("━━┳");
    printf("━━┓");
    
    // 정보 패널 시작
    printf("    ");
    set_color(11);
    printf("┏━━━━━━━━━━━━━━━━━━━━┓\n");
    
    // 각 줄마다 바둑판 + 정보 패널
    for(int row = 0; row < 19; row++) {
        // 돌 놓는 줄
        set_color(11);
        printf("┃");
        
        for(int col = 0; col < 19; col++) {
            // 돌 먼저 체크 (최우선)
            if(state->board[row][col] == 1) {
                set_color(0);
                printf("● ");
                set_color(11);
            }
            else if(state->board[row][col] == 2) {
                set_color(15);
                printf("● ");
                set_color(11);
            }
            // 커서 위치 (돌이 없을 때만)
            else if(cursor_x - 1 == col && cursor_y - 1 == row) {
                set_color(13);
                printf("◆ ");
                set_color(11);
            }
            // 빈 칸
            else {
                printf("  ");
            }
            // 세로선 (항상)
            printf("┃");
        }
        
        // 정보 패널 내용
        printf("    ");
        set_color(11);
        
        if(row == 0) {
            printf("┃                      ┃");
        }
        else if(row == 1) {
            printf("┃");
            set_color(15);
            if(state->current_player == PLAYER_BLACK) {
                printf("  현재 턴: ");
                set_color(0 | (15 << 4));
                printf("●흑돌");
                set_color(15);
                printf("  ");
            } else {
                printf("  현재 턴: ");
                set_color(15 | (0 << 4));
                printf("●백돌");
                set_color(15);
                printf("  ");
            }
            set_color(11);
            printf("┃");
        }
        else if(row == 2) {
            printf("┣━━━━━━━━━━━━━━━━━━━━┫");
        }
        else if(row == 3) {
            printf("┃");
            set_color(15);
            printf("  ⏱  타이머: ");
            int time = state->time_left[state->current_player];
            if(time <= 10) set_color(12);
            else if(time <= 20) set_color(14);
            else set_color(10);
            printf("%2d초  ", time);
            set_color(11);
            printf("┃");
        }
        else if(row == 4) {
            printf("┗━━━━━━━━━━━━━━━━━━━━┛");
        }
        else if(row == 6) {
            set_color(8);
            printf("┌──────────────────┐");
        }
        else if(row == 7) {
            set_color(8);
            printf("│  조작법          │");
        }
        else if(row == 8) {
            set_color(8);
            printf("├──────────────────┤");
        }
        else if(row == 9) {
            set_color(11);
            printf("│ ↑↓←→");
            set_color(8);
            printf(": 이동     │");
        }
        else if(row == 10) {
            set_color(14);
            printf("│ Space");
            set_color(8);
            printf(": 착수      │");
        }
        else if(row == 11) {
            set_color(12);
            printf("│ ESC");
            set_color(8);
            printf(": 나가기      │");
        }
        else if(row == 12) {
            set_color(8);
            printf("└──────────────────┘");
        }
        else {
            printf("                      ");
        }
        
        printf("\n");
        
        // 구분선
        if(row < 18) {
            set_color(11);
            printf("┣");
            for(int i = 0; i < 18; i++) printf("━━╋");
            printf("━━┫\n");
        }
    }
    
    // 하단
    set_color(11);
    printf("┗");
    for(int i = 0; i < 18; i++) printf("━━┻");
    printf("━━┛");
    
    // 남은 공간 지우기 (깜빡임 방지)
    printf("                      \n");
    
    set_color(7);
}

// AI 생각 중 메시지 표시 (오목판 밖에)
void show_ai_thinking(void) {
    gotoxy(45, 15);
    set_color(11);
    printf("┌──────────────────┐");
    gotoxy(45, 16);
    printf("│                  │");
    gotoxy(45, 17);
    set_color(14);
    printf("│ 💭 컴퓨터가    │");
    gotoxy(45, 18);
    printf("│   생각 중...     │");
    gotoxy(45, 19);
    set_color(11);
    printf("│                  │");
    gotoxy(45, 20);
    printf("└──────────────────┘");
    set_color(7);
}

// AI 생각 중 메시지 지우기
void clear_ai_thinking(void) {
    for(int y = 15; y <= 20; y++) {
        gotoxy(45, y);
        printf("                      ");
    }
}

void draw_settings_screen(int volume) {
    set_color(11);
    gotoxy(28, 8);
    printf("╔═══════════════════════════════╗");
    gotoxy(28, 9);
    printf("║");
    set_color(14);
    gotoxy(40, 9);
    printf("설  정");
    set_color(11);
    gotoxy(60, 9);
    printf("║");
    gotoxy(28, 10);
    printf("╚═══════════════════════════════╝");
    
    set_color(15);
    gotoxy(28, 13);
    printf("🔊 음량 조절:");
    
    gotoxy(28, 15);
    set_color(14);
    printf("◀ ");
    set_color(11);
    printf("[");
    
    for(int i = 0; i < 10; i++) {
        if(i < volume / 10) {
            set_color(10);
            printf("█");
        } else {
            set_color(8);
            printf("░");
        }
    }
    
    set_color(11);
    printf("]");
    set_color(14);
    printf(" ▶");
    
    set_color(15);
    gotoxy(28, 17);
    printf("현재 음량: ");
    set_color(14);
    printf("%d%%", volume);
    
    set_color(8);
    gotoxy(28, 20);
    printf("←/→ : 음량 조절 | Enter: 완료");
    set_color(7);
}

void draw_game_over_menu(int selected) {
    set_color(15);
    gotoxy(32, 10);
    printf("┏━━━━━━━━━━━━━━━━━━━━┓");
    gotoxy(32, 11);
    printf("┃");
    gotoxy(37, 11);
    printf("게임이 종료되었습니다");
    gotoxy(53, 11);
    printf("┃");
    gotoxy(32, 12);
    printf("┗━━━━━━━━━━━━━━━━━━━━┛");
    
    gotoxy(36, 14);
    if(selected == 0) {
        set_color(15 | (10 << 4));
        printf(" 🔄 다시하기 ");
    } else {
        set_color(7);
        printf("   🔄 다시하기   ");
    }
    
    gotoxy(36, 16);
    if(selected == 1) {
        set_color(15 | (9 << 4));
        printf(" 🏠 메인메뉴 ");
    } else {
        set_color(7);
        printf("   🏠 메인메뉴   ");
    }
    
    set_color(7);
}

void reset_game_screen(void) {
    // 더 이상 필요 없음
}