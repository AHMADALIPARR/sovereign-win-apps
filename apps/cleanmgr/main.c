/* cleanmgr - Windows-style Disk Cleanup for the Sovereign OS (win32sov SDK).
   Categories with sizes; arrows select, 'c' cleans selected. */
#include "win32sov.h"
#include <stdio.h>

static struct { const char *name; int mb; int cleaned; } cats[] = {
    {"Temporary files", 120, 0},
    {"Recycle Bin", 45, 0},
    {"Thumbnails", 30, 0},
    {"Windows Update", 800, 0}
};
static int n = 4, sel = 0, total = 0;
static void show(void) {
    SovTrace("CLEAN selected=%s mb=%d cleaned=%d total=%d", cats[sel].name, cats[sel].mb, cats[sel].cleaned, total);
}
static long WndProc(HWND hwnd, unsigned msg, long wp, long lp) {
    (void)hwnd; (void)lp;
    if (msg == WM_KEYDOWN) {
        if (wp == 38 && sel > 0) sel--;
        else if (wp == 40 && sel < n - 1) sel++;
        show();
    } else if (msg == WM_CHAR && (wp == 'c' || wp == 'C') && !cats[sel].cleaned) {
        cats[sel].cleaned = 1;
        total += cats[sel].mb;
        show();
    } else if (msg == WM_DESTROY) {
        SovTrace("CLEANMGR-END total_cleaned=%d", total);
    }
    return 0;
}
int SovWinMain(void) {
    CreateMainWindow("Disk Cleanup", 360, 240, WndProc);
    show();
    return SovMessageLoop();
}
