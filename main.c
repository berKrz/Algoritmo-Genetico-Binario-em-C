#include "ga.h"
#include "utils.h"
#include "args.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
  g_cfg = config_default();
  parse_args(argc, argv);

  srand(time(NULL));
  clear_screen();

  int *pop = malloc(g_cfg.pop_size * g_cfg.ind_size * sizeof(int));

  init(pop);
  wait_for_enter();

  for (int i = 0; i < g_cfg.generations; i++) {
    printf("\n\nIteração %d\n", i);
    g_cfg.selection_fn(pop);
    wait_for_enter();
    iter_pop(pop);
    wait_for_enter();
  }

  free(pop);
  return EXIT_SUCCESS;
}
