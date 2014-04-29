#include "cache.h"
#include "init.h"
#include "output.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define HIT 1
#define MISS 0

//#define DEBUG

cache_t L1d, L1i, L2;
output_t out;

void incCount(char op)
{
  switch(op)
  {
    case 'I': out.numInst++;
              break;
    case 'R': out.numRead++;
              break;
    case 'W': out.numWrite++;
              break;
  }
}

void incCycles(char op, int value)
{
  switch(op)
  {
    case 'I':
      out.numInstCycles += value;
      break;
    case 'R':
      out.numReadCycles += value;
      break;
    case 'W':
      out.numWriteCycles += value;
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

int isHit(cache_t *cache, unsigned long long tag, unsigned short index, int *way)
{
  for(*way = 0; *way < cache->assoc; (*way)++)
  {
    if(cache->entries[index][*way].tag == tag)
    {
      append_data(cache->lru[index], *way);
      return 1;
    }
  }
  return 0;
}

int updateTag(cache_t *cache, unsigned long long tag, unsigned short index,
              unsigned long long address, int *way)
{
  int time = 0;
  *way = remove_head(cache->lru[index]);
  append_data(cache->lru[index], *way);

  if(cache->entries[index][*way].dirty)
    time = calculate(cache->nextLevel, 'W', address, cache->blockSize);

  cache->entries[index][*way].tag = tag;
  cache->entries[index][*way].dirty = 0;
  
  return time;
}

void setDirty(cache_t *cache, unsigned short index, int way)
{
  cache->entries[index][way].dirty = 1;
}

int calculate(cache_t *cache, char op, unsigned long long address, int bytes)
{
  if(cache == NULL) 
  {
    if(op == 'W')
      return L2.memTime;
    else
      return 0;
  }

  int time, way;
	volatile unsigned long long tag = (address >> (64 - cache->tagSize));
	volatile unsigned short index = (address >> cache->offsetSize) & cache->indexMask;

  if(isHit(cache, tag, index, &way))
  {
    #ifdef DEBUG
	  //printf("Ref Type = %s, Address = %Lx, Tag = %Lx, \n", getType(op), address, tag);
    //printf("Bytes = %d, Index = %d, Offset = %d, HIT\n", bytes, index, offset);
    printf("Level %s access addr = %Lx, reftype = %s\n", cache->name, address, getType(op));
    printf("    index = %x, tag = %Lx, HIT\n", index, tag);
    printf("Add %s hit time (+ %d)\n", cache->name, cache->hitTime);
    printf("Simulated time = %d\n", cache->hitTime + cache->transferTime);
    #endif
    
    cache->hits++;
    
    time = cache->hitTime + cache->transferTime;
  }
  else
  {
    int nextTime = calculate(cache->nextLevel, op, address, bytes);
    #ifdef DEBUG
	  //printf("Ref Type = %s, Address = %Lx, Tag = %Lx, \n", getType(op), address, tag);
    //printf("Bytes = %d, Index = %d, Offset = %d, MISS\n", bytes, index, offset);
    printf("Level %s access addr = %Lx, reftype = %s\n", cache->name, address, getType(op));
    printf("    index = %x, tag = %Lx, MISS\n", index, tag);
    printf("Add %s miss time (+ %d)\n", cache->name, cache->missTime);
    printf("Add %s transfer time (+ %d)\n", cache->name, cache->transferTime);
    printf("Add %s hit time (+ %d)\n", cache->name, cache->hitTime);
    printf("Simulated time = %d\n", cache->missTime
                                  + cache->transferTime
                                  + cache->hitTime
                                  + cache->memTime
                                  + nextTime);
    #endif

    
    
    cache->misses++;
    cache->transfers++;
    
    time = updateTag(cache, tag, index, address, &way)
         + cache->missTime 
         + cache->transferTime
         + cache->hitTime
         + cache->memTime
         + nextTime;
  }

  if(op == 'W') setDirty(cache, index, way);

  return time;
}

int splitReference(cache_t *cache, char op, unsigned long long address, int bytes)
{
  int tmpBytes = bytes, tot = 0, bytesTot = 0;
  
  #ifdef DEBUG
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
  
  incCycles(op, tot);

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

  out = init_output(&L1i, &L1d, &L2, argv[1]);
  
  char op;
  unsigned long long address;
  int bytes;

  while(scanf("%c %Lx %d\n", &op, &address, &bytes) == 3) 
  {
    incCount(op);
    
    #ifdef DEBUG
    printf("-------------------------------------------------------\n");
    printf("Ref %d: Addr = %Lx, Type = %c, BSize = %d\n", out.numRef, address, op, bytes);
    #endif
    
    out.totalTime += (unsigned long long) splitReference(getCache(op), op, address, bytes);
    
    #ifdef DEBUG
    printf("Total time so far: %Lu\n", out.totalTime);
    printf("-------------------------------------------------------\n");
    #endif
    
    out.numRef++;
  }
  
  print_output(out);
  
  return 0;
}
