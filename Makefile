CC      ?= gcc
CFLAGS  ?= -std=c11 -O2 -Wall -Wextra -I sdk
APPS    := calc notepad paint fileman clock terminal ctrlpanel taskman mines solitaire mediaplayer regedit wordpad charmap stickies eventvwr recorder cleanmgr
BINS    := $(addprefix bin/,$(APPS))

all: $(BINS)
bin/%: apps/%/main.c sdk/win32sov.c sdk/win32sov.h | bin
	$(CC) $(CFLAGS) -o $@ apps/$*/main.c sdk/win32sov.c -lm
bin:
	mkdir -p bin
test: all
	sh tests/run-tests.sh
clean:
	rm -rf bin
.PHONY: all test clean
