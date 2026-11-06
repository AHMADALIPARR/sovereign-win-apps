/* win32sov.h - Windows-style C application SDK for the Sovereign OS.
   One main window per process, a message loop, and GDI-style drawing onto
   an in-memory canvas. The headless backend pumps scripted events so every
   application runs and is testable without a display server; a native
   backend for the Sovereign OS targets the same API. */
#ifndef WIN32SOV_H
#define WIN32SOV_H
#include <stdint.h>
#include <stddef.h>

typedef int BOOL;
typedef uint32_t DWORD;
typedef uint32_t COLORREF;
typedef struct SWnd *HWND;
typedef struct SDC *HDC;
#define TRUE 1
#define FALSE 0
#define RGB(r,g,b) ((COLORREF)((r) | ((g) << 8) | ((b) << 16)))

enum {
    WM_CREATE = 1, WM_DESTROY = 2, WM_PAINT = 3, WM_KEYDOWN = 4,
    WM_CHAR = 5, WM_LBUTTONDOWN = 6, WM_MOUSEMOVE = 7, WM_COMMAND = 8,
    WM_TIMER = 9, WM_USER = 0x400
};
#define VK_ESCAPE 27
#define VK_RETURN 13
#define VK_BACK   8

typedef long (*WNDPROC)(HWND hwnd, unsigned msg, long wparam, long lparam);

HWND CreateMainWindow(const char *title, int width, int height, WNDPROC proc);
void PostQuitMessage(int code);
int  SovMessageLoop(void);                 /* returns quit code */
void InvalidateRect(HWND hwnd);
HDC  BeginPaint(HWND hwnd);
void EndPaint(HWND hwnd, HDC dc);
void TextOut(HDC dc, int x, int y, const char *text);
void MoveToEx(HDC dc, int x, int y);
void LineTo(HDC dc, int x, int y);
void Rectangle(HDC dc, int l, int t, int r, int b);
void Ellipse(HDC dc, int l, int t, int r, int b);
void SetPixel(HDC dc, int x, int y, COLORREF c);
void FillRect(HDC dc, int l, int t, int r, int b, COLORREF c);
void SovTrace(const char *fmt, ...);       /* canonical transcript line */
uint64_t SovCanvasHash(HWND hwnd);         /* FNV-1a over the canvas */
void SovNow(int *hh, int *mm, int *ss);    /* scripted by SOV_TIME=hh:mm:ss */
int  SovArgCount(void);
const char *SovArg(int i);
#endif
