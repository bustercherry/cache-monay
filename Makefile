CC=gcc
CFLAGS=-Wall -Wextra

all: cache

cache: cache.c
	$(CC) $(CFLAGS) cache.c -o cache

clean:
	rm -rf cache
