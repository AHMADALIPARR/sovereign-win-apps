/* mediaplayer - Windows-style media player for the Sovereign OS (win32sov SDK).
   Fixed playlist of three tracks; keys drive it: 'p' play/pause, 's' stop,
   'n' next, 'b' previous, and each WM_TIMER tick advances one second while
   playing. Playback position is the model here; the audio driver arrives
   with the native backend. */
#include "win32sov.h"
#include <stdio.h>
#include <string.h>

static const char *names[3] = { "Boot Hymn", "Ledger Blues", "Ghost Waltz" };
static const int lens[3] = { 125, 200, 90 };
static int track, pos, playing;

static void show(void) {
    SovTrace("TRACK %s %d:%02d/%d:%02d %s", names[track], pos / 60, pos % 60,
        lens[track] / 60, lens[track] % 60, playing ? "playing" : "paused");
}
static void draw(HWND hwnd) {
    HDC dc = BeginPaint(hwnd);
    char buf[128];
    snprintf(buf, sizeof buf, "%s", names[track]);
    TextOut(dc, 12, 20, buf);
    snprintf(buf, sizeof buf, "%d:%02d / %d:%02d", pos / 60, pos % 60, lens[track] / 60, lens[track] % 60);
    TextOut(dc, 12, 44, buf);
    FillRect(dc, 12, 70, 12 + (pos * 200) / lens[track], 80, RGB(0, 120, 215));
    Rectangle(dc, 12, 70, 212, 80);
    EndPaint(hwnd, dc);
}
static long WndProc(HWND hwnd, unsigned msg, long wp, long lp) {
    (void)lp;
    if (msg == WM_CHAR) {
        if (wp == 'p' || wp == 'P') { playing = !playing; SovTrace(playing ? "PLAY %s" : "PAUSE %s", names[track]); }
        else if (wp == 's' || wp == 'S') { playing = 0; pos = 0; SovTrace("STOP %s", names[track]); }
        else if (wp == 'n' || wp == 'N') { track = (track + 1) % 3; pos = 0; SovTrace("NEXT %s", names[track]); }
        else if (wp == 'b' || wp == 'B') { track = (track + 2) % 3; pos = 0; SovTrace("PREV %s", names[track]); }
        draw(hwnd);
    } else if (msg == WM_TIMER) {
        if (playing) {
            pos++;
            if (pos >= lens[track]) {
                if (track < 2) { track++; pos = 0; SovTrace("NEXT %s", names[track]); }
                else { playing = 0; pos = 0; SovTrace("PLAYLIST-END"); }
            }
            show();
        }
        draw(hwnd);
    } else if (msg == WM_PAINT) {
        draw(hwnd);
    } else if (msg == WM_DESTROY) {
        SovTrace("MEDIA-END track=%d pos=%d playing=%d", track, pos, playing);
    }
    return 0;
}
int SovWinMain(void) {
    CreateMainWindow("Media Player", 240, 120, WndProc);
    return SovMessageLoop();
}
