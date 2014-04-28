CC=gcc
CFLAGS=-Wall -Wextra
LYAML=-lyaml
LMATH=-lm

all: lru.o cache

cache: cache.c init.c
	$(CC) $(CFLAGS) $^ -o $@ $(LYAML)

lru.o: lru.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf cache *.o
