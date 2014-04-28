#include "cache.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define HIT 1
#define MISS 0

cache_t L1d, L1i, L2;

int numRead, numWrite, numInst;

void initCache(char *config_file)
{
  if(config_file == NULL)
  {
    L1d.blockSize     =   32;
    L1d.cacheSize     = 8192;
    L1d.assoc         =    1;
    L1d.hitTime       =    1;
    L1d.missTime      =    1;
    L1d.transferTime  =    0;
    L1d.busWidth      =    0;

    L1d.indexMask     = 0x00000000000000FF;
    L1d.offsetMask    = 0x000000000000001F;
    
    L1d.tagSize       = 51;
    L1d.offsetSize    =  5;

    L1d.entries = calloc(sizeof(cache_entry_t), L1d.cacheSize / L1d.blockSize);
    L1d.nextLevel = &L2;

    L1i.blockSize     =   32;
    L1i.cacheSize     = 8192;
    L1i.assoc         =    1;
    L1i.hitTime       =    1;
    L1i.missTime      =    1;
    L1i.transferTime  =    0;
    L1i.busWidth      =    0;

    L1i.indexMask     = 0x00000000000000FF;
    L1i.offsetMask    = 0x000000000000001F;
    
    L1i.tagSize       = 51;
    L1i.offsetSize    =  5;

    L1i.entries = calloc(sizeof(cache_entry_t), L1d.cacheSize / L1d.blockSize);
    L1i.nextLevel = &L2;

    L2.blockSize      =    64;
    L2.cacheSize      = 32768;
    L2.assoc          =     1;
    L2.hitTime        =     5;
    L2.missTime       =     8;
    L2.transferTime   =     6;
    L2.busWidth       =    16;

    L2.indexMask      = 0x00000000000001FF;
    L2.offsetMask     = 0x000000000000003F;
    
    L2.tagSize        = 49;
    L2.offsetSize     =  6;

    L2.entries = calloc(sizeof(cache_entry_t), L1d.cacheSize / L1d.blockSize);
    L2.nextLevel = NULL;
  }
}

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

int calculate(cache_t *cache, char op, unsigned long long address, int bytes)
{
  if(cache == NULL) return 0;

  incCount(op);

	volatile unsigned long long tag = (address >> (64 - cache->tagSize));
	volatile unsigned short index = (address >> cache->offsetSize) & cache->indexMask;
	volatile unsigned short offset = address & cache->offsetMask;

	printf("Ref Type = %s, Tag = %Lx, Index = %d, Offset = %d\n", getType(op), tag, index, offset);

  if(cache->entries[index].tag == tag)
    return cache->hitTime;
  else
    return cache->missTime + cache->transferTime 
         + calculateInstruction(cache->nextLevel, op, address, bytes);
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

int calculateInstruction(cache_t *cache, char op, unsigned long long address, int bytes)
{
  if(cache == NULL) return 0;

  numInst++;

	volatile unsigned long long tag = (address >> (64 - cache->tagSize));
	volatile unsigned short index = (address >> cache->offsetSize) & cache->indexMask;
	volatile unsigned short offset = address & cache->offsetMask;

	printf("Ref Type = Inst, Tag = %Lx, Index = %d, Offset = %d\n", tag, index, offset);

  if(cache->entries[index].tag == tag)
    return cache->hitTime;
  else
    return cache->missTime + cache->transferTime 
         + calculateInstruction(cache->nextLevel, op, address, bytes);
}

int calculateRead(cache_t *cache, char op, unsigned long long address, int bytes)
{
	volatile unsigned long long tag = (address >> (64 - cache->tagSize));
	volatile unsigned short index = (address >> cache->offsetSize) & cache->indexMask;
	volatile unsigned short offset = address & cache->offsetMask;

	printf("Ref Type = Read, Tag = %Lx, Index = %d, Offset = %d\n", tag, index, offset);

  numRead++;
	return MISS;
}

int calculateWrite(cache_t *cache, char op, unsigned long long address, int bytes)
{
	volatile unsigned long long tag = (address >> (64 - cache->tagSize));
	volatile unsigned short index = (address >> cache->offsetSize) & cache->indexMask;
	volatile unsigned short offset = address & cache->offsetMask;

	printf("Ref Type = Write, Tag = %Lx, Index = %d, Offset = %d\n", tag, index, offset);

  numWrite++;
	return MISS;
}

int main()
{
	
  initCache(NULL);
  
  char op;
  unsigned long long address;
  int bytes, ret;

  while(scanf("%c %Lx %d\n", &op, &address, &bytes) == 3) 
  {
  
    switch(op)
    {
      case 'I': 
        ret = splitReference(&L1i, op, address, bytes);
        //if(ret == MISS) calculateInstruction(L2, op, address, bytes);
        break;
      case 'R':
        ret = splitReference(&L1d, op, address, bytes);
        //if(ret == MISS) calculateRead(L2, op, address, bytes);
        break;
      case 'W':
        ret = splitReference(&L1d, op, address, bytes);
        //if(ret == MISS) calculateWrite(L2, op, address, bytes);
        break;
    }
    
  }
  
  float total = numRead + numWrite + numInst;
  
  printf("Number of reads:  %d    [%0.2f%%]   \n", numRead, 100 * (numRead/total));
  printf("Number of writes: %d     [%0.2f%%]   \n", numWrite, 100 * (numWrite/total));
  printf("Number of inst:   %d    [%0.2f%%]   \n", numInst, 100 * (numInst/total));

  return 0;
}
