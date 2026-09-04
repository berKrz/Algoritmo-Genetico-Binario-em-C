#include "args.h"
#include "ga.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

// Lookup Tables
typedef struct { const char *name; double (*fn)(double);    } FitnessEntry;
typedef struct { const char *name; void (*fn)(int *);       } SelectionEntry;
typedef struct { const char *name; void (*fn)(int *, int *);} CrossoverEntry;

static FitnessEntry fitness_table[] = {
  { "quadratic",    fitness_quadratic },
};

static SelectionEntry selection_table[] = {
  { "roulette",     selection_roulette   },
  { "tournament",   selection_tournament },
};

static CrossoverEntry crossover_table[] = {
  { "single-point", crossover_single_point },
};

// Helpers
static void print_help(const char *prog) {
  printf("Usage: %s [OPTIONS]\n\n", prog);
  printf("Options:\n");
  printf("  -p, --pop-size          INT    Population size (min: 2)            [default: 15]\n");
  printf("  -i, --ind-size          INT    Individual size (min: 2)            [default: 22]\n");
  printf("  -g, --generations       INT    Number of generations (min: 1)      [default: 30]\n");
  printf("  -k, --tournament-size   INT    Number of individuals (min: 2)      [default: 2]\n");
  printf("  -c, --cut-point         FLOAT  Crossover cut point ratio (0.0,1.0) [default: 0.6]\n");
  printf("  -m, --mutation-rate     FLOAT  Mutation ratio [0.0,1.0)            [default: 0.01]\n");
  printf("  -d, --direction         STR    minimize | maximize                 [default: minimize]\n");
  printf("  -f, --fitness           STR    quadratic                           [default: quadratic]\n");
  printf("  -s, --selection         STR    roulette | tournament               [default: roulette]\n");
  printf("  -x, --crossover         STR    single-point                        [default: single-point]\n");
  printf("      --domain-min        FLOAT  Domain minimum value                [default: 0.0]\n");
  printf("      --domain-max        FLOAT  Domain maximum value                [default: 2^ind-size - 1]\n");
  printf("  -h, --help                     Print this message and exit\n");
}

static void die(const char *msg) {
  fprintf(stderr, "Error: %s\n", msg);
  fprintf(stderr, "Run with --help for usage.\n");
  exit(EXIT_FAILURE);
}

/* --- parse_args --- */

void parse_args(int argc, char **argv) {
  static const struct option long_opts[] = {
    { "pop-size",        required_argument, NULL, 'p'  },
    { "ind-size",        required_argument, NULL, 'i'  },
    { "generations",     required_argument, NULL, 'g'  },
    { "tournament-size", required_argument, NULL, 'k'  },
    { "cut-point",       required_argument, NULL, 'c'  },
    { "mutation-rate",   required_argument, NULL, 'm'  },
    { "direction",       required_argument, NULL, 'd'  },
    { "fitness",         required_argument, NULL, 'f'  },
    { "selection",       required_argument, NULL, 's'  },
    { "crossover",       required_argument, NULL, 'x'  },
    { "domain-min",      required_argument, NULL, 1000 },
    { "domain-max",      required_argument, NULL, 1001 },
    { "help",            no_argument,       NULL, 'h'  },
    { NULL,              0,                 NULL,  0   }
  };

  int domain_max_explicit = 0;

  int opt;
  while ((opt = getopt_long(argc, argv, "p:i:g:k:c:m:d:f:s:x:h", long_opts, NULL)) != -1) {
    switch (opt) {
      case 'p': {
        int v = atoi(optarg);
        if (v < 2) die("--pop-size must be at least 2.");
        g_cfg.pop_size = v;
        break;
      }
      case 'i': {
        int v = atoi(optarg);
        if (v < 2) die("--ind-size must be at least 2.");
        g_cfg.ind_size = v;
        break;
      }
      case 'g': {
        int v = atoi(optarg);
        if (v < 1) die("--generations must be at least 1.");
        g_cfg.generations = v;
        break;
      }
      case 'k': {
        int v = atoi(optarg);
        if (v < 2) die("--tournament-size must be at least 2.");
        g_cfg.tournament_size = v;
        break;
      }
      case 'c': {
        float v = (float)atof(optarg);
        if (v <= 0.0f || v >= 1.0f)
          die("--cut-point must be in the open interval (0.0, 1.0).");
        g_cfg.cut_point_ratio = v;
        break;
      }
      case 'm': {
        float v = (float)atof(optarg);
        if (v < 0.0f || v >= 1.0f)
          die("--mutation-rate must be in the interval [0.0, 1.0).");
        g_cfg.mutation_rate = v;
        break;
      }
      case 'd': {
        if      (strcmp(optarg, "minimize") == 0) g_cfg.direction = MINIMIZE;
        else if (strcmp(optarg, "maximize") == 0) g_cfg.direction = MAXIMIZE;
        else    die("--direction must be 'minimize' or 'maximize'.");
        break;
      }
      case 'f': {
        size_t n = sizeof(fitness_table) / sizeof(fitness_table[0]);
        int found = 0;
        for (size_t j = 0; j < n; j++) {
          if (strcmp(optarg, fitness_table[j].name) == 0) {
            g_cfg.fitness_fn = fitness_table[j].fn;
            found = 1;
            break;
          }
        }
        if (!found) die("Unknown --fitness value. Available: quadratic.");
        break;
      }
      case 's': {
        size_t n = sizeof(selection_table) / sizeof(selection_table[0]);
        int found = 0;
        for (size_t j = 0; j < n; j++) {
          if (strcmp(optarg, selection_table[j].name) == 0) {
            g_cfg.selection_fn = selection_table[j].fn;
            found = 1;
            break;
          }
        }
        if (!found) die("Unknown --selection value. Available: roulette and tournament.");
        break;
      }
      case 'x': {
        size_t n = sizeof(crossover_table) / sizeof(crossover_table[0]);
        int found = 0;
        for (size_t j = 0; j < n; j++) {
          if (strcmp(optarg, crossover_table[j].name) == 0) {
            g_cfg.crossover_fn = crossover_table[j].fn;
            found = 1;
            break;
          }
        }
        if (!found) die("Unknown --crossover value. Available: single-point.");
        break;
      }
      case 1000: {
        g_cfg.domain_min = atof(optarg);
        break;
      }
      case 1001: {
        g_cfg.domain_max = atof(optarg);
        domain_max_explicit = 1;
        break;
      }
      case 'h':
        print_help(argv[0]);
        exit(EXIT_SUCCESS);
      default:
        fprintf(stderr, "Run with --help for usage.\n");
        exit(EXIT_FAILURE);
    }
  }

  if (!domain_max_explicit)
    // Unsigned long shift avoids signed integer overflow
    g_cfg.domain_max = (double)((1UL << g_cfg.ind_size) - 1UL);

  if (g_cfg.domain_min >= g_cfg.domain_max)
    die("--domain-min must be strictly less than --domain-max.");

  if (g_cfg.tournament_size > g_cfg.pop_size) {
    char msg[128];
    snprintf(msg, sizeof(msg), "--tournament-size must be at most pop-size (pop-size=%d)",
             g_cfg.pop_size);
    die(msg);
  }
}
