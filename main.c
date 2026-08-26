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

  roleta(pop);
  wait_for_enter();

  crossover_pop(pop);

  return EXIT_SUCCESS;
}
