/* solitaire - Windows-style Klondike solitaire for the Sovereign OS (win32sov SDK).
   Fixed deck order (A..K of spades, hearts, diamonds, clubs) so every run
   deals the same board. Click a tableau top card to move it to its
   foundation when it follows suit ascending; click the stock (top right)
   to draw to the waste; click the waste card to move it to a foundation.
   No audio, no animation: the board state is the game. */
#include "win32sov.h"
#include <stdio.h>
#include <string.h>

#define COLS 7
#define CW 40
#define CH 48

static int deck[52], di;
static int tab[COLS][20], tn[COLS];
static int stock[52], ns, waste[52], nw;
static int found[4]; /* next rank expected per suit, 1..13 */
static int nmoves;

static const char *ranks = "A23456789TJQK";
static const char *suits = "SHDC";
static void cardname(int card, char *out) {
    out[0] = ranks[card % 13]; out[1] = suits[card / 13]; out[2] = 0;
}
static void deal(void) {
    for (int i = 0; i < 52; i++) deck[i] = i;
    di = 0;
    memset(tn, 0, sizeof tn); memset(found, 0, sizeof found);
    ns = 0; nw = 0; nmoves = 0;
    for (int r = 0; r < COLS; r++)
        for (int c = r; c < COLS; c++) tab[c][tn[c]++] = deck[di++];
    while (di < 52) stock[ns++] = deck[di++];
}
static int to_foundation(int card) {
    int suit = card / 13, rank = card % 13 + 1;
    if (rank != found[suit] + 1) return 0;
    found[suit] = rank; nmoves++;
    char n[3]; cardname(card, n);
    SovTrace("FOUNDATION %s count=%d", n, found[0] + found[1] + found[2] + found[3]);
    return 1;
}
static void draw_card(HWND hwnd) {
    HDC dc = BeginPaint(hwnd);
    for (int c = 0; c < COLS; c++) {
        int x = 8 + c * 44;
        for (int r = 0; r < tn[c]; r++) {
            int y = 70 + r * 14;
            Rectangle(dc, x, y, x + CW, y + CH);
            if (r == tn[c] - 1) { char n[3]; cardname(tab[c][r], n); TextOut(dc, x + 6, y + 6, n); }
        }
    }
    for (int s = 0; s < 4; s++) {
        int x = 8 + s * 44, y = 8;
        Rectangle(dc, x, y, x + CW, y + 44);
        if (found[s]) { char n[3]; cardname(s * 13 + found[s] - 1, n); TextOut(dc, x + 6, y + 6, n); }
    }
    Rectangle(dc, 280, 8, 320, 52);
    if (ns) TextOut(dc, 286, 12, "STOCK");
    if (nw) { char n[3]; cardname(waste[nw - 1], n); TextOut(dc, 236, 12, n); }
    EndPaint(hwnd, dc);
}
static long WndProc(HWND hwnd, unsigned msg, long wp, long lp) {
    (void)wp;
    if (msg == WM_CREATE) deal();
    else if (msg == WM_LBUTTONDOWN) {
        int x = (int)(lp & 0xFFFF), y = (int)((lp >> 16) & 0xFFFF);
        if (y < 60) {
            if (x >= 280 && ns) { waste[nw++] = stock[--ns]; char n[3]; cardname(waste[nw - 1], n); SovTrace("DRAW %s stock=%d", n, ns); }
            else if (x >= 220 && x < 280 && nw && to_foundation(waste[nw - 1])) nw--;
        } else {
            int c = (x - 8) / 44;
            if (c >= 0 && c < COLS && tn[c]) {
                int top = tab[c][tn[c] - 1];
                if (to_foundation(top)) tn[c]--;
                else { char n[3]; cardname(top, n); SovTrace("BLOCKED %s", n); }
            }
        }
        draw_card(hwnd);
    } else if (msg == WM_PAINT) {
        draw_card(hwnd);
    } else if (msg == WM_DESTROY) {
        int f = found[0] + found[1] + found[2] + found[3];
        SovTrace("SOLITAIRE-END foundation=%d stock=%d waste=%d moves=%d", f, ns, nw, nmoves);
        SovTrace("SOLITAIRE-HASH %llu", (unsigned long long)SovCanvasHash(hwnd));
    }
    return 0;
}
int SovWinMain(void) {
    CreateMainWindow("Solitaire", 328, 260, WndProc);
    return SovMessageLoop();
}
