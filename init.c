#include <stdlib.h>
#include "cache.h"
#include "init.h"
#include <yaml.h>

//#define DEBUG

extern cache_t L1d, L1i, L2;

void initCache(char *config_file)
{
  if(config_file == NULL)
    setDefaults();
  else
    parseConfig(config_file);

  L1i.name = "L1i";
  L1d.name = "L1d";
  L2.name  = "L2";

  L1i.nextLevel = &L2;
  L1d.nextLevel = &L2;
  L2.nextLevel = NULL;

  dynamicConfig(&L1i);
  dynamicConfig(&L1d);
  dynamicConfig(&L2);
}

unsigned int intLog2(unsigned int base)
{
  unsigned int exp = 0;
  while(base >>= 1) exp++;
  return exp;
}

void dynamicConfig(cache_t *cache)
{
  initEntries(cache);

  int numBlocks = cache->cacheSize/ (cache->blockSize * cache->assoc);
  unsigned int indexBits = intLog2(numBlocks);
  cache->indexMask = (unsigned long long)0xFFFFFFFFFFFFFFFF >> (64 - indexBits);
  cache->offsetSize = intLog2(cache->blockSize);
  cache->tagSize = 64 - indexBits - cache->offsetSize;
  
  cache->hits = 0;
  cache->misses = 0;
  cache->kickouts = 0;
  cache->dirtyKickouts = 0;
  cache->transfers = 0;

  #ifdef DEBUG
  printf("cacheSize: %d\tblockSize: %d\n", cache->cacheSize, cache->blockSize);
  printf("indexBits: %d\tindexMask: %Lx\n", indexBits, cache->indexMask);
  printf("offsetSize: %d\ttagSize: %d\n", cache->offsetSize, cache->tagSize);
  #endif

  int i;
  cache->lru = malloc(sizeof(lru_t*) * numBlocks);
  for(i = 0; i < numBlocks; i++)
  {
    cache->lru[i] = init_lru(cache->assoc);
  }

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
      cache->entries[set][way].address = 0xFFFFFFFFFFFFFFF;
      cache->entries[set][way].dirty = 0;
    }
  }
}

cache_t *configGetCache(char *name)
{
  #ifdef DEBUG
  printf("cache: %s\n", name);
  #endif

  if(strcmp(name, "L1i") == 0)
    return &L1i;
  if(strcmp(name, "L1d") == 0)
    return &L1d;
  if(strcmp(name, "L2") == 0)
    return &L2;

  return NULL;
}

void configSetVariable(cache_t *cache, char *key, char *value)
{
  #ifdef DEBUG
  printf("Setting %s = %s\n", key, value);
  #endif

  int val;
  sscanf(value, "%d", &val);

  if(strcmp(key, "blockSize") == 0)
    { cache->blockSize = val; return; }
  if(strcmp(key, "cacheSize") == 0)
    { cache->cacheSize = val; return; }
  if(strcmp(key, "assoc") == 0)
    { cache->assoc = val; return; }
  if(strcmp(key, "hitTime") == 0)
    { cache->hitTime = val; return; }
  if(strcmp(key, "missTime") == 0)
    { cache->missTime = val; return; }
  if(strcmp(key, "transferTime") == 0)
    { cache->transferTime = val; return; }
  if(strcmp(key, "memTime") == 0)
    { cache->memTime = val; return; }
  if(strcmp(key, "busWidth") == 0)
    { cache->busWidth = val; return; }
}

void parseConfig(char *file)
{
  FILE *F = fopen(file, "r");

  yaml_parser_t parser;
  yaml_token_t token;
  yaml_parser_initialize(&parser);
  yaml_parser_set_input_file(&parser, F);
  
  char *key;
  cache_t *cache;
  do {
    yaml_parser_scan(&parser, &token);
    switch(token.type)
    {
    case YAML_STREAM_START_TOKEN: key = NULL; break;
    case YAML_STREAM_END_TOKEN:   break;
    case YAML_KEY_TOKEN:          yaml_parser_scan(&parser, &token);
                                  key = strdup((char*)token.data.scalar.value);
                                  break;
    case YAML_VALUE_TOKEN:        yaml_parser_scan(&parser, &token);
                                  if(token.type == YAML_SCALAR_TOKEN)
                                  {
                                    configSetVariable(cache, key, (char *)token.data.scalar.value);
                                    break;
                                  }
    case YAML_BLOCK_MAPPING_START_TOKEN: if(key == NULL) break;
                                         cache = configGetCache(key);
                                         break;
    case YAML_BLOCK_END_TOKEN:    key = NULL; break;
    case YAML_SCALAR_TOKEN:  printf("Error: malformed configuration\n");
    default: printf("Unsupported token type\n");
    }
    if(token.type != YAML_STREAM_END_TOKEN)
      yaml_token_delete(&token);
  } while(token.type != YAML_STREAM_END_TOKEN);
  yaml_token_delete(&token);

  fclose(F);
}

void free_cache(cache_t *cache)
{
  
  int numSets = cache->cacheSize/(cache->blockSize * cache->assoc);

  int set;
  for(set = 0; set < numSets; set++)
  {
    free(cache->entries[set]);
  }
  
  free(cache->entries);
  
  int numBlocks = cache->cacheSize/(cache->blockSize * cache->assoc);

  int i;
  for(i = 0; i < numBlocks; i++)
  {
    destroy_lru(cache->lru[i]);
  }
  
  free(cache->lru);
  
}


void setDefaults()
{
  L1d.blockSize     =   32;
  L1d.cacheSize     = 8192;
  L1d.assoc         =    1;
  L1d.hitTime       =    1;
  L1d.missTime      =    1;
  L1d.transferTime  =    0;
  L1d.busWidth      =    0;
  L1d.memTime       =    0;

  /*
  L1d.indexMask     = 0x00000000000000FF;
  L1d.offsetMask    = 0x000000000000001F;
  
  L1d.tagSize       = 51;
  L1d.offsetSize    =  5;

  L1d.nextLevel = &L2;
  */

  L1i.blockSize     =   32;
  L1i.cacheSize     = 8192;
  L1i.assoc         =    1;
  L1i.hitTime       =    1;
  L1i.missTime      =    1;
  L1i.transferTime  =    0;
  L1i.busWidth      =    0;
  L1i.memTime       =    0;

  /*
  L1i.indexMask     = 0x00000000000000FF;
  L1i.offsetMask    = 0x000000000000001F;
  
  L1i.tagSize       = 51;
  L1i.offsetSize    =  5;

  L1i.nextLevel = &L2;
  */

  L2.blockSize      =    64;
  L2.cacheSize      = 32768;
  L2.assoc          =     1;
  L2.hitTime        =     5;
  L2.missTime       =     8;
  L2.transferTime   =    12;
  L2.busWidth       =    16;
  L2.memTime        =   140;

  /*
  L2.indexMask      = 0x00000000000001FF;
  L2.offsetMask     = 0x000000000000003F;
  
  L2.tagSize        = 49;
  L2.offsetSize     =  6;

  L2.nextLevel = NULL;
  */
}
