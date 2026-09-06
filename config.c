#include "config.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- Strategy Lookup Tables --- */

typedef struct { const char *name; double (*fn)(double);    } FitnessEntry;
typedef struct { const char *name; void   (*fn)(int *);     } SelectionEntry;
typedef struct { const char *name; void   (*fn)(int *,int*);} CrossoverEntry;

static FitnessEntry fitness_table[] = {
  { "quadratic",    fitness_quadratic    },
  { NULL, NULL }
};

static SelectionEntry selection_table[] = {
  { "roulette",     selection_roulette   },
  { "tournament",   selection_tournament },
  { NULL, NULL }
};

static CrossoverEntry crossover_table[] = {
  { "single-point", crossover_single_point },
  { NULL, NULL }
};

/* --- Field Handlers --- */

static int g_domain_max_set = 0;

static void handle_pop_size(const char *value, Config *dst, const char *source) {
  int v = atoi(value);
  if (v < 2) die_at(source, "pop_size", "must be at least 2.");
  dst->pop_size = v;
}

static void handle_ind_size(const char *value, Config *dst, const char *source) {
  int v = atoi(value);
  if (v < 2) die_at(source, "ind_size", "must be at least 2.");
  dst->ind_size = v;
}

static void handle_generations(const char *value, Config *dst, const char *source) {
  int v = atoi(value);
  if (v < 1) die_at(source, "generations", "must be at least 1.");
  dst->generations = v;
}

static void handle_tournament_size(const char *value, Config *dst, const char *source) {
  int v = atoi(value);
  if (v < 2) die_at(source, "tournament_size", "must be at least 2.");
  dst->tournament_size = v;
}

static void handle_cut_point(const char *value, Config *dst, const char *source) {
  float v = (float)atof(value);
  if (v <= 0.0f || v >= 1.0f)
    die_at(source, "cut_point", "must be in the open interval (0.0, 1.0).");
  dst->cut_point_ratio = v;
}

static void handle_mutation_rate(const char *value, Config *dst, const char *source) {
  float v = (float)atof(value);
  if (v < 0.0f || v >= 1.0f)
    die_at(source, "mutation_rate", "must be in the interval [0.0, 1.0).");
  dst->mutation_rate = v;
}

static void handle_direction(const char *value, Config *dst, const char *source) {
  if      (strcmp(value, "minimize") == 0) dst->direction = MINIMIZE;
  else if (strcmp(value, "maximize") == 0) dst->direction = MAXIMIZE;
  else    die_at(source, "direction", "must be 'minimize' or 'maximize'.");
}

static void handle_fitness(const char *value, Config *dst, const char *source) {
  for (int i = 0; fitness_table[i].name != NULL; i++) {
    if (strcmp(value, fitness_table[i].name) == 0) {
      dst->fitness_fn = fitness_table[i].fn;
      return;
    }
  }
  char msg[256] = "unknown fitness function. Available:";
  for (int i = 0; fitness_table[i].name != NULL; i++) {
    strncat(msg, " ", sizeof(msg) - strlen(msg) - 1);
    strncat(msg, fitness_table[i].name, sizeof(msg) - strlen(msg) - 1);
  }
  strncat(msg, ".", sizeof(msg) - strlen(msg) - 1);
  die_at(source, "fitness", msg);
}

static void handle_selection(const char *value, Config *dst, const char *source) {
  for (int i = 0; selection_table[i].name != NULL; i++) {
    if (strcmp(value, selection_table[i].name) == 0) {
      dst->selection_fn = selection_table[i].fn;
      return;
    }
  }
  char msg[256] = "unknown selection function. Available:";
  for (int i = 0; selection_table[i].name != NULL; i++) {
    strncat(msg, " ", sizeof(msg) - strlen(msg) - 1);
    strncat(msg, selection_table[i].name, sizeof(msg) - strlen(msg) - 1);
  }
  strncat(msg, ".", sizeof(msg) - strlen(msg) - 1);
  die_at(source, "selection", msg);
}

static void handle_crossover(const char *value, Config *dst, const char *source) {
  for (int i = 0; crossover_table[i].name != NULL; i++) {
    if (strcmp(value, crossover_table[i].name) == 0) {
      dst->crossover_fn = crossover_table[i].fn;
      return;
    }
  }
  char msg[256] = "unknown crossover function. Available:";
  for (int i = 0; crossover_table[i].name != NULL; i++) {
    strncat(msg, " ", sizeof(msg) - strlen(msg) - 1);
    strncat(msg, crossover_table[i].name, sizeof(msg) - strlen(msg) - 1);
  }
  strncat(msg, ".", sizeof(msg) - strlen(msg) - 1);
  die_at(source, "crossover", msg);
}

static void handle_domain_min(const char *value, Config *dst, const char *source) {
  (void)source;
  dst->domain_min = atof(value);
}

static void handle_domain_max(const char *value, Config *dst, const char *source) {
  (void)source;
  dst->domain_max  = atof(value);
  g_domain_max_set = 1;
}

/* --- Field Dispatch Table --- */

typedef void (*field_handler_fn)(const char *value, Config *dst, const char *source);

typedef struct {
  const char      *name;
  field_handler_fn fn;
} FieldHandler;

static FieldHandler field_table[] = {
  { "pop_size",        handle_pop_size        },
  { "ind_size",        handle_ind_size        },
  { "generations",     handle_generations     },
  { "tournament_size", handle_tournament_size },
  { "cut_point",       handle_cut_point       },
  { "mutation_rate",   handle_mutation_rate   },
  { "direction",       handle_direction       },
  { "fitness",         handle_fitness         },
  { "selection",       handle_selection       },
  { "crossover",       handle_crossover       },
  { "domain_min",      handle_domain_min      },
  { "domain_max",      handle_domain_max      },
  { NULL, NULL }
};

/* --- apply_field --- */

void apply_field(const char *key, const char *value, Config *dst, const char *source) {
  // Normalize key: replace '-' with '_' so both forms are accepted
  char normalized[64];
  strncpy(normalized, key, sizeof(normalized) - 1);
  normalized[sizeof(normalized) - 1] = '\0';
  for (int i = 0; normalized[i]; i++) {
    if (normalized[i] == '-') normalized[i] = '_';
  }

  for (int i = 0; field_table[i].name != NULL; i++) {
    if (strcmp(normalized, field_table[i].name) == 0) {
      field_table[i].fn(value, dst, source);
      return;
    }
  }

  char msg[128];
  snprintf(msg, sizeof(msg), "unknown key '%s'.", key);
  die_at(source, NULL, msg);
}

int config_domain_max_was_set(void) {
  return g_domain_max_set;
}
