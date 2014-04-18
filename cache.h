typedef struct
{
  int blockSize;
  int cacheSize;
  int assoc;
  int hitTime;
  int missTime;
  int transferTime;
  int busWidth;

  int tagMask;
  int indexMask;

} cache_t;
