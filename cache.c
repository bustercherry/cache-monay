#include "cache.h"
#include "init.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define HIT 1
#define MISS 0

cache_t L1d, L1i, L2;

int numRead, numWrite, numInst, numHit, numMiss;

void incCount(char op)
{
  switch(op)
  {
    case 'I': numInst++;
              break;
    case 'R': numRead++;
              break;
    case 'W': numWrite++;
              break;
  }
}

char *getType(char op)
{
  switch(op)
  {
    case 'I': return "Inst";
    case 'R': return "Read";
    case 'W': return "Write";
  }
  return "Void";
}

cache_t *getCache(char op)
{
  switch(op)
  {
    case 'I': return &L1i;
    case 'R': return &L1d;
    case 'W': return &L1d;
  }
  return NULL;
}

int isHit(cache_t *cache, unsigned long long tag, unsigned short index)
{
  int way;
  for(way = 0; way < cache->assoc; way++)
  {
    if(cache->entries[index][way].tag == tag)
      return 1;
  }
  return 0;
}

void updateTag(cache_t *cache, unsigned long long tag, unsigned short index)
{
  /* needs lru magic */
  cache->entries[index][0].tag = tag;
}

int calculate(cache_t *cache, char op, unsigned long long address, int bytes)
{
  if(cache == NULL) return 0;

  incCount(op);

	volatile unsigned long long tag = (address >> (64 - cache->tagSize));
	volatile unsigned short index = (address >> cache->offsetSize) & cache->indexMask;
	volatile unsigned short offset = address & cache->offsetMask;

  if(isHit(cache, tag, index))
  {
	  printf("Ref Type = %s, Tag = %Lx, Index = %d, Offset = %d Hit = Yes\n", 
           getType(op), tag, index, offset);
    numHit++;
    return cache->hitTime;
  }
  else
  {
	  printf("Ref Type = %s, Tag = %Lx, Index = %d, Offset = %d Hit = No\n", 
           getType(op), tag, index, offset);
    updateTag(cache, tag, index);
    numMiss++;
    return cache->missTime + cache->transferTime 
         + calculate(cache->nextLevel, op, address, bytes);
  }
}

int splitReference(cache_t *cache, char op, unsigned long long address, int bytes)
{
	volatile unsigned short offset = address & cache->offsetMask;
  int tmpBytes, tot = 0;

  while(bytes + offset > cache->blockSize)
  {
    tmpBytes = (cache->blockSize - offset);
    tot += calculate(cache, op, address, tmpBytes);

    address += tmpBytes;
    offset = address & cache->offsetMask;
    bytes -= tmpBytes;
  }
  /* calculate last chunk */
  return tot + calculate(cache, op, address, bytes);
}

int main()
{
  initCache(NULL);
  unsigned long long totalTime;
  
  char op;
  unsigned long long address;
  int bytes;

  while(scanf("%c %Lx %d\n", &op, &address, &bytes) == 3) 
  {
    totalTime += (unsigned long long)splitReference(getCache(op), op, address, bytes);
  }
  
  float total = numRead + numWrite + numInst;
  
  printf("Number of reads:  %d    [%0.2f%%]\n", numRead, 100 * (numRead/total));
  printf("Number of writes: %d    [%0.2f%%]\n", numWrite, 100 * (numWrite/total));
  printf("Number of inst:   %d    [%0.2f%%]\n", numInst, 100 * (numInst/total));
  printf("Number of hits:   %d    [%0.2f%%]\n", numHit, 100 * (numHit/total));
  printf("Total execution time: %llu\n", totalTime);

  return 0;
}
