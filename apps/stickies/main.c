/* stickies - Windows-style Sticky Notes for the Sovereign OS (win32sov SDK).
   In-memory note; typing + backspace; traces content on exit. */
#include "win32sov.h"
#include <stdio.h>
#include <string.h>

static char note[1024]; static size_t len;
static long WndProc(HWND hwnd, unsigned msg, long wp, long lp) {
    (void)hwnd; (void)lp;
    if (msg == WM_CHAR) {
        char c = (char)wp;
        if ((c >= 32 || c == '\n') && len < sizeof note - 1) note[len++] = c;
    } else if (msg == WM_KEYDOWN && wp == VK_BACK) {
        if (len) len--;
    } else if (msg == WM_DESTROY) {
        note[len] = 0;
        SovTrace("STICKIES-END len=%zu", len);
        SovTrace("NOTE %s", note);
    }
    return 0;
}
int SovWinMain(void) {
    CreateMainWindow("Sticky Notes", 300, 200, WndProc);
    return SovMessageLoop();
}
