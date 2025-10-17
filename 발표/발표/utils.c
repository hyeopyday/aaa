#include "utils.h"
#include <conio.h>
#include <time.h>

int clampi(int v,int lo,int hi){ if(v<lo)return lo; if(v>hi)return hi; return v; }
int kbhit_nonblock(void){ return kbhit(); }
int get_arrow_key(void){
    if(!kbhit()) return -1;
    int ch=getch();
    if(ch==224){
        ch=getch();
        if(ch==72||ch==75||ch==77||ch==80) return ch;
    }
    return ch;
}
void seed_rand(void){ srand((unsigned)time(NULL)); }
DWORD now_ms(void){ return GetTickCount(); }
