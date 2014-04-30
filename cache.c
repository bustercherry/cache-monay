#include "cache.h"
#include "init.h"
#include "output.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#define DEBUG

cache_t L1d, L1i, L2;
output_t out;

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

  if(cache->lru[index]->current_size < cache->assoc)
  {
    *way = cache->lru[index]->current_size;
  }
  else
  {
    *way = remove_head(cache->lru[index]);
    cache->kickouts++;
     
    if(cache->entries[index][*way].dirty == 1)
    {
      time = calculate(cache->nextLevel, 'W', cache->entries[index][*way].address, cache->blockSize);
      cache->dirtyKickouts++;
    }
  }

  append_data(cache->lru[index], *way);

  cache->entries[index][*way].address = address;
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

  int time = 0, way = 0;
	unsigned long long tag = (address >> (64 - cache->tagSize));
	unsigned short index = (address >> cache->offsetSize) & cache->indexMask;

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
    #ifdef DEBUG
	  //printf("Ref Type = %s, Address = %Lx, Tag = %Lx, \n", getType(op), address, tag);
    //printf("Bytes = %d, Index = %d, Offset = %d, MISS\n", bytes, index, offset);
    printf("Level %s access addr = %Lx, reftype = %s\n", cache->name, address, getType(op));
    printf("    index = %x, tag = %Lx, MISS\n", index, tag);
    printf("Add %s miss time (+ %d)\n", cache->name, cache->missTime);
    printf("Add %s transfer time (+ %d)\n", cache->name, cache->transferTime);
    printf("Add %s hit time (+ %d)\n", cache->name, cache->hitTime);
    #endif
    
    cache->misses++;
    cache->transfers++;
  
    time = updateTag(cache, tag, index, address, &way)
         + cache->missTime 
         + cache->transferTime
         + cache->hitTime
         + cache->memTime
         + calculate(cache->nextLevel, 'R', address, bytes);
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
  
  //incCycles(op, tot);

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
  
  unsigned long long refTime = 0;

  while(scanf("%c %Lx %d\n", &op, &address, &bytes) == 3) 
  {
    #ifdef DEBUG
    printf("-------------------------------------------------------\n");
    printf("Ref %Lu: Addr = %Lx, Type = %c, BSize = %d\n", out.numRef, address, op, bytes);
    #endif
    
    switch(op)
    {
      case 'I':
        out.numInst++;
        refTime = (unsigned long long) splitReference(&L1i, op, address, bytes);
        out.totalTime += refTime;
        out.numInstCycles += refTime;
        break;
      case 'R':
        out.numRead++;
        refTime = (unsigned long long) splitReference(&L1d, op, address, bytes);
        out.totalTime += refTime;
        out.numReadCycles += refTime;
        break;
      case 'W':
        out.numWrite++;
        refTime = (unsigned long long) splitReference(&L1d, op, address, bytes);
        out.totalTime += refTime;
        out.numWriteCycles += refTime;
        break;
     }
        
    out.numRef++;
    
    #ifdef DEBUG
    printf("Simulated time = %Lu\n", out.totalTime);
    #endif
    
  }
  
  print_output(out);
  
  free_cache(&L1i);
  free_cache(&L1d);
  free_cache(&L2);
  
  return 0;
}
