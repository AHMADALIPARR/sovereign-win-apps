/* win32sov.c - headless backend for the win32sov SDK.
   Script format (argv[1], "-" for stdin): one event per line:
     char X | line <text+Enter> | key N | click X Y | move X Y | timer | paint | quit
   After the script ends the window receives WM_DESTROY and the loop ends. */
#include "win32sov.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

struct SWnd { int w, h; WNDPROC proc; COLORREF *px; char title[128]; };
struct SDC { HWND wnd; int cx, cy; COLORREF pen; };
static HWND g_wnd;
static int g_quit = -1;
static int g_argc; static char **g_argv;

void SovTrace(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt); vprintf(fmt, ap); va_end(ap); putchar('\n'); fflush(stdout);
}
int SovArgCount(void) { return g_argc; }
const char *SovArg(int i) { return (i >= 0 && i < g_argc) ? g_argv[i] : ""; }

HWND CreateMainWindow(const char *title, int width, int height, WNDPROC proc) {
    HWND w = calloc(1, sizeof *w);
    w->w = width; w->h = height; w->proc = proc;
    w->px = calloc((size_t)width * height, sizeof(COLORREF));
    snprintf(w->title, sizeof w->title, "%s", title);
    g_wnd = w;
    proc(w, WM_CREATE, 0, 0);
    return w;
}
void PostQuitMessage(int code) { g_quit = code; }
void InvalidateRect(HWND hwnd) { if (hwnd) hwnd->proc(hwnd, WM_PAINT, 0, 0); }
HDC BeginPaint(HWND hwnd) { HDC dc = calloc(1, sizeof *dc); dc->wnd = hwnd; dc->pen = RGB(255,255,255); return dc; }
void EndPaint(HWND hwnd, HDC dc) { (void)hwnd; free(dc); }
static void plot(HDC dc, int x, int y) {
    if (x < 0 || y < 0 || x >= dc->wnd->w || y >= dc->wnd->h) return;
    dc->wnd->px[(size_t)y * dc->wnd->w + x] = dc->pen;
}
void SetPixel(HDC dc, int x, int y, COLORREF c) { COLORREF old = dc->pen; dc->pen = c; plot(dc, x, y); dc->pen = old; }
void MoveToEx(HDC dc, int x, int y) { dc->cx = x; dc->cy = y; }
void LineTo(HDC dc, int x, int y) {
    int x0 = dc->cx, y0 = dc->cy, dx = abs(x - x0), dy = -abs(y - y0);
    int sx = x0 < x ? 1 : -1, sy = y0 < y ? 1 : -1, err = dx + dy;
    for (;;) { plot(dc, x0, y0); if (x0 == x && y0 == y) break;
        int e2 = 2 * err; if (e2 >= dy) { err += dy; x0 += sx; } if (e2 <= dx) { err += dx; y0 += sy; } }
    dc->cx = x; dc->cy = y;
}
void Rectangle(HDC dc, int l, int t, int r, int b) {
    MoveToEx(dc, l, t); LineTo(dc, r, t); LineTo(dc, r, b); LineTo(dc, l, b); LineTo(dc, l, t);
}
void FillRect(HDC dc, int l, int t, int r, int b, COLORREF c) {
    COLORREF old = dc->pen; dc->pen = c;
    for (int y = t; y < b; y++) for (int x = l; x < r; x++) plot(dc, x, y);
    dc->pen = old;
}
void Ellipse(HDC dc, int l, int t, int r, int b) {
    double cx = (l + r) / 2.0, cy = (t + b) / 2.0, rx = (r - l) / 2.0, ry = (b - t) / 2.0;
    if (rx <= 0 || ry <= 0) return;
    for (int a = 0; a < 720; a++) {
        double th = a * 3.141592653589793 / 360.0;
        plot(dc, (int)(cx + rx * __builtin_cos(th)), (int)(cy + ry * __builtin_sin(th)));
    }
}
void TextOut(HDC dc, int x, int y, const char *text) {
    /* 3x5 micro-font would lie about legibility; the canvas records the
       string's footprint as a ruled line so painting is observable. */
    int n = (int)strlen(text);
    for (int i = 0; i < n * 4 - 1; i++) plot(dc, x + i, y);
    (void)0;
}
uint64_t SovCanvasHash(HWND hwnd) {
    uint64_t h = 1469598103934665603ULL;
    size_t n = (size_t)hwnd->w * hwnd->h;
    for (size_t i = 0; i < n; i++) { h ^= hwnd->px[i]; h *= 1099511628211ULL; }
    return h;
}
void SovNow(int *hh, int *mm, int *ss) {
    const char *t = getenv("SOV_TIME");
    int H = 0, M = 0, S = 0;
    if (t) sscanf(t, "%d:%d:%d", &H, &M, &S);
    *hh = H; *mm = M; *ss = S;
}
static void dispatch(unsigned msg, long wp, long lp) { if (g_wnd) g_wnd->proc(g_wnd, msg, wp, lp); }
int SovMessageLoop(void) {
    const char *path = g_argc > 1 ? g_argv[1] : "-";
    FILE *f = strcmp(path, "-") == 0 ? stdin : fopen(path, "r");
    if (!f) { fprintf(stderr, "cannot open script %s\n", path); return 2; }
    char line[512];
    while (g_quit < 0 && fgets(line, sizeof line, f)) {
        char ev[32]; long a = 0, b = 0;
        if (strncmp(line, "line ", 5) == 0) {
            for (char *p = line + 5; *p && *p != '\n'; p++) dispatch(WM_CHAR, (unsigned char)*p, 0);
            dispatch(WM_KEYDOWN, 13, 0);
        }
        else if (sscanf(line, "char %c", ev) == 1) dispatch(WM_CHAR, (unsigned char)ev[0], 0);
        else if (sscanf(line, "key %ld", &a) == 1) dispatch(WM_KEYDOWN, a, 0);
        else if (sscanf(line, "click %ld %ld", &a, &b) == 2) dispatch(WM_LBUTTONDOWN, 0, (b << 16) | (a & 0xFFFF));
        else if (sscanf(line, "move %ld %ld", &a, &b) == 2) dispatch(WM_MOUSEMOVE, 0, (b << 16) | (a & 0xFFFF));
        else if (strncmp(line, "timer", 5) == 0) dispatch(WM_TIMER, 1, 0);
        else if (strncmp(line, "paint", 5) == 0) dispatch(WM_PAINT, 0, 0);
        else if (strncmp(line, "quit", 4) == 0) PostQuitMessage(0);
    }
    if (f != stdin) fclose(f);
    dispatch(WM_DESTROY, 0, 0);
    return g_quit < 0 ? 0 : g_quit;
}
int main(int argc, char **argv) {
    extern int SovWinMain(void);
    g_argc = argc; g_argv = argv;
    return SovWinMain();
}
