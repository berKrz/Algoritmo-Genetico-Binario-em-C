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

  struct {
    int    *ind;
    double  fitness;
    int     generation;
    int     index;
  } best_ever = {0};
  best_ever.ind = malloc(g_cfg.ind_size * sizeof(int));

  init(pop);
  wait_for_enter();
  print_separator();

  for (int i = 0; i < g_cfg.generations; i++) {
    print_generation_header(i);
    g_cfg.selection_fn(pop);
    wait_for_enter();
    print_separator();
    iter_pop(pop);
    wait_for_enter();
    print_separator();

    // Track best ever across generations
    int    best_idx = find_best(pop);
    double best_fit = g_cfg.fitness_fn(g_cfg.decode_fn(pop + best_idx * g_cfg.ind_size));
    int is_better   = (i == 0) ||
                      (g_cfg.direction == MINIMIZE ? best_fit < best_ever.fitness
                                                   : best_fit > best_ever.fitness);
    if (is_better) {
      copy_ind(best_ever.ind, pop + best_idx * g_cfg.ind_size);
      best_ever.fitness    = best_fit;
      best_ever.generation = i;
      best_ever.index      = best_idx;
    }
  }

  // Results
  int    last_idx = find_best(pop);
  double last_x   = g_cfg.decode_fn(pop + last_idx * g_cfg.ind_size);
  double last_fit = g_cfg.fitness_fn(last_x);
  double ever_x   = g_cfg.decode_fn(best_ever.ind);
  double ever_fit = g_cfg.fitness_fn(ever_x);

  for (int i = 0; i < LINE_WIDTH; i++) printf("=");
  printf("\n");
  char rbuf[32];
  int  rn = snprintf(rbuf, sizeof(rbuf), "=== Resultado Final ");
  printf("%s", rbuf);
  for (int i = rn; i < LINE_WIDTH; i++) printf("=");
  printf("\n\n");

  printf("Melhor da ultima geracao (geracao %d):\n", g_cfg.generations - 1);
  printf("   Individuo %d\n", last_idx);
  printf("     ");
  print_ind(pop + last_idx * g_cfg.ind_size);
  printf("\n");
  printf("     x = %g   fitness = %g\n\n", last_x, last_fit);

  printf("Melhor geral (geracao %d):\n", best_ever.generation);
  printf("   Individuo %d\n", best_ever.index);
  printf("     ");
  print_ind(best_ever.ind);
  printf("\n");
  printf("     x = %g   fitness = %g\n\n", ever_x, ever_fit);

  for (int i = 0; i < LINE_WIDTH; i++) printf("=");
  printf("\n");

  free(best_ever.ind);
  free(pop);
  return EXIT_SUCCESS;
}
