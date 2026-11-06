/* taskman - Windows-style task manager for the Sovereign OS (win32sov SDK).
   Lists the live processes from /proc (pid + command name), sorted by pid.
   Up/down (key 38/40) move the selection; 'r' re-reads the table. */
#include "win32sov.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>

static long pids[512]; static char comms[512][128]; static int count, sel;

static int is_pid(const char *s) { for (; *s; s++) if (!isdigit((unsigned char)*s)) return 0; return 1; }
static void read_procs(void) {
    count = 0; sel = 0;
    DIR *d = opendir("/proc");
    if (!d) return;
    struct dirent *e;
    while ((e = readdir(d)) && count < 512) {
        if (!is_pid(e->d_name)) continue;
        char path[512]; snprintf(path, sizeof path, "/proc/%s/comm", e->d_name);
        FILE *f = fopen(path, "r");
        if (!f) continue;
        char buf[128];
        if (fgets(buf, sizeof buf, f)) {
            buf[strcspn(buf, "\n")] = 0;
            pids[count] = atol(e->d_name);
            snprintf(comms[count], sizeof comms[0], "%s", buf);
            count++;
        }
        fclose(f);
    }
    closedir(d);
    for (int i = 1; i < count; i++) {
        long tp = pids[i]; char tc[128]; memcpy(tc, comms[i], 128); int j = i - 1;
        while (j >= 0 && pids[j] > tp) { pids[j+1] = pids[j]; memcpy(comms[j+1], comms[j], 128); j--; }
        pids[j+1] = tp; memcpy(comms[j+1], tc, 128);
    }
}
static void draw(HWND hwnd) {
    HDC dc = BeginPaint(hwnd);
    int n = count < 20 ? count : 20;
    for (int i = 0; i < n; i++) {
        char buf[200]; snprintf(buf, sizeof buf, "%ld %s", pids[i], comms[i]);
        TextOut(dc, 8, 8 + i * 12, buf);
        if (i == sel) Rectangle(dc, 2, 4 + i * 12, 300, 18 + i * 12);
    }
    EndPaint(hwnd, dc);
}
static long WndProc(HWND hwnd, unsigned msg, long wp, long lp) {
    (void)lp;
    if (msg == WM_CREATE) {
        read_procs();
        SovTrace("PROCESSES %d", count);
    } else if (msg == WM_KEYDOWN) {
        if (wp == 40 && sel < count - 1) sel++;
        else if (wp == 38 && sel > 0) sel--;
        draw(hwnd);
    } else if (msg == WM_CHAR) {
        if (wp == 'r' || wp == 'R') { read_procs(); SovTrace("REFRESHED %d", count); draw(hwnd); }
    } else if (msg == WM_PAINT) {
        draw(hwnd);
    } else if (msg == WM_DESTROY) {
        SovTrace("TASKMAN-END processes=%d selected=%ld %s",
            count, count ? pids[sel] : 0, count ? comms[sel] : "(none)");
    }
    return 0;
}
int SovWinMain(void) {
    CreateMainWindow("Task Manager", 320, 280, WndProc);
    return SovMessageLoop();
}
