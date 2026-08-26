#pragma once

#define POP 4
#define IND 5

int fitness(int ind[IND]);
void roleta(int pop[][IND]);
void crossover(int i1[], int i2[]);
void crossover_pop(int pop[][IND]);
