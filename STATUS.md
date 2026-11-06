# STATUS

## Verified (2026-11-06, this machine, gcc -std=c11 -Wall -Wextra)

- All five applications build with zero errors and zero warnings.
- `make test`: 10/10 scripted smoke tests pass — calculator arithmetic
  (12+7=19, 9*9=81), notepad edit + file round-trip, paint stroke
  determinism (identical canvas hash across runs), file manager listing
  + selection, clock time source + face rendering.

## Batch 2 (2026-11-06, this machine, gcc -std=c11 -Wall -Wextra)

- All nine applications build with zero errors and zero warnings.
- `make test`: 21/21 scripted smoke tests pass — batch 1's 10 plus 11
  new: terminal `add 2 3` = 5, echo, ver, unknown-command error;
  control panel settings land exactly (volume 60, brightness 75,
  theme dark, Wi-Fi on); task manager lists live /proc processes;
  Minesweeper reveals from the safe corner, flags a cell, and reports
  its canvas hash.
- SDK addition: a `line <text>` script event types a whole line then
  Enter, so multi-word terminal commands are testable; batch 1 tests
  are unchanged and still pass.

## Scope and limits

- The headless backend is the test surface: one main window per
  process, scripted events, in-memory canvas. Multi-window management,
  menus, and child controls are SDK surface still to be added.
- `TextOut` records a text footprint on the canvas rather than glyphs;
  legible text rendering arrives with the native backend's font driver.
- Nothing here has run on the Sovereign OS itself yet; the native
  backend (display/input drivers behind the same header) is the next
  stage, followed by batch 3 (suggested: solitaire, media player,
  registry editor, wordpad).

## Repository state

Local only. Not pushed; publication follows the owner's destination +
fresh-token protocol.
