/* regedit - Windows-style registry editor for the Sovereign OS (win32sov SDK).
   A fixed in-memory key tree (three keys, two values each). Up/down pick a
   key, left/right pick a value; typing a line and pressing Enter sets the
   selected value's data, key 46 (Delete) deletes the selected value. The
   tree is state only: no on-disk hive yet. */
#include "win32sov.h"
#include <stdio.h>
#include <string.h>

#define NKEYS 3
#define NVALS 2

static const char *keys[NKEYS] = {
    "HKLM\\Software\\Sovereign",
    "HKCU\\Control Panel",
    "HKLM\\System\\CurrentControlSet"
};
static const char *vnames[NKEYS][NVALS] = {
    { "Version", "Owner" },
    { "Theme", "Volume" },
    { "Boot", "Debug" }
};
static char vdata[NKEYS][NVALS][64] = {
    { "1.0", "Nova Parr" },
    { "dark", "60" },
    { "fast", "off" }
};
static int deleted[NKEYS][NVALS];
static int sel_key, sel_val;
static char edit[256]; static int elen;

static int nvalues(void) {
    int n = 0;
    for (int k = 0; k < NKEYS; k++) for (int v = 0; v < NVALS; v++) if (!deleted[k][v]) n++;
    return n;
}
static void draw(HWND hwnd) {
    HDC dc = BeginPaint(hwnd);
    for (int k = 0; k < NKEYS; k++) {
        char buf[160];
        snprintf(buf, sizeof buf, "%s", keys[k]);
        TextOut(dc, 12, 16 + k * 64, buf);
        if (k == sel_key) Rectangle(dc, 4, 12 + k * 64, 380, 30 + k * 64);
        for (int v = 0; v < NVALS; v++) {
            if (deleted[k][v]) continue;
            snprintf(buf, sizeof buf, "  %s = %s", vnames[k][v], vdata[k][v]);
            TextOut(dc, 12, 34 + k * 64 + v * 14, buf);
            if (k == sel_key && v == sel_val) Rectangle(dc, 8, 30 + k * 64 + v * 14, 300, 46 + k * 64 + v * 14);
        }
    }
    char prompt[300]; snprintf(prompt, sizeof prompt, "> %s", edit);
    TextOut(dc, 12, 220, prompt);
    EndPaint(hwnd, dc);
}
static long WndProc(HWND hwnd, unsigned msg, long wp, long lp) {
    (void)lp;
    if (msg == WM_CHAR) {
        char c = (char)wp;
        if (c >= 32 && elen < 250) { edit[elen++] = c; edit[elen] = 0; }
    } else if (msg == WM_KEYDOWN) {
        if (wp == 46) {
            if (!deleted[sel_key][sel_val]) {
                deleted[sel_key][sel_val] = 1;
                SovTrace("DEL %s\\%s", keys[sel_key], vnames[sel_key][sel_val]);
            }
        }
        else if (wp == 40 && sel_key < NKEYS - 1) { sel_key++; sel_val = 0; }
        else if (wp == 38 && sel_key > 0) { sel_key--; sel_val = 0; }
        else if (wp == 39 && sel_val < NVALS - 1) sel_val++;
        else if (wp == 37 && sel_val > 0) sel_val--;
        else if (wp == 13 && elen) {
            snprintf(vdata[sel_key][sel_val], sizeof vdata[0][0], "%.*s", (int)sizeof vdata[0][0] - 1, edit);
            deleted[sel_key][sel_val] = 0;
            SovTrace("SET %s\\%s=%s", keys[sel_key], vnames[sel_key][sel_val], edit);
            elen = 0; edit[0] = 0;
        } else if (wp == 8 && elen > 0) edit[--elen] = 0;
        draw(hwnd);
    } else if (msg == WM_PAINT) {
        draw(hwnd);
    } else if (msg == WM_DESTROY) {
        for (int k = 0; k < NKEYS; k++) for (int v = 0; v < NVALS; v++)
            if (!deleted[k][v]) SovTrace("REG %s\\%s=%s", keys[k], vnames[k][v], vdata[k][v]);
        SovTrace("REG-END keys=%d values=%d", NKEYS, nvalues());
    }
    return 0;
}
int SovWinMain(void) {
    CreateMainWindow("Registry Editor", 400, 240, WndProc);
    return SovMessageLoop();
}
