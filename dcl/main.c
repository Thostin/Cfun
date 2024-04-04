#include "func.c"
#include <stdio.h>

#define WTF 2147000000

int main(int argc, char *argv[]) {
  if (argc == 1) {
    insert_status();
  } else if (argc < 1) {
    printf("Yo wtf\n");
    return WTF;
  } else {
    for (int i = 1; i < argc; ++i)
      write_status(argv[i]), putchar(10);
  }

  return EXIT_SUCCESS;
}
