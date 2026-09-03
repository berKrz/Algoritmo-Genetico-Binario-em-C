#pragma once

typedef enum { MINIMIZE, MAXIMIZE } OptDirection;

typedef struct {
  int          pop_size;
  int          ind_size;
  int          generations;
  float        cut_point_ratio;
  float        mutation_rate;
  OptDirection direction;

  // Function Pointers
  int (*fitness_fn) (int *ind);
  void (*selection_fn)(int *pop);
  void (*crossover_fn)(int *i1, int *i2);
} Config;

extern Config g_cfg;

Config config_default(void);

int  fitness_quadratic     (int *ind);
void selection_roulette    (int *pop);
void crossover_single_point(int *i1, int *i2);
void iter_pop              (int *pop);
