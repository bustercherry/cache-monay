CC=gcc
CFLAGS=-Wall -Wextra

all: cache lru

cache: cache.c
	$(CC) $(CFLAGS) cache.c -o cache
	
lru: lru.c
  $(CC) $(CFLAGS) lru.c -o lru.o

clean:
	rm -rf cache lru
