#ifndef UTILS_H
#define UTILS_H
#include <windows.h>
int  clampi(int v, int lo, int hi);
int  kbhit_nonblock(void);
int  get_arrow_key(void);  // 72/75/77/80 or -1
void seed_rand(void);
DWORD now_ms(void);
#endif
