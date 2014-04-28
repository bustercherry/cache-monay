CC=gcc
CFLAGS=-Wall -Wextra

all: lru.o cache

cache: cache.c init.c
	$(CC) $(CFLAGS) $^ -o $@

lru.o: lru.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf cache *.o
