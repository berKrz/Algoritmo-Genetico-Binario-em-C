#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void clear_screen(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
}

void print_ind(int ind[IND]) {
  for (int i = IND-1; i > -1; i--) {
    printf("[ %d ] ", ind[i]);
  }
}

void print_pop(int pop[POP][IND]) {
  printf("\nPopulação Atual\n");

  for (int i = 0; i < POP; i++) {
    printf("\nIndivíduo %d = ", i);
    print_ind(pop[i]);
    printf("f(%d) = %d", i, f(pop[i]));
    printf("\n");
  }
  printf("\n");
}

void init(int matriz[POP][IND]) {
  for (int i = 0; i < POP; i++) {
    for (int j = 0; j < IND; j++) {
      matriz[i][j] = rand() % 2;
    }
  }
}
