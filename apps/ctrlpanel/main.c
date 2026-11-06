/* ctrlpanel - Windows-style control panel for the Sovereign OS (win32sov SDK).
   Up/down (key 38/40) pick a setting; left/right (37/39) adjust it;
   space toggles the selected on/off setting. Settings are real state,
   reported on exit. */
#include "win32sov.h"
#include <stdio.h>

static int volume = 50, brightness = 70, theme_dark, wifi_on = 1, sel;
static const char *names[4] = { "Volume", "Brightness", "Theme", "Wi-Fi" };

static int clamp(int v) { return v < 0 ? 0 : v > 100 ? 100 : v; }
static void adjust(int d) {
    if (sel == 0) volume = clamp(volume + 5 * d);
    else if (sel == 1) brightness = clamp(brightness + 5 * d);
    else if (sel == 2) theme_dark = !theme_dark;
    else wifi_on = !wifi_on;
    SovTrace("SETTING %s", names[sel]);
}
static void draw(HWND hwnd) {
    HDC dc = BeginPaint(hwnd);
    for (int i = 0; i < 4; i++) {
        int y = 20 + i * 40;
        char buf[96];
        if (i == 0) snprintf(buf, sizeof buf, "Volume %d", volume);
        else if (i == 1) snprintf(buf, sizeof buf, "Brightness %d", brightness);
        else if (i == 2) snprintf(buf, sizeof buf, "Theme %s", theme_dark ? "dark" : "light");
        else snprintf(buf, sizeof buf, "Wi-Fi %s", wifi_on ? "on" : "off");
        TextOut(dc, 12, y, buf);
        if (i < 2) {
            int v = i == 0 ? volume : brightness;
            FillRect(dc, 140, y, 140 + v * 2, y + 10, RGB(0, 120, 215));
        }
        if (i == sel) Rectangle(dc, 4, y - 4, 380, y + 16);
    }
    EndPaint(hwnd, dc);
}
static long WndProc(HWND hwnd, unsigned msg, long wp, long lp) {
    (void)lp;
    if (msg == WM_KEYDOWN) {
        if (wp == 40 && sel < 3) sel++;
        else if (wp == 38 && sel > 0) sel--;
        else if (wp == 39) adjust(1);
        else if (wp == 37) adjust(-1);
        else if (wp == 32) adjust(0);
        draw(hwnd);
    } else if (msg == WM_PAINT) {
        draw(hwnd);
    } else if (msg == WM_DESTROY) {
        SovTrace("SETTINGS volume=%d brightness=%d theme=%s wifi=%s",
            volume, brightness, theme_dark ? "dark" : "light", wifi_on ? "on" : "off");
    }
    return 0;
}
int SovWinMain(void) {
    CreateMainWindow("Control Panel", 400, 200, WndProc);
    return SovMessageLoop();
}
