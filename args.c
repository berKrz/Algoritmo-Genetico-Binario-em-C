#include "args.h"
#include "ga.h"
#include "config.h"
#include "ini.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

static void print_help(const char *prog) {
  printf("Usage: %s [OPTIONS]\n\n", prog);
  printf("Options:\n");
  printf("  -F, --config            FILE   Path to INI config file\n");
  printf("  -p, --pop-size          INT    Population size (min: 2)            [default: 15]\n");
  printf("  -i, --ind-size          INT    Individual size (min: 2)            [default: 22]\n");
  printf("  -g, --generations       INT    Number of generations (min: 1)      [default: 30]\n");
  printf("  -k, --tournament-size   INT    Number of individuals (min: 2)      [default: 2]\n");
  printf("  -c, --cut-point         FLOAT  Crossover cut point ratio (0.0,1.0) [default: 0.6]\n");
  printf("  -r, --mutation-rate     FLOAT  Mutation ratio [0.0,1.0)            [default: 0.01]\n");
  printf("  -d, --direction         STR    minimize | maximize                 [default: minimize]\n");
  printf("  -f, --fitness           STR    quadratic                           [default: quadratic]\n");
  printf("  -s, --selection         STR    roulette | tournament               [default: roulette]\n");
  printf("  -x, --crossover         STR    single-point                        [default: single-point]\n");
  printf("  -m  --domain-min        FLOAT  Domain minimum value                [default: 0.0]\n");
  printf("  -M  --domain-max        FLOAT  Domain maximum value                [default: 2^ind-size - 1]\n");
  printf("  -h, --help                     Print this message and exit\n");
}

void parse_args(int argc, char **argv) {
  static const struct option long_opts[] = {
    { "config",          required_argument, NULL, 'F'  },
    { "pop-size",        required_argument, NULL, 'p'  },
    { "ind-size",        required_argument, NULL, 'i'  },
    { "generations",     required_argument, NULL, 'g'  },
    { "tournament-size", required_argument, NULL, 'k'  },
    { "cut-point",       required_argument, NULL, 'c'  },
    { "mutation-rate",   required_argument, NULL, 'r'  },
    { "direction",       required_argument, NULL, 'd'  },
    { "fitness",         required_argument, NULL, 'f'  },
    { "selection",       required_argument, NULL, 's'  },
    { "crossover",       required_argument, NULL, 'x'  },
    { "domain-min",      required_argument, NULL, 'm'  },
    { "domain-max",      required_argument, NULL, 'M'  },
    { "help",            no_argument,       NULL, 'h'  },
    { NULL,              0,                 NULL,  0   }
  };

  Config aux_cfg = {0};

  struct {
    int pop_size, ind_size, generations, tournament_size;
    int cut_point, mutation_rate, direction;
    int fitness, selection, crossover;
    int domain_min, domain_max;
  } set = {0};

  const char *config_path = NULL;

  int opt;
  while ((opt = getopt_long(argc, argv, "F:p:i:g:k:c:r:d:f:s:x:m:M:h", long_opts, NULL)) != -1) {
    switch (opt) {
      case 'F': config_path = optarg;                                                    break;
      case 'p': apply_field("pop_size",        optarg, &aux_cfg, "CLI"); set.pop_size        = 1; break;
      case 'i': apply_field("ind_size",        optarg, &aux_cfg, "CLI"); set.ind_size        = 1; break;
      case 'g': apply_field("generations",     optarg, &aux_cfg, "CLI"); set.generations     = 1; break;
      case 'k': apply_field("tournament_size", optarg, &aux_cfg, "CLI"); set.tournament_size = 1; break;
      case 'c': apply_field("cut_point",       optarg, &aux_cfg, "CLI"); set.cut_point       = 1; break;
      case 'r': apply_field("mutation_rate",   optarg, &aux_cfg, "CLI"); set.mutation_rate   = 1; break;
      case 'd': apply_field("direction",       optarg, &aux_cfg, "CLI"); set.direction       = 1; break;
      case 'f': apply_field("fitness",         optarg, &aux_cfg, "CLI"); set.fitness         = 1; break;
      case 's': apply_field("selection",       optarg, &aux_cfg, "CLI"); set.selection       = 1; break;
      case 'x': apply_field("crossover",       optarg, &aux_cfg, "CLI"); set.crossover       = 1; break;
      case 'm': apply_field("domain_min",      optarg, &aux_cfg, "CLI"); set.domain_min      = 1; break;
      case 'M': apply_field("domain_max",      optarg, &aux_cfg, "CLI"); set.domain_max      = 1; break;
      case 'h':
        print_help(argv[0]);
        exit(EXIT_SUCCESS);
      default:
        fprintf(stderr, "Run with --help for usage.\n");
        exit(EXIT_FAILURE);
    }
  }

  // Parse config file into g_cfg before applying CLI overrides
  if (config_path) parse_ini(config_path, &g_cfg);

  // Apply CLI values over any file-provided values
  if (set.pop_size)        g_cfg.pop_size        = aux_cfg.pop_size;
  if (set.ind_size)        g_cfg.ind_size        = aux_cfg.ind_size;
  if (set.generations)     g_cfg.generations     = aux_cfg.generations;
  if (set.tournament_size) g_cfg.tournament_size = aux_cfg.tournament_size;
  if (set.cut_point)       g_cfg.cut_point_ratio = aux_cfg.cut_point_ratio;
  if (set.mutation_rate)   g_cfg.mutation_rate   = aux_cfg.mutation_rate;
  if (set.direction)       g_cfg.direction       = aux_cfg.direction;
  if (set.fitness)         g_cfg.fitness_fn      = aux_cfg.fitness_fn;
  if (set.selection)       g_cfg.selection_fn    = aux_cfg.selection_fn;
  if (set.crossover)       g_cfg.crossover_fn    = aux_cfg.crossover_fn;
  if (set.domain_min)      g_cfg.domain_min      = aux_cfg.domain_min;
  if (set.domain_max)      g_cfg.domain_max      = aux_cfg.domain_max;

  // Resolve domain_max default if not set by any source
  if (!config_domain_max_was_set())
    // Unsigned long shift avoids signed integer overflow
    g_cfg.domain_max = (double)((1UL << g_cfg.ind_size) - 1UL);

  // Cross-field validation
  if (g_cfg.domain_min >= g_cfg.domain_max)
    die("--domain-min must be strictly less than --domain-max.");

  if (g_cfg.tournament_size > g_cfg.pop_size) {
    char msg[128];
    snprintf(msg, sizeof(msg),
             "--tournament-size must be at most pop-size (pop-size=%d)",
             g_cfg.pop_size);
    die(msg);
  }
}
