/* charmap - Windows-style Character Map for the Sovereign OS (win32sov SDK).
   Shows printable ASCII; arrows or clicks select; traces selected codepoint. */
#include "win32sov.h"
#include <stdio.h>
#include <string.h>

static int sel = 32;
static void show(void) {
    char c = (char)sel;
    SovTrace("CHARMAP selected=%d '%c'", sel, (c >= 32 && c < 127) ? c : '?');
}
static long WndProc(HWND hwnd, unsigned msg, long wp, long lp) {
    (void)hwnd; (void)lp;
    if (msg == WM_KEYDOWN) {
        if (wp == 37 && sel > 32) sel--;
        else if (wp == 39 && sel < 126) sel++;
        else if (wp == 38 && sel > 41) sel -= 10;
        else if (wp == 40 && sel < 117) sel += 10;
        show();
    } else if (msg == WM_LBUTTONDOWN) {
        int x = lp & 0xFFFF, y = (lp >> 16) & 0xFFFF;
        int col = x / 20, row = y / 20;
        int n = 32 + row * 10 + col;
        if (n >= 32 && n <= 126) { sel = n; show(); }
    } else if (msg == WM_DESTROY) {
        SovTrace("CHARMAP-END selected=%d", sel);
    }
    return 0;
}
int SovWinMain(void) {
    CreateMainWindow("Character Map", 220, 200, WndProc);
    show();
    return SovMessageLoop();
}
