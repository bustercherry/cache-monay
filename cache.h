typedef struct
{
  int blockSize;
  int cacheSize;
  int assoc;
  int hitTime;
  int missTime;
  int transferTime;
  int busWidth;

  unsigned long long tagMask;
  unsigned long long indexMask;
  unsigned long long offsetMask;
  
  int tagSize;
  int indexSize;
  int offsetSize;

} cache_t;


typedef struct
{
	unsigned short index;
	unsigned long long tag;
	unsigned short valid;
	unsigned short dirty;
	
} cache_entry_t;


int calculateInstruction(cache_t cache, char op, unsigned long long address, int bytes);
int calculateRead(cache_t cache, char op, unsigned long long address, int bytes);
int calculateWrite(cache_t cache, char op, unsigned long long address, int bytes);
