# Sovereign Win Apps

Windows-style applications written in C for the custom Sovereign
operating system. Every application is built against one small SDK,
`win32sov`, which provides the Windows programming shape — a main
window, a window procedure, a message loop, and GDI-style drawing —
without any Microsoft code or headers.

## Layout

- `sdk/win32sov.h` — the application-facing API: `CreateMainWindow`,
  message constants (`WM_CHAR`, `WM_PAINT`, `WM_LBUTTONDOWN`, ...),
  `BeginPaint`/`EndPaint`, `TextOut`, `LineTo`, `Rectangle`, `Ellipse`,
  `SetPixel`, `FillRect`, `SovMessageLoop`.
- `sdk/win32sov.c` — the headless backend. It owns an in-memory canvas
  and pumps a scripted event stream (`char`, `key`, `click`, `move`,
  `timer`, `paint`, `quit`) into the window procedure, so applications
  run and are testable on any machine with a C compiler. A native
  backend for the Sovereign OS implements the same header against the
  OS display and input drivers.
- `apps/` — one directory per application; each is a single `main.c`.

## Applications

- **calc** — four-function calculator with chained operations.
- **notepad** — text editor: loads a file at start, edits in memory
  (typing, backspace), writes the file back on exit.
- **paint** — freehand canvas; mouse strokes, brush colors on
  `r`/`g`/`b`/`w`, `c` clears.
- **fileman** — directory browser: sorted entries with sizes, arrow-key
  selection (key codes 38/40).
- **clock** — analog clock face drawn from the OS time source
  (`SOV_TIME` in the headless backend).
- **terminal** — command terminal: `help`, `ver`, `echo`, `add <a> <b>`,
  `clear`; Enter is key 13, backspace key 8, whole lines via the `line`
  script event.
- **ctrlpanel** — control panel: volume, brightness, theme, Wi-Fi;
  up/down pick a setting, left/right adjust it, space toggles it.
- **taskman** — task manager: live process list from `/proc` (pid +
  command name, sorted by pid), arrow-key selection, `r` refreshes.
- **mines** — Minesweeper: 9x9, 10 mines on a fixed-seed board (same
  board every run), click to reveal with zero flood-fill, `f` toggles
  flag mode.

## Build and test

```
make        # builds all nine apps into bin/
make test   # scripted smoke tests; transcripts are compared verbatim
```

Applications print a canonical transcript through `SovTrace` (display
contents, entry listings, canvas hashes), and the test suite asserts on
it. Drawing is verified by FNV-1a canvas hash, so a rendering change is
a visible, reviewable event.

## License

SNAPKITTY OPAQUE SOURCE LICENSE v1.0 — see `LICENSE`.
