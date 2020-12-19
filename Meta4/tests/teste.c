//#include <stdio.h>

int main(void) {
  int i = 0;
  int j;
  while (i < 32) {
    j = 0;
    while (j < 32) {
      if (i & j)
        putchar(' ');
      else
        putchar('+');
      j = j + 1;
    }
    i = i + 1;
    putchar('\n');
  }
  return 0;
}