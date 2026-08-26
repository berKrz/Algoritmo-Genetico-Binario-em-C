#include <stdlib.h>
#include <time.h>
#include "ga.h"
#include "utils.h"

int main(int argc, char *argv[])
{
  srand(time(NULL));
  clear_screen();

  int pop[POP][IND];
  init(pop);

  print_pop(pop);

  return EXIT_SUCCESS;
}
