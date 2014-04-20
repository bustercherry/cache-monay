#include "cache.h"
#include <stdio.h>

#define HIT 1
#define MISS 0

cache_t L1d, L1i, L2;

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

	L1d.tagMask       = 0x0007FFFFFFFFFFFF;
    L1d.indexMask     = 0x00000000000000FF;
    L1d.offsetMask    = 0x000000000000001F;
    
    L1d.tagSize = 51;
    L1d.indexSize = 8;
    L1d.offsetSize = 5;

    L1i.blockSize     =   32;
    L1i.cacheSize     = 8192;
    L1i.assoc         =    1;
    L1i.hitTime       =    1;
    L1i.missTime      =    1;
    L1i.transferTime  =    0;
    L1i.busWidth      =    0;

    L1i.tagMask       = 0x0007FFFFFFFFFFFF;
    L1i.indexMask     = 0x00000000000000FF;
    L1i.offsetMask    = 0x000000000000001F;
    
    L1i.tagSize = 51;
    L1i.indexSize = 8;
    L1i.offsetSize = 5;

    L2.blockSize     =    64;
    L2.cacheSize     = 32768;
    L2.assoc         =     1;
    L2.hitTime       =     5;
    L2.missTime      =     8;
    L2.transferTime  =     6;
    L2.busWidth      =    16;

    L2.tagMask       = 0x0001FFFFFFFFFFFF;
    L2.indexMask     = 0x00000000000001FF;
    L2.offsetMask    = 0x000000000000003F;
    
    L2.tagSize = 49;
    L2.indexSize = 9;
    L2.offsetSize = 6;
  }
}

int calculateCache(cache_t cache, char op, unsigned long long address, int bytes)
{
  switch(op)
  {
    case 'I': 
      return calculateInstruction(cache, op, address, bytes);
    case 'R':
      return calculateRead(cache, op, address, bytes);
    case 'W':
      return calculateWrite(cache, op, address, bytes);
	default:
	  return 0;
  }
}

int calculateInstruction(cache_t cache, char op, unsigned long long address, int bytes)
{
	unsigned long long tag = (address >> (64 - cache.tagSize));
	unsigned long long index = (address >> cache.offsetSize) & cache.indexMask;
	unsigned long long offset = address & cache.offsetMask;
	
	
	printf("Cache tag size = %d\n", cache.tagSize);
	printf("Address = %Lx\n", address >> (64 - cache.tagSize));
	printf("Tag = %Lx, Index = %Lx, Offset = %Lx\n", tag, index, offset);
	
	
	return MISS;
}

int calculateRead(cache_t cache, char op, unsigned long long address, int bytes)
{
	return MISS;
}

int calculateWrite(cache_t cache, char op, unsigned long long address, int bytes)
{
	return MISS;
}

int main()
{
	
  initCache(NULL);
  unsigned long long add = 0x3c1ee00b00;
  
  calculateCache(L1d, 'I', add, 3);
  calculateCache(L2, 'I', add, 3);
  

  //char op;
  //unsigned long long address;
  //int bytes;

  //while(scanf("%c %Lx %d\n", &op, &address, &bytes) == 3) 
  //{

  //}

  return 0;
}
