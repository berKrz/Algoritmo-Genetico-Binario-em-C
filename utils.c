#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void clear_screen(void) {
  printf("\033[2J\033[H");
  fflush(stdout);
}

void wait_for_enter(void) {
  printf("\n\nAperte [Enter] para continuar...\n");
  while (getchar() != '\n');
}

void print_ind(int *ind) {
  for (int i = g_cfg.ind_size - 1; i >= 0; i--) {
    printf("[ %d ] ", ind[i]);
  }
}

void print_pop(int *pop) {
  for (int i = 0; i < g_cfg.pop_size; i++) {
    printf("\nIndivíduo %d = ", i);
    print_ind(pop + i * g_cfg.ind_size);
    printf("fitness(%d) = %d", i, g_cfg.fitness_fn(pop + i * g_cfg.ind_size));
    printf("\n");
  }
  printf("\n");
}

void print_pares(int *pop) {
  printf("Pares Selecionados");
  for (int i = 0; i < g_cfg.pop_size; i++) {
    printf("\n\nPar %d", i / 2);
    printf("\nIndivíduo %d = ", i);
    print_ind(pop + i * g_cfg.ind_size);
    printf("fitness(%d) = %d", i, g_cfg.fitness_fn(pop + i * g_cfg.ind_size));

    if (i == g_cfg.pop_size - 1) {
      break;
    }
    printf("\nIndivíduo %d = ", ++i);
    print_ind(pop + i * g_cfg.ind_size);
    printf("fitness(%d) = %d", i, g_cfg.fitness_fn(pop + i * g_cfg.ind_size));
  }
}

void init(int *pop) {
  for (int i = 0; i < g_cfg.pop_size; i++) {
    for (int j = 0; j < g_cfg.ind_size; j++) {
      pop[i * g_cfg.ind_size + j] = rand() % 2;
    }
  }
  printf("\nPopulação Inicial\n");
  print_pop(pop);
}

void copy_ind(int *i1, int *i2) {
  for (int i = 0; i < g_cfg.ind_size; i++) {
    i1[i] = i2[i];
  }
}
