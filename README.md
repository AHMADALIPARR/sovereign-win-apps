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
- `WINDOWS_CATALOG.md` — reference catalog of Windows 10/11 applications.

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
- **solitaire** — Klondike: fixed deck order so every run deals the
  same board; click a tableau top card to move it to its foundation,
  click the stock to draw to the waste.
- **mediaplayer** — media player: three-track playlist, `p` play/pause,
  `s` stop, `n`/`b` next/previous, timer ticks advance one second.
- **regedit** — registry editor: three-key in-memory tree, arrow keys
  navigate, typing a line sets the selected value, key 46 deletes it.
- **wordpad** — rich text editor: notepad's file round-trip plus
  per-character bold (key 2) and italic (key 3) flags.
- **charmap** — character map: printable ASCII grid, arrow keys or clicks
  select a character, traces code and glyph.
- **stickies** — sticky notes: in-memory note with typing and backspace,
  traces content on exit.
- **eventvwr** — event viewer: fixed event list, arrow keys select,
  traces selected event.
- **recorder** — sound recorder: `r` toggles recording, timer ticks
  accumulate seconds while recording.
- **cleanmgr** — disk cleanup: categories with sizes, arrows select,
  `c` cleans the selected category.

## Build and test

```
make        # builds all apps into bin/
make test   # scripted smoke tests; transcripts are compared verbatim
```

Applications print a canonical transcript through `SovTrace` (display
contents, entry listings, canvas hashes), and the test suite asserts on
it. Drawing is verified by FNV-1a canvas hash, so a rendering change is
a visible, reviewable event.

## License

SNAPKITTY OPAQUE SOURCE LICENSE v1.0 — see `LICENSE`.
