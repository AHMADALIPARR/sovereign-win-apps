/* fileman - Windows-style file manager for the Sovereign OS (win32sov SDK).
   Lists argv[2] (default "."); key 38/40 move the selection cursor. */
#include "win32sov.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

static char names[512][256]; static long sizes[512]; static int count, sel;

static long WndProc(HWND hwnd, unsigned msg, long wp, long lp) {
    (void)hwnd; (void)lp;
    if (msg == WM_KEYDOWN) {
        if (wp == 40 && sel < count - 1) sel++;
        if (wp == 38 && sel > 0) sel--;
    } else if (msg == WM_DESTROY) {
        SovTrace("SELECTED %s", count ? names[sel] : "(empty)");
    }
    return 0;
}
int SovWinMain(void) {
    const char *dir = SovArgCount() > 2 ? SovArg(2) : ".";
    DIR *d = opendir(dir);
    if (d) {
        struct dirent *e; char full[1024];
        while ((e = readdir(d)) && count < 512) {
            if (e->d_name[0] == '.') continue;
            snprintf(names[count], 256, "%s", e->d_name);
            snprintf(full, sizeof full, "%s/%s", dir, e->d_name);
            struct stat st; sizes[count] = stat(full, &st) == 0 ? (long)st.st_size : -1;
            count++;
        }
        closedir(d);
    }
    /* insertion sort by name (count is small) */
    for (int i = 1; i < count; i++) {
        char tn[256]; long ts; memcpy(tn, names[i], 256); ts = sizes[i]; int j = i - 1;
        while (j >= 0 && strcmp(names[j], tn) > 0) { memcpy(names[j+1], names[j], 256); sizes[j+1] = sizes[j]; j--; }
        memcpy(names[j+1], tn, 256); sizes[j+1] = ts;
    }
    for (int i = 0; i < count; i++) SovTrace("ENTRY %s %ld", names[i], sizes[i]);
    CreateMainWindow("File Manager", 560, 420, WndProc);
    return SovMessageLoop();
}
