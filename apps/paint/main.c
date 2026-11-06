/* paint - Windows-style paint canvas for the Sovereign OS (win32sov SDK).
   Keys r/g/b/w pick the brush, c clears, mouse draws freehand lines. */
#include "win32sov.h"

static COLORREF brush = RGB(255, 255, 255);
static int down, lastx, lasty;

static long WndProc(HWND hwnd, unsigned msg, long wp, long lp) {
    int x = (int)(lp & 0xFFFF), y = (int)((lp >> 16) & 0xFFFF);
    if (msg == WM_LBUTTONDOWN) { down = 1; lastx = x; lasty = y; }
    else if (msg == WM_MOUSEMOVE && down) {
        HDC dc = BeginPaint(hwnd);
        /* recolor via 1px stamps along the segment: LineTo uses dc pen,
           so stamp with SetPixel in a short walk */
        int steps = 24;
        for (int i = 0; i <= steps; i++)
            SetPixel(dc, lastx + (x - lastx) * i / steps, lasty + (y - lasty) * i / steps, brush);
        EndPaint(hwnd, dc);
        lastx = x; lasty = y;
    }
    else if (msg == WM_CHAR) {
        char c = (char)wp; down = 0;
        if (c == 'r') brush = RGB(255, 0, 0); else if (c == 'g') brush = RGB(0, 255, 0);
        else if (c == 'b') brush = RGB(0, 0, 255); else if (c == 'w') brush = RGB(255, 255, 255);
        else if (c == 'c') { HDC dc = BeginPaint(hwnd); FillRect(dc, 0, 0, 4096, 4096, RGB(0,0,0)); EndPaint(hwnd, dc); }
    }
    else if (msg == WM_DESTROY) SovTrace("PAINT-HASH %llu", (unsigned long long)SovCanvasHash(hwnd));
    return 0;
}
int SovWinMain(void) { CreateMainWindow("Paint", 480, 360, WndProc); return SovMessageLoop(); }
