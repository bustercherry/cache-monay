#include "cache.h"
#include <stdio.h>
#include "output.h"
#include "init.h"

output_t init_output(cache_t *one, cache_t *two, cache_t *three, char* configName)
{
  output_t out;
  
  out.L1i = one;
  out.L1d = two;
  out.L2 = three;
  out.configName = configName;
  
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

int print_cost(cache_t *cache, int type)
{
  if(type == 0)
  {
    int nfk = (cache->cacheSize/4096);
    int cost = 100 * nfk + 100 * nfk * intLog2(cache->assoc);
    printf("  %s cache cost = $%d \n", cache->name, cost);
    return cost;
  }
  else
  {
    int nfk = (cache->cacheSize/32768);
    int cost = 50 * nfk + 50 * nfk * intLog2(cache->assoc);
    printf("  %s  cache cost = $%d \n", cache->name, cost);
    return cost;
  }
    
}

void print_cache(cache_t *cache)
{
  float total = cache->hits + cache->misses;
  printf("\nMemory level: %s\n", cache->name);
  printf("  Hits   =  %10Lu  [%0.2f%%]\n  Misses =  %10Lu  [%0.2f%%]\n  Total  =  %10Lu\n", 
         cache->hits, 100*(cache->hits/total), cache->misses, 
         100*(cache->misses/total), cache->hits + cache->misses);
  printf("  Kickouts = %Lu, Dirty kickouts = %Lu, Transfers = %Lu\n", 
         cache->kickouts, cache->dirtyKickouts, cache->transfers);
}

void print_output(output_t out)
{
  double total = (double) out.numRef;
  
  printf("Number of reference types: \n");
  printf("  Number of reads   =  %10Lu  [%0.2f%%]\n", out.numRead,   100 * (out.numRead/total));
  printf("  Number of writes  =  %10Lu  [%0.2f%%]\n", out.numWrite,  100 * (out.numWrite/total));
  printf("  Number of inst    =  %10Lu  [%0.2f%%]\n", out.numInst,   100 * (out.numInst/total));
  printf("  Total             =  %10Lu\n", (unsigned long long) total);
  
  total = (double) out.totalTime;
  printf("\nTotal cycles for all activities: \n");
  printf("  Cycles for reads  =  %10Lu  [%0.2f%%]\n", out.numReadCycles, 100 * (out.numReadCycles/total));
  printf("  Cycles for writes =  %10Lu  [%0.2f%%]\n", out.numWriteCycles, 100 * (out.numWriteCycles/total));
  printf("  Cycles for inst   =  %10Lu  [%0.2f%%]\n", out.numInstCycles, 100 * (out.numInstCycles/total));
  printf("  Total time        =  %10Lu\n", out.totalTime);
  
  printf("\nAverage cycles per activity:\n");
  printf("  Read  = %2.2f\n", (double) out.numReadCycles/out.numRead);
  printf("  Write = %2.2f\n", (double) out.numWriteCycles/out.numWrite);
  printf("  Inst  = %2.2f\n", (double) out.totalTime/out.numInst);
  
  unsigned long long ideal_time = out.numInst*2 + out.numRead + out.numWrite;
  unsigned long long misal_time = ideal_time + 
                                  (out.L1i->hits + out.L1i->misses) - out.numInst +
                                  (out.L1d->hits + out.L1d->misses) - out.numRead - out.numWrite;
  
  printf("  Ideal execution time   =  %7Lu  [CPI %2.2f]\n", ideal_time, (double) ideal_time/out.numInst);
  printf("  Ideal misaligned time  =  %7Lu  [CPI %2.2f]\n", misal_time, (double) misal_time/out.numInst);
  
  print_cache(out.L1i);
  print_cache(out.L1d);
  print_cache(out.L2);
  
  int totalCost = 0, memCost = 0, memChunkSize = 0;
  printf("\nCost analysis: \n");
  totalCost += print_cost(out.L1i, 0);
  totalCost += print_cost(out.L1d, 0);
  totalCost += print_cost(out.L2, 1);
  
  memChunkSize = (20 * 64)/(out.L2->memTime - 60);
  memCost = 50 + 100 * (memChunkSize/16/2) + 25;
  totalCost += memCost;
  
  printf("  Memory cost    = $%d\n", memCost);
  printf("  Total cost     = $%d\n", totalCost);
  
  printf("\n");       
}
