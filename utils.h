#pragma once

#include "ga.h"

void die    (const char *msg);
void die_at (const char *source, const char *key, const char *msg);

void clear_screen(void);
void wait_for_enter(void);
void print_ind  (int *ind);
void print_pop  (int *pop);
void print_pares(int *pop);
void init       (int *pop);
void copy_ind   (int *i1, int *i2);
