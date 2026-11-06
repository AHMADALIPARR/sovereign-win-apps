/* notepad - Windows-style text editor for the Sovereign OS (win32sov SDK).
   Loads argv[2] at create when the file exists; saves it back on destroy. */
#include "win32sov.h"
#include <stdio.h>
#include <string.h>

static char buf[65536]; static size_t len;
static const char *path;

static long WndProc(HWND hwnd, unsigned msg, long wp, long lp) {
    (void)hwnd; (void)lp;
    if (msg == WM_CHAR) {
        char c = (char)wp;
        if (c >= 32 || c == '\n' || c == '\t') { if (len < sizeof buf - 1) buf[len++] = c; }
    } else if (msg == WM_KEYDOWN && wp == VK_BACK) {
        if (len) len--;
    } else if (msg == WM_DESTROY) {
        int lines = 1; for (size_t i = 0; i < len; i++) if (buf[i] == '\n') lines++;
        if (path) { FILE *f = fopen(path, "w"); if (f) { fwrite(buf, 1, len, f); fclose(f); } }
        SovTrace("NOTEPAD-END chars=%zu lines=%d", len, lines);
        SovTrace("CONTENT-BEGIN"); fwrite(buf, 1, len, stdout); SovTrace("\nCONTENT-END");
    }
    return 0;
}
int SovWinMain(void) {
    path = SovArgCount() > 2 ? SovArg(2) : NULL;
    if (path) { FILE *f = fopen(path, "r"); if (f) { len = fread(buf, 1, sizeof buf - 1, f); fclose(f); } }
    CreateMainWindow("Notepad", 640, 480, WndProc);
    return SovMessageLoop();
}
