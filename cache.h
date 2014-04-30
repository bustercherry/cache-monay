#include "lru.h"

typedef struct cache_entry_t cache_entry_t;
struct cache_entry_t
{
	unsigned long long tag;
  unsigned long long address;
	unsigned short valid;
	unsigned short dirty;
};

typedef struct cache_t cache_t;
struct cache_t
{
  char *name;

  int blockSize;
  int cacheSize;
  int assoc;
  int hitTime;
  int missTime;
  int transferTime;
  int memTime;
  int busWidth;
  
  unsigned long long hits;
  unsigned long long misses;
  unsigned long long kickouts;
  unsigned long long dirtyKickouts;
  unsigned long long transfers;

  unsigned long long indexMask;
  
  int tagSize;
  int offsetSize;

  cache_entry_t **entries;
  cache_t *nextLevel;

  lru_t **lru;
};


int calculate(cache_t*, char, unsigned long long, int);
void incCount(char);
char *getType(char);
cache_t *getCache(char);
int isHit(cache_t*, unsigned long long, unsigned short, int*);
int updateTag(cache_t*, unsigned long long, unsigned short, unsigned long long, int*);
int splitReference(cache_t*, char, unsigned long long, int);
