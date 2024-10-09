#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  long long x,y,res;
  x = atoi(argv[1]);
  y = atoi(argv[2]);
  res = x - ~y - 1;
  printf("%d\n", res);
  
  return 0;
}
