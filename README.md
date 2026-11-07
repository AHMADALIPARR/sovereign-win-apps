# Sovereign Win Apps

Windows-style applications written in pure C for the custom Sovereign operating system.

Every application is built against one small, Microsoft-free SDK (`win32sov`) that provides the classic Windows programming shape: a main window, a window procedure, a message loop, and GDI-style drawing. Applications run and are fully testable on any machine with a C compiler via a headless scripted backend. A native backend for the Sovereign OS targets the same header against real display and input drivers.

> **Purpose:** Re-implement the familiar catalog of Windows applications (and the programming model behind them) as owned, auditable C code for a sovereign OS — no Microsoft headers, no binary blobs, no telemetry. The shape is recognizable; the implementation is ours.

## Retro Windows

The visual and interaction language of this repository is deliberately drawn from the classic Windows era (Windows 95 through XP): title bars, menus, dialogs, the Start-button mental model, and the small set of built-in tools that defined the desktop.

![Windows 95 desktop](https://upload.wikimedia.org/wikipedia/en/9/91/Windows_95_Desktop_screenshot.png)

*Classic Windows 95 desktop — the reference point for window management, icons, and the taskbar metaphor.*

## Architecture

### Repository layout

| Path | Role |
|------|------|
| `sdk/win32sov.h` | Application-facing API (window, messages, drawing, trace) |
| `sdk/win32sov.c` | Headless backend: in-memory canvas + scripted event pump |
| `apps/<name>/main.c` | One self-contained application per directory |
| `bin/` | Built executables |
| `tests/` | Scripted smoke tests and expected transcripts |
| `WINDOWS_CATALOG.md` | Full reference catalog of Windows 10/11 applications |
| `Makefile` | Builds every application with `gcc -std=c11` |

### SDK surface

| Concept | Implementation |
|---------|----------------|
| Window | `CreateMainWindow(title, w, h, WndProc)` — one main window per process |
| Messages | `WM_CREATE`, `WM_DESTROY`, `WM_PAINT`, `WM_CHAR`, `WM_KEYDOWN`, `WM_LBUTTONDOWN`, `WM_MOUSEMOVE`, `WM_TIMER`, `WM_COMMAND` |
| Drawing | `BeginPaint` / `EndPaint`, `TextOut`, `MoveToEx` / `LineTo`, `Rectangle`, `Ellipse`, `SetPixel`, `FillRect` |
| Transcript | `SovTrace` — canonical output used by the test suite |
| Canvas verification | `SovCanvasHash` — FNV-1a over the in-memory canvas |
| Time source | `SovNow` (scripted via `SOV_TIME=hh:mm:ss`) |
| Arguments | `SovArgCount` / `SovArg` |

### Event script (headless backend)

| Event | Meaning |
|-------|---------|
| `char X` | Deliver `WM_CHAR` for character X |
| `line <text>` | Type the text then Enter |
| `key N` | Deliver `WM_KEYDOWN` with virtual-key N |
| `click X Y` | Left-button click at coordinates |
| `move X Y` | Mouse move |
| `timer` | Deliver `WM_TIMER` |
| `paint` | Deliver `WM_PAINT` |
| `quit` | Post quit message |

### Build & test

```
make        # builds all applications into bin/
make test   # scripted smoke tests; transcripts compared verbatim
```

## Application catalog

All applications are pure C, single-file (`apps/<name>/main.c`), and exercise the same SDK. They emit deterministic traces so the test suite can verify behavior without a display server.

### Core desktop tools

| App | Windows counterpart | Description |
|-----|---------------------|-------------|
| **notepad** | Notepad | Plain-text editor; loads a file, edits in memory (type + backspace), writes back on exit |
| **wordpad** | WordPad | Rich-text editor; notepad plus per-character bold (key 2) and italic (key 3) flags |
| **paint** | Paint | Freehand canvas; mouse strokes, brush colors (`r`/`g`/`b`/`w`), `c` clears |
| **calc** | Calculator | Four-function calculator with chained operations |
| **clock** | Clock | Analog clock face drawn from the OS time source |
| **fileman** | File Explorer | Directory browser; sorted entries with sizes, arrow-key selection |
| **charmap** | Character Map | Printable ASCII grid; arrows or clicks select a character |
| **stickies** | Sticky Notes | In-memory note; typing + backspace; traces content on exit |

### System & administration

| App | Windows counterpart | Description |
|-----|---------------------|-------------|
| **taskman** | Task Manager | Live process list from `/proc` (pid + command), arrow-key selection, `r` refreshes |
| **regedit** | Registry Editor | In-memory registry tree; arrows navigate, typing sets values, key 46 deletes |
| **ctrlpanel** | Control Panel | Volume, brightness, theme, Wi-Fi; up/down select, left/right adjust, space toggles |
| **eventvwr** | Event Viewer | Fixed event list; arrows select; traces the chosen event |
| **cleanmgr** | Disk Cleanup | Categories with sizes; arrows select, `c` cleans the selected category |

### Media, games & terminal

| App | Windows counterpart | Description |
|-----|---------------------|-------------|
| **mediaplayer** | Windows Media Player | Three-track playlist; `p` play/pause, `s` stop, `n`/`b` next/previous; timer advances seconds |
| **recorder** | Sound Recorder | `r` toggles recording; timer ticks accumulate seconds while recording |
| **mines** | Minesweeper | 9×9, 10 mines on a fixed-seed board; click to reveal (zero flood-fill), `f` flag mode |
| **solitaire** | Solitaire (Klondike) | Fixed deck order; click tableau top card to foundation, click stock to draw |
| **terminal** | Windows Terminal / Command Prompt | Commands: `help`, `ver`, `echo`, `add <a> <b>`, `clear`; Enter = key 13, backspace = key 8, `line` for whole commands |

## Reference catalog

The complete list of Windows 10 and Windows 11 built-in applications, utilities, management consoles, developer tools, and system components is maintained in [`WINDOWS_CATALOG.md`](WINDOWS_CATALOG.md). The applications above are the subset currently implemented in C against the `win32sov` SDK.

## License

SNAPKITTY OPAQUE SOURCE LICENSE v1.0 — see [`LICENSE`](LICENSE).
