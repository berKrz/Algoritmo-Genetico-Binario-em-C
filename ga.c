#include "ga.h"

int f(int ind[IND]) {
   int n = 0;
 
   for (int i = 0; i < IND; i++) {
     n += (ind[i] * (1 << i)) ;
   }

  return n * n;
 }
