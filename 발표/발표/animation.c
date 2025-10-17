// animation.c - 화려한 애니메이션
#include "animation.h"
#include "ui.h"
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>

void set_color(int color);

// 타이머 초과 애니메이션 - 깜빡임 제거!
void show_timeout_animation(void) {
    // 화면 한 번만 클리어
    clear_screen();
    
    // 시계 흔들리는 애니메이션 (배경 고정)
    for(int shake = 0; shake < 8; shake++) {
        int offset_x = (shake % 2) ? 2 : -2;
        
        // 이전 위치 지우기
        if(shake > 0) {
            int prev_offset = (shake % 2) ? -2 : 2;
            gotoxy(32 + prev_offset, 8);
            printf("                               ");
            gotoxy(32 + prev_offset, 9);
            printf("                               ");
            gotoxy(32 + prev_offset, 10);
            printf("                               ");
            gotoxy(32 + prev_offset, 11);
            printf("                               ");
            gotoxy(32 + prev_offset, 12);
            printf("                               ");
            gotoxy(32 + prev_offset, 13);
            printf("                               ");
            gotoxy(32 + prev_offset, 14);
            printf("                               ");
        }
        
        // 큰 경고 박스
        gotoxy(32 + offset_x, 8);
        set_color(12); // 빨강
        printf("╔═══════════════════════════╗");
        gotoxy(32 + offset_x, 9);
        printf("║                           ║");
        gotoxy(32 + offset_x, 10);
        printf("║                           ║");
        gotoxy(32 + offset_x, 11);
        set_color(15); // 흰색
        printf("║     ⏰ TIME UP! ⏰       ║");
        gotoxy(32 + offset_x, 12);
        set_color(12);
        printf("║                           ║");
        gotoxy(32 + offset_x, 13);
        printf("║                           ║");
        gotoxy(32 + offset_x, 14);
        printf("╚═══════════════════════════╝");
        
        // 주변 경고 표시 (고정)
        if(shake == 0) {
            for(int i = 0; i < 8; i++) {
                int x = 15 + i * 10;
                gotoxy(x, 6);
                set_color(14);
                printf("⚠");
                gotoxy(x, 16);
                printf("⚠");
            }
        }
        
        set_color(7);
        Sleep(200);
    }
    
    Sleep(500); // 마지막 여운
    set_color(7);
}

// 폭죽 입자 구조체
typedef struct {
    int x, y;
    int dx, dy;
    int life;
    int color;
} Particle;

// 승리 애니메이션 - 화려한 폭죽
void show_victory_animation(void) {
    clear_screen();
    
    Particle particles[100];
    int num_particles = 100;
    
    // 폭죽 초기화
    for(int i = 0; i < num_particles; i++) {
        particles[i].x = 40 * 10; // 중앙 (고정소수점)
        particles[i].y = 12 * 10;
        
        double angle = (double)i / num_particles * 6.28;
        double speed = 5.0 + (rand() % 30) / 10.0;
        
        particles[i].dx = (int)(cos(angle) * speed * 10);
        particles[i].dy = (int)(sin(angle) * speed * 10);
        particles[i].life = 20 + rand() % 10;
        particles[i].color = 9 + rand() % 6; // 밝은 색상들
    }
    
    // 폭죽 애니메이션
    for(int frame = 0; frame < 30; frame++) {
        // 이전 프레임 지우기 (일부만)
        if(frame > 0) {
            for(int i = 0; i < num_particles; i++) {
                if(particles[i].life > 0) {
                    gotoxy(particles[i].x / 10, particles[i].y / 10);
                    printf(" ");
                }
            }
        }
        
        // 파티클 업데이트 및 그리기
        for(int i = 0; i < num_particles; i++) {
            if(particles[i].life > 0) {
                particles[i].x += particles[i].dx;
                particles[i].y += particles[i].dy;
                particles[i].dy += 3; // 중력
                particles[i].life--;
                
                int x = particles[i].x / 10;
                int y = particles[i].y / 10;
                
                if(x >= 1 && x <= 80 && y >= 1 && y <= 30) {
                    gotoxy(x, y);
                    set_color(particles[i].color);
                    if(particles[i].life > 15) printf("★");
                    else if(particles[i].life > 10) printf("*");
                    else if(particles[i].life > 5) printf("·");
                    else printf(".");
                }
            }
        }
        
        Sleep(50);
    }
    
    // 승리 텍스트 - 부드러운 등장
    clear_screen();
    
    // 황금빛 테두리
    set_color(14); // 노란색
    gotoxy(28, 12);
    printf("╔═══════════════════════════╗");
    gotoxy(28, 13);
    printf("║                           ║");
    gotoxy(28, 14);
    printf("║");
    set_color(15); // 흰색
    printf("    ★★ 승  리 ★★    ");
    set_color(14);
    printf("║");
    gotoxy(28, 15);
    printf("║                           ║");
    gotoxy(28, 16);
    printf("║");
    set_color(11); // 밝은 청록
    printf("   🎉 VICTORY! 🎉         ");
    set_color(14);
    printf("║");
    gotoxy(28, 17);
    printf("║                           ║");
    gotoxy(28, 18);
    printf("╚═══════════════════════════╝");
    
    // 반짝이는 별 효과 (주변에만)
    for(int i = 0; i < 20; i++) {
        int sx = 20 + rand() % 40;
        int sy = 10 + rand() % 10;
        gotoxy(sx, sy);
        set_color(14 + rand() % 2);
        printf("✦");
        Sleep(100);
    }
    
    set_color(7);
    Sleep(2000);
}

// 패배 애니메이션 - Matrix 스타일
void show_defeat_animation(void) {
    clear_screen();
    
    // Matrix 비 효과
    int columns[80];
    for(int i = 0; i < 80; i++) {
        columns[i] = rand() % 30;
    }
    
    for(int frame = 0; frame < 40; frame++) {
        for(int x = 1; x <= 80; x++) {
            columns[x-1]++;
            if(columns[x-1] > 30) columns[x-1] = 0;
            
            int y = columns[x-1];
            if(y > 0 && y <= 30) {
                gotoxy(x, y);
                set_color(12); // 빨강
                printf("█");
                
                if(y > 1) {
                    gotoxy(x, y - 1);
                    set_color(4); // 어두운 빨강
                    printf("▓");
                }
                if(y > 2) {
                    gotoxy(x, y - 2);
                    set_color(0);
                    printf(" ");
                }
            }
        }
        Sleep(50);
    }
    
    // 패배 텍스트 - 깨지는 효과
    clear_screen();
    
    char* text[] = {
        "╔═══════════════════════════╗",
        "║                           ║",
        "║        패    배           ║",
        "║                           ║",
        "║      DEFEAT...            ║",
        "║                           ║",
        "╚═══════════════════════════╝"
    };
    
    // 글자가 위에서 떨어지면서 조립됨
    for(int phase = 0; phase < 20; phase++) {
        for(int line = 0; line < 7; line++) {
            int target_y = 12 + line;
            int current_y = target_y - 20 + phase;
            
            if(current_y >= 1 && current_y <= target_y) {
                gotoxy(28, current_y);
                set_color(12 + (phase % 4));
                printf("%s", text[line]);
            }
        }
        Sleep(100);
    }
    
    // 깜빡이는 효과
    for(int i = 0; i < 4; i++) {
        Sleep(300);
        for(int line = 0; line < 7; line++) {
            gotoxy(28, 12 + line);
            if(i % 2 == 0) {
                set_color(0);
            } else {
                set_color(12);
            }
            printf("%s", text[line]);
        }
    }
    
    set_color(7);
    Sleep(1500);
}

// 흑돌 승리 - 파도 효과
void show_black_win_animation(void) {
    clear_screen();
    
    // 파도 애니메이션
    for(int wave = 0; wave < 3; wave++) {
        for(int phase = 0; phase < 20; phase++) {
            for(int y = 5; y < 20; y++) {
                for(int x = 10; x < 70; x++) {
                    gotoxy(x, y);
                    
                    double dist = sqrt((x - 40) * (x - 40) + (y - 12) * (y - 12));
                    if(fabs(dist - phase * 2) < 2) {
                        set_color(0 | (8 << 4)); // 검은색 + 회색 배경
                        printf("●");
                    } else {
                        printf(" ");
                    }
                }
            }
            Sleep(50);
        }
    }
    
    clear_screen();
    
    // 깃발 애니메이션 - 더 화려하게
    for(int frame = 0; frame < 10; frame++) {
        clear_screen();
        
        int wave_offset = (frame % 4);
        
        // 깃대
        set_color(8);
        for(int y = 5; y < 20; y++) {
            gotoxy(38, y);
            printf("║");
            gotoxy(39, y);
            printf("║");
        }
        
        // 펄럭이는 깃발
        for(int y = 0; y < 6; y++) {
            gotoxy(40, 7 + y);
            
            int offset = (wave_offset + y) % 4;
            if(offset == 0) set_color(0 | (8 << 4));
            else if(offset == 1) set_color(8 | (0 << 4));
            else if(offset == 2) set_color(0 | (7 << 4));
            else set_color(7 | (0 << 4));
            
            if(y % 2 == 0) {
                if(frame % 2 == 0) printf("●●●●●●●");
                else printf(" ●●●●●●●");
            } else {
                if(frame % 2 == 0) printf(" ●●●●●●●");
                else printf("●●●●●●●");
            }
        }
        
        // 반짝이는 효과
        for(int i = 0; i < 10; i++) {
            int sx = 20 + rand() % 40;
            int sy = 5 + rand() % 15;
            gotoxy(sx, sy);
            set_color(15);
            if(rand() % 2) printf("✦");
            else printf("✧");
        }
        
        Sleep(200);
    }
    
    // 승리 텍스트
    gotoxy(26, 20);
    set_color(15 | (0 << 4));
    printf("╔═══════════════════════════════╗");
    gotoxy(26, 21);
    printf("║                               ║");
    gotoxy(26, 22);
    printf("║     ●● 흑돌 승리 ●●       ║");
    gotoxy(26, 23);
    printf("║                               ║");
    gotoxy(26, 24);
    printf("║      BLACK WINS! 🏆          ║");
    gotoxy(26, 25);
    printf("║                               ║");
    gotoxy(26, 26);
    printf("╚═══════════════════════════════╝");
    
    set_color(7);
    Sleep(2000);
}

// 백돌 승리 - 빛 효과
void show_white_win_animation(void) {
    clear_screen();
    
    // 빛 퍼지는 효과
    for(int radius = 0; radius < 30; radius++) {
        for(int y = 5; y < 25; y++) {
            for(int x = 10; x < 70; x++) {
                gotoxy(x, y);
                
                double dist = sqrt((x - 40) * (x - 40) + (y - 15) * (y - 15));
                if(dist <= radius && dist > radius - 3) {
                    int brightness = 15 - (int)(dist / 3);
                    if(brightness < 7) brightness = 7;
                    set_color(brightness);
                    printf("○");
                } else if(dist <= radius - 3) {
                    set_color(15);
                    printf("○");
                }
            }
        }
        Sleep(30);
    }
    
    Sleep(500);
    clear_screen();
    
    // 깃발 애니메이션
    for(int frame = 0; frame < 10; frame++) {
        clear_screen();
        
        int wave_offset = (frame % 4);
        
        // 깃대
        set_color(14);
        for(int y = 5; y < 20; y++) {
            gotoxy(38, y);
            printf("║");
            gotoxy(39, y);
            printf("║");
        }
        
        // 펄럭이는 백색 깃발
        for(int y = 0; y < 6; y++) {
            gotoxy(40, 7 + y);
            
            int offset = (wave_offset + y) % 4;
            if(offset < 2) set_color(15 | (7 << 4));
            else set_color(7 | (15 << 4));
            
            if(y % 2 == 0) {
                if(frame % 2 == 0) printf("○○○○○○○");
                else printf(" ○○○○○○○");
            } else {
                if(frame % 2 == 0) printf(" ○○○○○○○");
                else printf("○○○○○○○");
            }
        }
        
        // 반짝이는 별
        for(int i = 0; i < 15; i++) {
            int sx = 20 + rand() % 40;
            int sy = 5 + rand() % 15;
            gotoxy(sx, sy);
            set_color(14 + rand() % 2);
            if(rand() % 3 == 0) printf("★");
            else printf("☆");
        }
        
        Sleep(200);
    }
    
    // 승리 텍스트
    gotoxy(26, 20);
    set_color(0 | (15 << 4)); // 검은글씨 + 흰배경
    printf("╔═══════════════════════════════╗");
    gotoxy(26, 21);
    printf("║                               ║");
    gotoxy(26, 22);
    printf("║     ○○ 백돌 승리 ○○       ║");
    gotoxy(26, 23);
    printf("║                               ║");
    gotoxy(26, 24);
    printf("║      WHITE WINS! 👑          ║");
    gotoxy(26, 25);
    printf("║                               ║");
    gotoxy(26, 26);
    printf("╚═══════════════════════════════╝");
    
    set_color(7);
    Sleep(2000);
}