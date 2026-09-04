#pragma once

typedef enum { MINIMIZE, MAXIMIZE } OptDirection;

typedef struct {
  int          pop_size;
  int          ind_size;
  int          generations;
  int          tournament_size;
  float        cut_point_ratio;
  float        mutation_rate;
  double       domain_min;
  double       domain_max;
  OptDirection direction;

  // Function Pointers
  double (*decode_fn)    (int *ind);
  double (*fitness_fn)   (double x);
  void   (*selection_fn) (int *pop);
  void   (*crossover_fn) (int *i1, int *i2);
} Config;

extern Config g_cfg;

Config config_default(void);

double decode_linear       (int *ind);
double fitness_quadratic   (double x);

void selection_roulette    (int *pop);
void selection_tournament  (int *pop);
void crossover_single_point(int *i1, int *i2);
void iter_pop              (int *pop);
