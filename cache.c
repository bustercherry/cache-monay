#include "cache.h"
#include "init.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define HIT 1
#define MISS 0

#define DEBUG

cache_t L1d, L1i, L2;
int numRead, numWrite, numInst;
int numReadCycles, numWriteCycles, numInstCycles;

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

void incCycles(char op, int value)
{
  switch(op)
  {
    case 'I':
      numInstCycles += value;
      break;
    case 'R':
      numReadCycles += value;
      break;
    case 'W':
      numWriteCycles += value;
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
    {
      //append_data(cache->lru[index], way);
      return 1;
    }
  }
  return 0;
}

void updateTag(cache_t *cache, unsigned long long tag, unsigned short index)
{
  int value = remove_head(cache->lru[index]);
  append_data(cache->lru[index], value);
  cache->entries[index][value].tag = tag;
}

int calculate(cache_t *cache, char op, unsigned long long address, int bytes)
{
  if(cache == NULL) return 0;

	volatile unsigned long long tag = (address >> (64 - cache->tagSize));
	volatile unsigned short index = (address >> cache->offsetSize) & cache->indexMask;

  if(isHit(cache, tag, index))
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
    
    return cache->hitTime + cache->transferTime;
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
    printf("Simulated time = %d\n", cache->missTime + cache->transferTime + cache->hitTime + cache->memTime + nextTime);
    #endif

    updateTag(cache, tag, index);
    
    cache->misses++;
    cache->transfers++;
    
    return cache->missTime 
         + cache->transferTime
         + cache->hitTime
         + cache->memTime
         + nextTime;
  }
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

void print_cache(cache_t cache)
{
  float total = cache.hits + cache.misses;
  printf("  Hits   =  %10d  [%0.2f%%]\n  Misses =  %10d  [%0.2f%%]\n  Total  =  %10d\n", 
         cache.hits, 100*(cache.hits/total), cache.misses, 
         100*(cache.misses/total), cache.hits + cache.misses);
  printf("  kickouts = %d, dirty kickouts = %d, transfers = %d\n", 
         cache.kickouts, cache.dirtyKickouts, cache.transfers);
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
    
    #ifdef DEBUG
    printf("-------------------------------------------------------\n");
    printf("Ref %d: Addr = %Lx, Type = %c, BSize = %d\n", refNum, address, op, bytes);
    #endif
    
    totalTime += splitReference(getCache(op), op, address, bytes);
    
    #ifdef DEBUG
    printf("Total time so far: %d\n", totalTime);
    printf("-------------------------------------------------------\n");
    #endif
    
    refNum++;
  }
  
  float total = numRead + numWrite + numInst;
  
  printf("Number of reference types: \n");
  printf("Number of reads   = %10d  [%0.2f%%]\n", numRead,   100 * (numRead/total));
  printf("Number of writes  = %10d  [%0.2f%%]\n", numWrite,  100 * (numWrite/total));
  printf("Number of inst    = %10d  [%0.2f%%]\n", numInst,   100 * (numInst/total));
  printf("Total             = %10d\n", (int) total);
  
  total = (float) totalTime;
  printf("\nTotal cycles for all activities: \n");
  printf("Cycles for reads  =  %10d  [%0.2f%%]\n", numReadCycles, 100 * (numReadCycles/total));
  printf("Cycles for writes =  %10d  [%0.2f%%]\n", numWriteCycles, 100 * (numWriteCycles/total));
  printf("Cycles for inst   =  %10d  [%0.2f%%]\n", numInstCycles, 100 * (numInstCycles/total));
  printf("Total time        =  %10d\n", totalTime);
  
  printf("\nMemory Level: L1i \n");
  print_cache(L1i);
  printf("\nMemory Level: L1d \n");
  print_cache(L1d);
  printf("\nMemory Level: L2 \n");
  print_cache(L2);
  printf("\n");       
  

  return 0;
}
