/* mines - Windows-style Minesweeper for the Sovereign OS (win32sov SDK).
   9x9 board, 10 mines placed by a fixed-seed generator (same board every
   run). Click reveals; 'f' toggles flag mode; zero cells flood-fill.
   The safe corner (0,0) never holds a mine. */
#include "win32sov.h"
#include <stdio.h>
#include <string.h>

#define N 9
#define MINES 10
#define CELL 20

static int mine[N][N], revealed[N][N], flagged[N][N], adj[N][N];
static int flag_mode, over, won, nrevealed;

static unsigned rng_state = 12345;
static unsigned rng_next(void) { rng_state = rng_state * 1664525u + 1013904223u; return rng_state >> 16; }
static void place_mines(void) {
    memset(mine, 0, sizeof mine);
    int placed = 0;
    while (placed < MINES) {
        int r = (int)(rng_next() % N), c = (int)(rng_next() % N);
        if (mine[r][c] || (r < 2 && c < 2)) continue;
        mine[r][c] = 1; placed++;
    }
    for (int r = 0; r < N; r++) for (int c = 0; c < N; c++) {
        int n = 0;
        for (int dr = -1; dr <= 1; dr++) for (int dc = -1; dc <= 1; dc++) {
            int rr = r + dr, cc = c + dc;
            if (rr >= 0 && rr < N && cc >= 0 && cc < N && mine[rr][cc]) n++;
        }
        adj[r][c] = n;
    }
}
static void reveal(int r, int c) {
    if (r < 0 || r >= N || c < 0 || c >= N || revealed[r][c] || flagged[r][c]) return;
    revealed[r][c] = 1; nrevealed++;
    if (adj[r][c] == 0 && !mine[r][c]) {
        for (int dr = -1; dr <= 1; dr++) for (int dc = -1; dc <= 1; dc++) reveal(r + dr, c + dc);
    }
}
static void draw(HWND hwnd) {
    HDC dc = BeginPaint(hwnd);
    for (int r = 0; r < N; r++) for (int c = 0; c < N; c++) {
        int x = c * CELL, y = r * CELL;
        Rectangle(dc, x, y, x + CELL, y + CELL);
        if (revealed[r][c]) {
            FillRect(dc, x + 1, y + 1, x + CELL - 1, y + CELL - 1, RGB(200, 200, 200));
            if (mine[r][c]) FillRect(dc, x + 2, y + 2, x + CELL - 2, y + CELL - 2, RGB(255, 0, 0));
            else if (adj[r][c]) { char b[2] = { (char)('0' + adj[r][c]), 0 }; TextOut(dc, x + 7, y + 6, b); }
        } else if (flagged[r][c]) {
            FillRect(dc, x + 3, y + 3, x + CELL - 3, y + CELL - 3, RGB(255, 220, 0));
        }
    }
    EndPaint(hwnd, dc);
}
static long WndProc(HWND hwnd, unsigned msg, long wp, long lp) {
    (void)wp;
    if (msg == WM_CREATE) place_mines();
    else if (msg == WM_CHAR) {
        if (wp == 'f' || wp == 'F') { flag_mode = !flag_mode; SovTrace("FLAG-MODE %s", flag_mode ? "on" : "off"); }
    } else if (msg == WM_LBUTTONDOWN) {
        if (!over && !won) {
            int x = (int)(lp & 0xFFFF), y = (int)((lp >> 16) & 0xFFFF);
            int c = x / CELL, r = y / CELL;
            if (r >= 0 && r < N && c >= 0 && c < N) {
                if (flag_mode) { flagged[r][c] = !flagged[r][c]; SovTrace("FLAG %d,%d %s", r, c, flagged[r][c] ? "set" : "clear"); }
                else if (!flagged[r][c]) {
                    if (mine[r][c]) { over = 1; revealed[r][c] = 1; SovTrace("HIT-MINE %d,%d", r, c); }
                    else { reveal(r, c); SovTrace("REVEALED %d", nrevealed); if (nrevealed == N * N - MINES) won = 1; }
                }
            }
        }
        draw(hwnd);
    } else if (msg == WM_PAINT) {
        draw(hwnd);
    } else if (msg == WM_DESTROY) {
        int nf = 0; for (int r = 0; r < N; r++) for (int c = 0; c < N; c++) nf += flagged[r][c];
        SovTrace("MINES-END revealed=%d flagged=%d status=%s", nrevealed, nf, won ? "won" : over ? "lost" : "playing");
        SovTrace("MINES-HASH %llu", (unsigned long long)SovCanvasHash(hwnd));
    }
    return 0;
}
int SovWinMain(void) {
    CreateMainWindow("Minesweeper", N * CELL, N * CELL, WndProc);
    return SovMessageLoop();
}
