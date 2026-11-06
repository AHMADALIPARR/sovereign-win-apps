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

## Batch 3 (2026-11-06, this machine, gcc -std=c11 -Wall -Wextra)

- All thirteen applications build with zero errors and zero warnings.
- `make test`: 34/34 scripted smoke tests pass — batch 1+2's 21 plus 13
  new: solitaire moves the A of spades to its foundation on the fixed
  deck and draws the K of clubs from the stock; media player plays,
  advances 0:03 on three timer ticks, and steps to the next track;
  registry editor sets `HKCU\Control Panel\Volume=70`, deletes it, and
  reports 5 values left; wordpad round-trips "Hi!?" with bold=2,
  italic=1 per-character flags.
- No SDK change was needed: batch 3 runs on the batch 1 event surface
  (`char`, `line`, `key`, `click`, `timer`, `paint`).

## Scope and limits

- The headless backend is the test surface: one main window per
  process, scripted events, in-memory canvas. Multi-window management,
  menus, and child controls are SDK surface still to be added.
- `TextOut` records a text footprint on the canvas rather than glyphs;
  legible text rendering arrives with the native backend's font driver.
- Nothing here has run on the Sovereign OS itself yet; the native
  backend (display/input drivers behind the same header) is the next
  stage, followed by batch 4 (suggested: event viewer, character map,
  sound recorder, disk cleanup).

## Repository state

Local only. Not pushed; publication follows the owner's destination +
fresh-token protocol.
