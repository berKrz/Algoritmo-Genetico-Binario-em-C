#include "ga.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fitness(int ind[IND]) {
   int n = 0;
 
   for (int i = 0; i < IND; i++) {
     n += (ind[i] * (1 << i)) ;
   }

  return n * n;
}

void roleta(int pop[][IND]) {
  int total = 0, fits[POP], aux[POP][IND];

  printf("Roleta\n");

  for (int i = 0; i < POP; i++) {
    copy_ind(aux[i], pop[i]);
  }

  for (int i = 0; i < POP; i++) {
    fits[i] = fitness(pop[i]);
    total += fits[i];
  }

  for (int i = 0; i < POP; i++) {
    int n = rand() % total, cumulativo = 0;

    for (int j = 0; j < POP; j++) {
      cumulativo += fits[j];

      if (n < cumulativo) {
        copy_ind(pop[i], aux[j]);
        break;
      }
    }
  }

  print_pares(pop);
}

void crossover(int i1[], int i2[]) {
  int corte = (IND * 6) / 10;

  for (int i = corte; i < IND; i++) {
    // XOR Swap Values without temporary variable trick
    i1[i] = i1[i] ^ i2[i];
    i2[i] = i2[i] ^ i1[i];
    i1[i] = i1[i] ^ i2[i];
  }
}

void crossover_pop(int pop[][IND]) {
  for (int i = 0; i < POP; i += 2) {
    crossover(pop[i], pop[i+1]);
  }

  printf("População Atual\n\n");
  print_pop(pop);
}
