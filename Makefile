CC=gcc
CFLAGS=-c -g -Wall -Wextra
LFLAGS=-g -Wall -Wextra
LYAML=-lyaml
LMATH=-lm

all: lru.o cache

cache: cache.c init.c lru.o
	$(CC) $(LFLAGS) $^ -o $@ $(LYAML)

lru.o: lru.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf cache *.o
