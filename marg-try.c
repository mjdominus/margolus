

#include "margolus.h"

#define SIZE 100
int main(void) {
  MARG m = new(SIZE, SIZE);
  unsigned i;
  for (i = 0; i < 10000; i++) 
    iterate_neighborhoods(m, rot_random);
  return 0;
}
