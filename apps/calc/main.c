/* calc - Windows-style calculator for the Sovereign OS (win32sov SDK). */
#include "win32sov.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static char disp[64] = "0";
static double acc; static char pending; static int fresh = 1;

static void show(void) { SovTrace("DISPLAY %s", disp); }
static void apply_pending(void) {
    double v = strtod(disp, NULL);
    switch (pending) {
        case '+': acc += v; break; case '-': acc -= v; break;
        case '*': acc *= v; break; case '/': acc = v != 0 ? acc / v : 0; break;
        default: acc = v; break;
    }
    pending = 0;
    snprintf(disp, sizeof disp, "%g", acc);
}
static long WndProc(HWND hwnd, unsigned msg, long wp, long lp) {
    (void)hwnd; (void)lp;
    if (msg == WM_CHAR) {
        char c = (char)wp;
        if ((c >= '0' && c <= '9') || c == '.') {
            if (fresh || strcmp(disp, "0") == 0) { disp[0] = 0; fresh = 0; }
            size_t n = strlen(disp);
            if (n < 40) { disp[n] = c; disp[n + 1] = 0; }
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            if (pending) apply_pending();
            else acc = strtod(disp, NULL);
            pending = c; fresh = 1;
        } else if (c == '=' || c == '\r') { if (pending) apply_pending(); fresh = 1; show(); }
        else if (c == 'C' || c == 'c') { strcpy(disp, "0"); acc = 0; pending = 0; fresh = 1; }
    }
    if (msg == WM_DESTROY) SovTrace("CALC-END %s", disp);
    return 0;
}
int SovWinMain(void) {
    HWND w = CreateMainWindow("Calculator", 240, 320, WndProc);
    (void)w;
    return SovMessageLoop();
}
