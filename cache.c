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
    L1d.tagMask       = 0x0;
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
