#pragma once

#include "ga.h"

void clear_screen(void);
void wait_for_enter(void);
void print_ind(int ind[IND]);
void print_pop(int pop[POP][IND]);
void print_pares(int pop[POP][IND]);
void init(int matriz[POP][IND]);
void copy_ind(int i1[], int i2[]);
