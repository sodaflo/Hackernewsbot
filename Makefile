CC = gcc
CFLAGS = -O0 -g -Wall -Wextra -std=c89 -pedantic-errors

all: Bot

.PHONY: test
Bot: main.c parson.c
	$(CC) $(CFLAGS) -o $@ main.c parson.c
	./$@

clean:
	rm -f Bot *.o
