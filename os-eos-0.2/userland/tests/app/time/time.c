/*
 * prog.c -- ein kleines Testprogramm
 */


#include <time.h>
#include <stdio.h>


int main(void) {
  time_t t;

  t = time(NULL);
  printf("Sekunden seit 1970: %ld\n", t);
  return 0;
}
