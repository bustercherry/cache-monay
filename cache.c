#include "cache.h"
#include <stdio.h>

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

    L1d.tagMask       = 0xFFFFFFFFFFFFE000;
    L1d.indexMask     = 0x0000000000001FE0;
    L1d.offsetMask    = 0x000000000000001F;

    L1i.blockSize     =   32;
    L1i.cacheSize     = 8192;
    L1i.assoc         =    1;
    L1i.hitTime       =    1;
    L1i.missTime      =    1;
    L1i.transferTime  =    0;
    L1i.busWidth      =    0;

    L1i.tagMask       = 0xFFFFFFFFFFFFE000;
    L1i.indexMask     = 0x0000000000001FE0;
    L1i.offsetMask    = 0x000000000000001F;

    L2.blockSize     =    64;
    L2.cacheSize     = 32768;
    L2.assoc         =     1;
    L2.hitTime       =     5;
    L2.missTime      =     8;
    L2.transferTime  =     6;
    L2.busWidth      =    16;

    L2.tagMask       = 0xFFFFFFFFFFFF8000;
    L2.indexMask     = 0x0000000000007FC0;
    L2.offsetMask    = 0x000000000000003F;
  }
}

int calculateCache(char op, unsigned long long address, int bytes)
{
  switch(op)
    case 'I': 
      return calculateInstruction(op, address, bytes);
    case 'R':
      return calculateRead(op, address, bytes);
    case 'W':
      return calculateWrite(op, address, bytes);
}

int main()
{

  char op;
  unsigned long long address;
  int bytes;

  while(scanf("%c %Lx %d\n", &op, &address, &bytes) == 3) {

  }

  return 0;
}
