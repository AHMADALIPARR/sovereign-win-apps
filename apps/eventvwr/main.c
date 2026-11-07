/* eventvwr - Windows-style Event Viewer for the Sovereign OS (win32sov SDK).
   Fixed event list; arrows select; traces selection. */
#include "win32sov.h"
#include <stdio.h>

static const char *events[] = {
    "Info: System started",
    "Warning: Low disk space",
    "Error: Service failed",
    "Info: User logon",
    "Warning: Update available"
};
static int n = 5, sel = 0;
static void show(void) { SovTrace("EVENT %d %s", sel, events[sel]); }
static long WndProc(HWND hwnd, unsigned msg, long wp, long lp) {
    (void)hwnd; (void)lp;
    if (msg == WM_KEYDOWN) {
        if (wp == 38 && sel > 0) sel--;
        else if (wp == 40 && sel < n - 1) sel++;
        show();
    } else if (msg == WM_DESTROY) {
        SovTrace("EVENTVWR-END selected=%d", sel);
    }
    return 0;
}
int SovWinMain(void) {
    CreateMainWindow("Event Viewer", 400, 300, WndProc);
    show();
    return SovMessageLoop();
}
