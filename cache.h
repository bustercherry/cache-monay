typedef struct cache_entry_t cache_entry_t;
struct cache_entry_t
{
	unsigned long long tag;
	unsigned short valid;
	unsigned short dirty;
};

typedef struct cache_t cache_t;
struct cache_t
{
  int blockSize;
  int cacheSize;
  int assoc;
  int hitTime;
  int missTime;
  int transferTime;
  int busWidth;

  unsigned long long indexMask;
  unsigned long long offsetMask;
  
  int tagSize;
  int offsetSize;

  cache_entry_t **entries;
  cache_t *nextLevel;
};


int calculate(cache_t*, char, unsigned long long, int);
void incCount(char);
char *getType(char);
cache_t *getCache(char);
int isHit(cache_t*, unsigned long long, unsigned short);
void updateTag(cache_t*, unsigned long long, unsigned short);
int splitReference(cache_t*, char, unsigned long long, int);
