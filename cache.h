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

} cache_t;
