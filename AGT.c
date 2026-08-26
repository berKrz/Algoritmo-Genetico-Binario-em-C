#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define POP 4
#define IND 5

void clear_screen(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
}

int f(int ind[IND]) {
   int n = 0;
 
   for (int i = 0; i < IND; i++) {
     n += (ind[i] * (1 << i)) ;
   }

  return n * n;
 }

void print_ind(int ind[IND]) {
  for (int i = IND-1; i > -1; i--) {
    printf("[ %d ] ", ind[i]);
  }
}

void print_pop(int pop[POP][IND]) {
  printf("\nPopulação Atual\n");

  for (int i = 0; i < POP; i++) {
    printf("\nIndivíduo %d = ", i);
    print_ind(pop[i]);
    printf("f(%d) = %d", i, f(pop[i]));
    printf("\n");
  }
  printf("\n");
}

void init(int matriz[POP][IND]) {
  for (int i = 0; i < POP; i++) {
    for (int j = 0; j < IND; j++) {
      matriz[i][j] = rand() % 2;
    }
  }
}

int main(int argc, char *argv[])
{
  srand(time(NULL));
  clear_screen();

  int pop[POP][IND];
  init(pop);

  print_pop(pop);

  return EXIT_SUCCESS;
}
