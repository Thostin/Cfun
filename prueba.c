#include "btree/btree.c"
#include <stdio.h>
#include <stdlib.h>

int menu(void);
int main(void) { return menu(); }

int menu(void) {
  printf("**MENU DE PRUEBA**\n");
  printf("Opciones:\n");
  printf("1. Create a list\n");
  printf("2. Create a binary tree\n");
  printf("3. Rehash binary tree\n");
  printf("4. Insert an element to a binary tree\n");
  printf("5. Insert an element to a list\n");

  int op;

  // List of lists
  listptr_t listslist = nullptr;
  while (1) {
    printf("> ");
    if (1 != scanf(" %d", &op)) {
      printf("Improper input");
      return EXIT_FAILURE;
    }

    switch (op) {}
  }

  return EXIT_SUCCESS;
}
