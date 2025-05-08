CC=gcc
CFLAGS=-g -Wall -Wextra -Wshadow -Wconversion -Wunreachable-code -std=c99

PROG=buf philo canvas msgbox semaphores

.PHONY: all
all: $(PROG)

%: %.c
	$(CC) -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(PROG)

zip:
	git archive --format zip --output ${USER}-lab04.zip HEAD

html:
	pandoc -o README.html -f gfm README.md
