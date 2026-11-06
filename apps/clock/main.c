/* clock - Windows-style analog clock for the Sovereign OS (win32sov SDK).
   Time source is SovNow (SOV_TIME in the headless backend). */
#include "win32sov.h"
#include <math.h>
#define SOV_PI 3.141592653589793

static void draw(HWND hwnd) {
    int hh, mm, ss; SovNow(&hh, &mm, &ss);
    HDC dc = BeginPaint(hwnd);
    Ellipse(dc, 20, 20, 220, 220);
    double cx = 120, cy = 120;
    double ha = ((hh % 12) + mm / 60.0) * 30.0 * SOV_PI / 180.0;
    double ma = (mm + ss / 60.0) * 6.0 * SOV_PI / 180.0;
    double sa = ss * 6.0 * SOV_PI / 180.0;
    MoveToEx(dc, (int)cx, (int)cy); LineTo(dc, (int)(cx + 55 * sin(ha)), (int)(cy - 55 * cos(ha)));
    MoveToEx(dc, (int)cx, (int)cy); LineTo(dc, (int)(cx + 85 * sin(ma)), (int)(cy - 85 * cos(ma)));
    MoveToEx(dc, (int)cx, (int)cy); LineTo(dc, (int)(cx + 95 * sin(sa)), (int)(cy - 95 * cos(sa)));
    EndPaint(hwnd, dc);
    SovTrace("TIME %02d:%02d:%02d", hh, mm, ss);
}
static long WndProc(HWND hwnd, unsigned msg, long wp, long lp) {
    (void)wp; (void)lp;
    if (msg == WM_CREATE || msg == WM_TIMER) draw(hwnd);
    if (msg == WM_DESTROY) SovTrace("CLOCK-HASH %llu", (unsigned long long)SovCanvasHash(hwnd));
    return 0;
}
int SovWinMain(void) { CreateMainWindow("Clock", 240, 240, WndProc); return SovMessageLoop(); }
