#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void die(const char *msg) {
  fprintf(stderr, "Error: %s\n", msg);
  fprintf(stderr, "Run with --help for usage.\n");
  exit(EXIT_FAILURE);
}

void die_at(const char *source, const char *key, const char *msg) {
  if (key)
    fprintf(stderr, "Error in '%s' [%s]: %s\n", source, key, msg);
  else
    fprintf(stderr, "Error in '%s': %s\n", source, msg);
  fprintf(stderr, "Run with --help for usage.\n");
  exit(EXIT_FAILURE);
}

void clear_screen(void) {
  printf("\033[2J\033[H");
  fflush(stdout);
}

void wait_for_enter(void) {
  if (!g_cfg.interactive) return;
  printf("\n  [Enter para continuar...]\n");
  while (getchar() != '\n');
}

void print_separator(void) {
  printf("\n");
  for (int i = 0; i < LINE_WIDTH / 2; i++) printf("- ");
  printf("\n\n");
}

void print_generation_header(int gen) {
  char buf[32];
  int  n = snprintf(buf, sizeof(buf), "=== Geracao %d ", gen);
  printf("%s", buf);
  for (int i = n; i < LINE_WIDTH; i++) printf("=");
  printf("\n\n");
}

void print_ind(int *ind) {
  for (int i = g_cfg.ind_size - 1; i >= 0; i--) {
    printf("[ %d ]", ind[i]);
  }
}

int find_best(int *pop) {
  int    best_idx = 0;
  double best_fit = g_cfg.fitness_fn(g_cfg.decode_fn(pop));
  for (int i = 1; i < g_cfg.pop_size; i++) {
    double f = g_cfg.fitness_fn(g_cfg.decode_fn(pop + i * g_cfg.ind_size));
    if (g_cfg.direction == MINIMIZE ? f < best_fit : f > best_fit) {
      best_fit = f;
      best_idx = i;
    }
  }
  return best_idx;
}

void print_pop(int *pop) {
  int best_idx = find_best(pop);

  for (int i = 0; i < g_cfg.pop_size; i++) {
    double x = g_cfg.decode_fn(pop + i * g_cfg.ind_size);
    double f = g_cfg.fitness_fn(x);

    if (i == best_idx)
      printf(">> Individuo %d\n", i);
    else
      printf("   Individuo %d\n", i);

    printf("     ");
    print_ind(pop + i * g_cfg.ind_size);
    printf("\n");
    printf("     x = %g   fitness = %g\n\n", x, f);
  }
}

void print_pares(int *pop) {
  for (int i = 0; i < g_cfg.pop_size; i++) {
    int is_odd_last = (i == g_cfg.pop_size - 1) && (g_cfg.pop_size % 2 != 0);

    if (is_odd_last)
      printf("  Par %d  (sem par)\n", i / 2);
    else
      printf("  Par %d\n", i / 2);

    double x = g_cfg.decode_fn(pop + i * g_cfg.ind_size);
    printf("    Individuo %d\n", i);
    printf("      ");
    print_ind(pop + i * g_cfg.ind_size);
    printf("\n");
    printf("      x = %g   fitness = %g\n", x, g_cfg.fitness_fn(x));

    if (is_odd_last) {
      printf("\n");
      break;
    }

    ++i;
    x = g_cfg.decode_fn(pop + i * g_cfg.ind_size);
    printf("    Individuo %d\n", i);
    printf("      ");
    print_ind(pop + i * g_cfg.ind_size);
    printf("\n");
    printf("      x = %g   fitness = %g\n\n", x, g_cfg.fitness_fn(x));
  }
}

void init(int *pop) {
  for (int i = 0; i < g_cfg.pop_size; i++) {
    for (int j = 0; j < g_cfg.ind_size; j++) {
      pop[i * g_cfg.ind_size + j] = rand() % 2;
    }
  }
  printf("Populacao Inicial\n\n");
  print_pop(pop);
}

void copy_ind(int *i1, int *i2) {
  for (int i = 0; i < g_cfg.ind_size; i++) {
    i1[i] = i2[i];
  }
}
