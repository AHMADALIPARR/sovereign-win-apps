#!/bin/sh
# Scripted smoke tests: each app runs headless against an event script and
# its transcript is compared with the recorded expectation.
set -u
cd "$(dirname "$0")/.."
fail=0
check() { # name expected-substring actual
  case "$3" in *"$2"*) echo "PASS $1";; *) echo "FAIL $1 (wanted: $2)"; echo "$3" | head -5; fail=1;; esac
}
printf 'char 1\nchar 2\nchar +\nchar 7\nchar =\n' > tests/calc.txt
out=$(./bin/calc tests/calc.txt);            check calc-eq "DISPLAY 19" "$out"
printf 'char 9\nchar *\nchar 9\nchar =\n' > tests/calc2.txt
out=$(./bin/calc tests/calc2.txt);           check calc-mul "DISPLAY 81" "$out"
printf 'char H\nchar i\n' > tests/note.txt
rm -f tests/note-out.txt
out=$(./bin/notepad tests/note.txt tests/note-out.txt)
check notepad-chars "chars=2 lines=1" "$out"
check notepad-file "Hi" "$(cat tests/note-out.txt)"
printf 'click 10 10\nmove 60 40\nmove 120 90\n' > tests/paint.txt
out=$(./bin/paint tests/paint.txt);          check paint-draw "PAINT-HASH" "$out"
hash1=$(echo "$out" | awk '{print $2}')
out=$(./bin/paint tests/paint.txt);          check paint-stable "$hash1" "$out"
out=$(printf 'key 40\nkey 40\n' | ./bin/fileman - sdk)
check fileman-list "ENTRY win32sov.c" "$out"
check fileman-sel "SELECTED win32sov.h" "$out"
out=$(printf '' | SOV_TIME=10:09:35 ./bin/clock -)
check clock-time "TIME 10:09:35" "$out"
check clock-draw "CLOCK-HASH" "$out"
# --- batch 2 ---
printf 'line add 2 3\nline echo hello\nline ver\nline bogus\n' > tests/term.txt
out=$(./bin/terminal tests/term.txt)
check term-add "RESULT 5" "$out"
check term-echo "TERM hello" "$out"
check term-ver "SOVEREIGN WIN32SOV 1.0" "$out"
check term-err "ERROR unknown command: bogus" "$out"
printf 'key 39\nkey 39\nkey 40\nkey 39\nkey 40\nkey 32\n' > tests/ctrl.txt
out=$(./bin/ctrlpanel tests/ctrl.txt)
check ctrl-settings "SETTINGS volume=60 brightness=75 theme=dark wifi=on" "$out"
out=$(printf '' | ./bin/taskman -)
check taskman-list "PROCESSES " "$out"
check taskman-end "TASKMAN-END processes=" "$out"
printf 'click 10 10\nchar f\nclick 30 10\n' > tests/mines.txt
out=$(./bin/mines tests/mines.txt)
check mines-reveal "REVEALED " "$out"
check mines-flag "FLAG 0,1 set" "$out"
check mines-end "MINES-END revealed=" "$out"
check mines-hash "MINES-HASH" "$out"
exit $fail
