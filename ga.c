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

void crossover_pop(int *pop) {
  int ind_size = g_cfg.ind_size;
  for (int i = 0; i < (g_cfg.pop_size / 2) * 2; i += 2) {
    g_cfg.crossover_fn(pop + i * ind_size, pop + (i + 1) * ind_size);
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
    .direction       = MAXIMIZE,
    .fitness_fn      = fitness_quadratic,
    .selection_fn    = selection_roulette,
    .crossover_fn    = crossover_single_point,
  };
}
