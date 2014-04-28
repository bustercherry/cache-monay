#include "cache.h"
#include <stdio.h>
#include "output.h"

output_t init_output(cache_t *one, cache_t *two, cache_t *three)
{
  output_t out;
  
  out.L1d = one;
  out.L1i = two;
  out.L2 = three;
  
  out.numRef = 0;
  out.numInst = 0;
  out.numRead = 0;
  out.numWrite = 0;
  out.numWriteCycles = 0;
  out.numReadCycles = 0;
  out.numInstCycles = 0;
  out.totalTime = 0;
  
  return out;
}

void print_cache(cache_t *cache)
{
  float total = cache->hits + cache->misses;
  printf("\nMemory level: %s\n", cache->name);
  printf("  Hits   =  %10d  [%0.2f%%]\n  Misses =  %10d  [%0.2f%%]\n  Total  =  %10d\n", 
         cache->hits, 100*(cache->hits/total), cache->misses, 
         100*(cache->misses/total), cache->hits + cache->misses);
  printf("  kickouts = %d, dirty kickouts = %d, transfers = %d\n", 
         cache->kickouts, cache->dirtyKickouts, cache->transfers);
}

void print_output(output_t out)
{
  float total = out.numRead + out.numWrite + out.numInst;
  
  printf("Number of reference types: \n");
  printf("Number of reads   = %10d  [%0.2f%%]\n", out.numRead,   100 * (out.numRead/total));
  printf("Number of writes  = %10d  [%0.2f%%]\n", out.numWrite,  100 * (out.numWrite/total));
  printf("Number of inst    = %10d  [%0.2f%%]\n", out.numInst,   100 * (out.numInst/total));
  printf("Total             = %10d\n", (int) total);
  
  total = (double) out.totalTime;
  printf("\nTotal cycles for all activities: \n");
  printf("Cycles for reads  =  %10d  [%0.2f%%]\n", out.numReadCycles, 100 * (out.numReadCycles/total));
  printf("Cycles for writes =  %10d  [%0.2f%%]\n", out.numWriteCycles, 100 * (out.numWriteCycles/total));
  printf("Cycles for inst   =  %10d  [%0.2f%%]\n", out.numInstCycles, 100 * (out.numInstCycles/total));
  printf("Total time        =  %10Lu\n", out.totalTime);
  
  print_cache(out.L1d);
  print_cache(out.L1i);
  print_cache(out.L2);
  
  
  int cost = 100 * (out.L1d->cacheSize/4096) + 100 * (out.L1d->assoc/2);
  printf("\nL1i cache cost = %d\n", cost);
  
  
  printf("\n");       
}
