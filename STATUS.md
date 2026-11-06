# STATUS

## Verified (2026-11-06, this machine, gcc -std=c11 -Wall -Wextra)

- All five applications build with zero errors and zero warnings.
- `make test`: 10/10 scripted smoke tests pass — calculator arithmetic
  (12+7=19, 9*9=81), notepad edit + file round-trip, paint stroke
  determinism (identical canvas hash across runs), file manager listing
  + selection, clock time source + face rendering.

## Scope and limits

- The headless backend is the test surface: one main window per
  process, scripted events, in-memory canvas. Multi-window management,
  menus, and child controls are SDK surface still to be added.
- `TextOut` records a text footprint on the canvas rather than glyphs;
  legible text rendering arrives with the native backend's font driver.
- Nothing here has run on the Sovereign OS itself yet; the native
  backend (display/input drivers behind the same header) is the next
  stage, followed by the next application batch (terminal, control
  panel, task manager, minesweeper).

## Repository state

Local only. Not pushed; publication follows the owner's destination +
fresh-token protocol.
