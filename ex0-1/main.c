/*
 * main.c -- a simple standalone C program
 */


#define CTRL	((unsigned int *) 0xF0300008)	/* serial xmt ctrl reg */
#define DATA	((unsigned int *) 0xF030000C)	/* serial xmt data reg */


void putc(int c) {
  volatile unsigned int *ctrl = CTRL;
  volatile unsigned int *data = DATA;

  while ((*ctrl & 1) == 0) ;
  *data = c;
}


int main(void) {
  putc('h');
  putc('e');
  putc('l');
  putc('l');
  putc('o');
  return 0;
}
