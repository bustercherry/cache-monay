#include <stdlib.h>
#include "cache.h"
#include "init.h"

extern cache_t L1d, L1i, L2;

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

    L2.nextLevel = NULL;
  }

    initEntries(&L1i);
    initEntries(&L1d);
    initEntries(&L2);
}

void initEntries(cache_t *cache)
{
  int numSets = cache->cacheSize / (cache->blockSize * cache->assoc);
  cache->entries = malloc(sizeof(cache_entry_t*) * numSets);

  int set;
  for(set = 0; set < numSets; set++)
  {
    cache->entries[set] = malloc(sizeof(cache_entry_t) * cache->assoc);
    int way;
    for(way = 0; way < cache->assoc; way++)
    {
      cache->entries[set][way].tag = 0xFFFFFFFFFFFFFFFF;
    }
  }
}
