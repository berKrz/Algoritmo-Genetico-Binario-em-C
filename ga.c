#include "ga.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

Config g_cfg;

int fitness_quadratic(int *ind) {
  int n = 0;
  for (int i = 0; i < g_cfg.ind_size; i++) {
      n += ind[i] * (1 << i);
  }
  return n * n;
}

static void mutation(int *ind, int index) {
  for (int i = 0; i < g_cfg.ind_size; i++) {
    int n = rand() % 100;
    // RAND_MAX resolve erros por um (44.99999 vira 44)
    if ((float)rand() / RAND_MAX < g_cfg.mutation_rate){
      printf("\nMutação no indivíduo %d, bit %d\n", index,i);
      print_ind(ind);
      ind[i] = !ind[i];
      printf("  ->  ");
      print_ind(ind);
      printf("\n");
    }
  }
}

void crossover_single_point(int *i1, int *i2) {
  int cut = (int)(g_cfg.ind_size * g_cfg.cut_point_ratio);
  for (int i = cut; i < g_cfg.ind_size; i++) {
    // XOR Swap Values without temporary variable trick
    i1[i] = i1[i] ^ i2[i];
    i2[i] = i2[i] ^ i1[i];
    i1[i] = i1[i] ^ i2[i];
  }
}

void selection_roulette(int *pop) {
  int pop_size = g_cfg.pop_size;
  int ind_size = g_cfg.ind_size;

  int *fits = malloc(pop_size * sizeof(int));
  int *aux  = malloc(pop_size * ind_size * sizeof(int));

  printf("Roleta\n");

  for (int i = 0; i < pop_size; i++) {
    copy_ind(aux + i * ind_size, pop + i * ind_size);
    fits[i] = g_cfg.fitness_fn(pop + i * ind_size);
  }

  int total = 0;

  if (g_cfg.direction == MAXIMIZE) {
    for (int i = 0; i < pop_size; i++) {
        total += fits[i];
    }
  } else {
    int max_fit = fits[0];
    for (int i = 1; i < pop_size; i++) {
        if (fits[i] > max_fit) max_fit = fits[i];
    }
    for (int i = 0; i < pop_size; i++) {
        fits[i] = max_fit - fits[i] + 1;
        total += fits[i];
    }
  }

  for (int i = 0; i < pop_size; i++) {
    int n = rand() % total, cumulative = 0;
    for (int j = 0; j < pop_size; j++) {
      cumulative += fits[j];
      if (n < cumulative) {
        copy_ind(pop + i * ind_size, aux + j * ind_size);
        break;
      }
    }
  }

  free(fits);
  free(aux);

  print_pares(pop);
}

void iter_pop(int *pop) {
  int ind_size = g_cfg.ind_size;
  for (int i = 0; i < g_cfg.pop_size; i++) {
    if (i != g_cfg.pop_size - 1 && i % 2 == 0) {
      g_cfg.crossover_fn(pop + i * ind_size, pop + (i + 1) * ind_size);
    }
    mutation(pop + i * ind_size, i);
  }
  printf("População Atual\n\n");
  print_pop(pop);
}

Config config_default(void) {
  return (Config){
    .pop_size        = 4,
    .ind_size        = 5,
    .generations     = 10,
    .cut_point_ratio = 0.6f,
    .mutation_rate   = 0.0f,
    .direction       = MAXIMIZE,
    .fitness_fn      = fitness_quadratic,
    .selection_fn    = selection_roulette,
    .crossover_fn    = crossover_single_point,
  };
}
