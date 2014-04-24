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

  cache_entry_t *entries;
  cache_t *nextLevel;
};


int calculateInstruction(cache_t *cache, char op, unsigned long long address, int bytes);
int calculateRead(cache_t *cache, char op, unsigned long long address, int bytes);
int calculateWrite(cache_t *cache, char op, unsigned long long address, int bytes);
