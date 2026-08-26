#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void clear_screen(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
}

void wait_for_enter(void) {
  printf("\n\nAperte [Enter] para continuar...\n");
  while (getchar() != '\n');
}

void print_ind(int ind[IND]) {
  for (int i = IND-1; i > -1; i--) {
    printf("[ %d ] ", ind[i]);
  }
}

void print_pop(int pop[POP][IND]) {
  for (int i = 0; i < POP; i++) {
    printf("\nIndivíduo %d = ", i);
    print_ind(pop[i]);
    printf("fitness(%d) = %d", i, fitness(pop[i]));
    printf("\n");
  }
  printf("\n");
}

void print_pares(int pop[POP][IND]) {
  printf("Pares Selecionados");

  for (int i = 0; i < POP; i++) {
    printf("\n\nPar %d", i/2);
    printf("\nIndivíduo %d = ", i);
    print_ind(pop[i]);
    printf("fitness(%d) = %d", i, fitness(pop[i]));

    printf("\nIndivíduo %d = ", ++i);
    print_ind(pop[i]);
    printf("fitness(%d) = %d", i, fitness(pop[i]));
  }
}

void init(int matriz[POP][IND]) {
  for (int i = 0; i < POP; i++) {
    for (int j = 0; j < IND; j++) {
      matriz[i][j] = rand() % 2;
    }
  }

  printf("\nPopulação Inicial\n");
  print_pop(matriz);
}

void copy_ind(int i1[], int i2[]) {
  for (int i = 0; i < IND; i++) {
    i1[i] = i2[i];
  }
}
