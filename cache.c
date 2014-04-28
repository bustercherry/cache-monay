#include "cache.h"
#include "init.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define HIT 1
#define MISS 0

#define DEBUG 1

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

	volatile unsigned long long tag = (address >> (64 - cache->tagSize));
	volatile unsigned short index = (address >> cache->offsetSize) & cache->indexMask;
	volatile unsigned short offset = address & cache->offsetMask;

  if(isHit(cache, tag, index))
  {
    #if DEBUG
	  printf("Ref Type = %s, Address = %Lx, Tag = %Lx, \n", getType(op), address, tag);
    printf("Bytes = %d, Index = %d, Offset = %d, HIT\n", bytes, index, offset);
    #endif
    numHit++;
    return cache->hitTime + cache->transferTime;
  }
  else
  {
    #if DEBUG
	  printf("Ref Type = %s, Address = %Lx, Tag = %Lx, \n", getType(op), address, tag);
    printf("Bytes = %d, Index = %d, Offset = %d, MISS\n", bytes, index, offset);
    #endif
    updateTag(cache, tag, index);
    numMiss++;
    return cache->missTime + cache->transferTime + cache->hitTime + cache->memTime
         + calculate(cache->nextLevel, op, address, bytes);
  }
}

int splitReference(cache_t *cache, char op, unsigned long long address, int bytes)
{
	//volatile unsigned short offset = address & 0xF;
  int tmpBytes = bytes, tot = 0, bytesTot = 0;
  
  #if DEBUG
  printf("Unmodified address: %Lx\n", address);
  #endif

  while(tmpBytes > 0)
  {
    if(address % 4 != 0)
    {
      address -= 1;
      bytesTot++;
    }
    else
    {
      if(bytesTot > 0)
      {
        tmpBytes -= (4 - bytesTot);
        tot += calculate(cache, op, address, bytes);
        bytes -= (4 - bytesTot);
        bytesTot = 0;
        address += 4;
      }
      else
      {
        tot += calculate(cache, op, address, bytes);
        tmpBytes -= 4;
        bytes -= 4;
        address += 4;
      }
    }
  }

  //while(bytes + offset > 4)
  //{
    //tmpBytes = (cache->blockSize - offset);
    
    //tot += calculate(cache, op, address, tmpBytes);

    //address += tmpBytes;
    //offset = address & cache->offsetMask;
    //bytes -= tmpBytes;
  //}
  
  
  /* calculate last chunk */
  return tot;
}

int main(int argc, char *argv[])
{
  if(argc == 2)
    initCache(argv[1]);
  else if(argc == 1)
    initCache(NULL);
  else
    { printf("Bad argument(s)\n"); return 100; }

  int totalTime = 0;
  int refNum = 0;
  
  char op;
  unsigned long long address;
  int bytes;

  while(scanf("%c %Lx %d\n", &op, &address, &bytes) == 3) 
  {
    incCount(op);
    
    #if DEBUG
    printf("-------------------------------------------------------\n");
    printf("Reference #%d\n", refNum);
    #endif
    
    totalTime += splitReference(getCache(op), op, address, bytes);
    
    #if DEBUG
    printf("Total time so far: %d\n", totalTime);
    printf("-------------------------------------------------------\n");
    #endif
    
    refNum++;
  }
  
  float total = numRead + numWrite + numInst;
  
  printf("Number of reads:  %d    [%0.2f%%]\n", numRead, 100 * (numRead/total));
  printf("Number of writes: %d    [%0.2f%%]\n", numWrite, 100 * (numWrite/total));
  printf("Number of inst:   %d    [%0.2f%%]\n", numInst, 100 * (numInst/total));
  printf("Number of hits:   %d    [%0.2f%%]\n", numHit, 100 * (numHit/total));
  printf("Total execution time: %d\n", totalTime);

  return 0;
}
