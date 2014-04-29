#include "cache.h"
#include <stdio.h>
#include "output.h"

output_t init_output(cache_t *one, cache_t *two, cache_t *three, char* configName)
{
  output_t out;
  
  out.L1d = one;
  out.L1i = two;
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
    int cost = 100 * nfk + 100 * nfk * (cache->assoc/2);
    printf("%s cache cost = $%d \n", cache->name, cost);
    return cost;
  }
  else
  {
    int nfk = (cache->cacheSize/65536);
    if(nfk == 0) nfk++;
    int cost = 50 * nfk + 50 * nfk * (cache->assoc/2);
    printf("%s cache cost = $%d \n", cache->name, cost);
    
    return cost;
  }
    
}

void print_cache(cache_t *cache)
{
  float total = cache->hits + cache->misses;
  printf("\nMemory level: %s\n", cache->name);
  printf("  Hits   =  %10d  [%0.2f%%]\n  Misses =  %10d  [%0.2f%%]\n  Total  =  %10d\n", 
         cache->hits, 100*(cache->hits/total), cache->misses, 
         100*(cache->misses/total), cache->hits + cache->misses);
  printf("  Kickouts = %d, Dirty kickouts = %d, Transfers = %d\n", 
         cache->kickouts, cache->dirtyKickouts, cache->transfers);
}

void print_output(output_t out)
{
  
  float total = out.numRead + out.numWrite + out.numInst;
  
  printf("Number of reference types: \n");
  printf("  Number of reads   = %10d  [%0.2f%%]\n", out.numRead,   100 * (out.numRead/total));
  printf("  Number of writes  = %10d  [%0.2f%%]\n", out.numWrite,  100 * (out.numWrite/total));
  printf("  Number of inst    = %10d  [%0.2f%%]\n", out.numInst,   100 * (out.numInst/total));
  printf("  Total             = %10d\n", (int) total);
  
  total = (double) out.totalTime;
  printf("\nTotal cycles for all activities: \n");
  printf("  Cycles for reads  =  %10d  [%0.2f%%]\n", out.numReadCycles, 100 * (out.numReadCycles/total));
  printf("  Cycles for writes =  %10d  [%0.2f%%]\n", out.numWriteCycles, 100 * (out.numWriteCycles/total));
  printf("  Cycles for inst   =  %10d  [%0.2f%%]\n", out.numInstCycles, 100 * (out.numInstCycles/total));
  printf("  Total time        =  %10Lu\n", out.totalTime);
  
  printf("\nAverage cycles per activity:\n");
  printf("  Read  = %2.2f\n", (double) out.numReadCycles/out.numRead);
  printf("  Write = %2.2f\n", (double) out.numWriteCycles/out.numWrite);
  printf("  Inst  = %2.2f\n", (double) out.numInstCycles/out.numInst);
  
  print_cache(out.L1d);
  print_cache(out.L1i);
  print_cache(out.L2);
  
  int totalCost = 0;
  printf("\n");
  totalCost += print_cost(out.L1d, 0);
  totalCost += print_cost(out.L1i, 0);
  totalCost += print_cost(out.L2, 1);
  totalCost += 75;
  
  printf("Memory cost = $75\n");
  printf("Total cost = $%d\n", totalCost);
  
  printf("\n");       
}
