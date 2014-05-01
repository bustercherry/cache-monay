CC=gcc
#CFLAGS=-c -g -Wall -Wextra
#LFLAGS=-g -Wall -Wextra
CFLAGS=-c -march=native -O2 -Wall -Wextra
LFLAGS=-g -march=native -O2 -Wall -Wextra
LYAML=-lyaml
LMATH=-lm

all: lru.o cache

cache: cache.c init.c lru.o output.o
	$(CC) $(LFLAGS) $^ -o $@ $(LYAML)

lru.o: lru.c
	$(CC) $(CFLAGS) $^ -o $@
	
output.o: output.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf cache *.o
