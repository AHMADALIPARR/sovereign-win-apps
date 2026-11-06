/* wordpad - Windows-style rich text editor for the Sovereign OS (win32sov SDK).
   Notepad's file round-trip (argv[2]) plus per-character bold and italic
   flags. Format toggles are key events in the headless script: key 2 is
   Ctrl-B (bold), key 3 is Ctrl-I (italic); typed characters take the
   toggles in force at the time. Backspace is key 8. */
#include "win32sov.h"
#include <stdio.h>
#include <string.h>

static char buf[65536]; static size_t len;
static unsigned char bold[65536], ital[65536];
static int bold_on, ital_on;
static const char *path;

static int count_flag(const unsigned char *f) {
    int n = 0; for (size_t i = 0; i < len; i++) if (f[i]) n++; return n;
}
static long WndProc(HWND hwnd, unsigned msg, long wp, long lp) {
    (void)hwnd; (void)lp;
    if (msg == WM_CHAR) {
        char c = (char)wp;
        if (c >= 32 || c == '\n' || c == '\t') {
            if (len < sizeof buf - 1) {
                buf[len] = c; bold[len] = bold_on ? 1 : 0; ital[len] = ital_on ? 1 : 0; len++;
            }
        }
    } else if (msg == WM_KEYDOWN) {
        if (wp == VK_BACK) { if (len) len--; }
        else if (wp == 2) { bold_on = !bold_on; SovTrace("BOLD %s", bold_on ? "on" : "off"); }
        else if (wp == 3) { ital_on = !ital_on; SovTrace("ITALIC %s", ital_on ? "on" : "off"); }
    } else if (msg == WM_DESTROY) {
        int lines = 1; for (size_t i = 0; i < len; i++) if (buf[i] == '\n') lines++;
        if (path) { FILE *f = fopen(path, "w"); if (f) { fwrite(buf, 1, len, f); fclose(f); } }
        SovTrace("WORDPAD-END chars=%zu lines=%d bold=%d italic=%d", len, lines, count_flag(bold), count_flag(ital));
        SovTrace("CONTENT-BEGIN"); fwrite(buf, 1, len, stdout); SovTrace("\nCONTENT-END");
    }
    return 0;
}
int SovWinMain(void) {
    path = SovArgCount() > 2 ? SovArg(2) : NULL;
    if (path) { FILE *f = fopen(path, "r"); if (f) { len = fread(buf, 1, sizeof buf - 1, f); fclose(f); } }
    CreateMainWindow("WordPad", 640, 480, WndProc);
    return SovMessageLoop();
}
