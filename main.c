#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ga.h"
#include "utils.h"

int main() {
  srand(time(NULL));
  clear_screen();

  int pop[POP][IND];

  init(pop);
  wait_for_enter();

  for (int i = 0; i < 10; i++) {
    printf("\n\nIteração %d\n", i);
    roleta(pop);
    wait_for_enter();

    crossover_pop(pop);
  }

  return EXIT_SUCCESS;
}
