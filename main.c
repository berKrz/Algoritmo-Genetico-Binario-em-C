#include "ga.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  g_cfg = config_default();

  srand(time(NULL));
  clear_screen();

  int *pop = malloc(g_cfg.pop_size * g_cfg.ind_size * sizeof(int));

  init(pop);
  wait_for_enter();

  for (int i = 0; i < g_cfg.generations; i++) {
    printf("\n\nIteração %d\n", i);
    g_cfg.selection_fn(pop);
    wait_for_enter();
    crossover_pop(pop);
    wait_for_enter();
  }

  free(pop);
  return EXIT_SUCCESS;
}
