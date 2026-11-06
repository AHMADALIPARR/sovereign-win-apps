/* terminal - Windows-style command terminal for the Sovereign OS (win32sov SDK).
   Type a command, Enter (key 13) runs it. Built-ins: help, ver, echo,
   add <a> <b>, clear. Backspace is key 8. */
#include "win32sov.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static char line[256]; static int llen;
static char hist[64][160]; static int hcount;

static void emit(const char *s) {
    if (hcount < 64) { snprintf(hist[hcount], sizeof hist[0], "%s", s); hcount++; }
    SovTrace("TERM %s", s);
}
static void run_line(void) {
    char cmd[256]; snprintf(cmd, sizeof cmd, "%s", line);
    char out[256];
    if (strcmp(cmd, "help") == 0) {
        emit("commands: help ver echo <text> add <a> <b> clear");
    } else if (strcmp(cmd, "ver") == 0) {
        emit("SOVEREIGN WIN32SOV 1.0");
    } else if (strncmp(cmd, "echo ", 5) == 0) {
        emit(cmd + 5);
    } else if (strncmp(cmd, "add ", 4) == 0) {
        long a, b; char extra;
        if (sscanf(cmd + 4, "%ld %ld %c", &a, &b, &extra) == 2) {
            snprintf(out, sizeof out, "RESULT %ld", a + b); emit(out);
        } else emit("ERROR add needs two integers");
    } else if (strcmp(cmd, "clear") == 0) {
        hcount = 0; SovTrace("TERM cleared");
    } else if (cmd[0] == 0) {
        /* empty line: nothing to run */
    } else {
        snprintf(out, sizeof out, "ERROR unknown command: %.200s", cmd); emit(out);
    }
    llen = 0; line[0] = 0;
}
static void draw(HWND hwnd) {
    HDC dc = BeginPaint(hwnd);
    for (int i = 0; i < hcount; i++) TextOut(dc, 8, 8 + i * 12, hist[i]);
    char prompt[300]; snprintf(prompt, sizeof prompt, "> %s", line);
    TextOut(dc, 8, 8 + hcount * 12, prompt);
    EndPaint(hwnd, dc);
}
static long WndProc(HWND hwnd, unsigned msg, long wp, long lp) {
    (void)lp;
    if (msg == WM_CHAR) {
        char c = (char)wp;
        if (isprint((unsigned char)c) && llen < 250) { line[llen++] = c; line[llen] = 0; }
    } else if (msg == WM_KEYDOWN) {
        if (wp == 13) run_line();
        else if (wp == 8 && llen > 0) line[--llen] = 0;
    } else if (msg == WM_PAINT) {
        draw(hwnd);
    } else if (msg == WM_DESTROY) {
        draw(hwnd);
        SovTrace("TERMINAL-END lines=%d last=%s", hcount, hcount ? hist[hcount - 1] : "(none)");
    }
    return 0;
}
int SovWinMain(void) {
    CreateMainWindow("Terminal", 480, 320, WndProc);
    return SovMessageLoop();
}
