/* recorder - Windows-style Sound Recorder for the Sovereign OS (win32sov SDK).
   'r' start/stop; timer ticks accumulate seconds while recording. */
#include "win32sov.h"
#include <stdio.h>

static int recording = 0, secs = 0;
static void show(void) { SovTrace("RECORDER %s secs=%d", recording ? "recording" : "stopped", secs); }
static long WndProc(HWND hwnd, unsigned msg, long wp, long lp) {
    (void)hwnd; (void)lp;
    if (msg == WM_CHAR && (wp == 'r' || wp == 'R')) {
        recording = !recording;
        if (!recording) show();
    } else if (msg == WM_TIMER && recording) {
        secs++;
        show();
    } else if (msg == WM_DESTROY) {
        SovTrace("RECORDER-END secs=%d", secs);
    }
    return 0;
}
int SovWinMain(void) {
    CreateMainWindow("Sound Recorder", 240, 120, WndProc);
    show();
    return SovMessageLoop();
}
