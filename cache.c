#include "cache.h"
#include <stdio.h>

int main() {

  char op;
  unsigned long long address;
  int bytes;

  while(scanf("%c %Lx %d\n", &op, &address, &bytes) == 3) {
    printf("%c|%Lx|%d\n", op, address, bytes);
  }

  return 0;
}
