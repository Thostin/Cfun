#include "../list/list.h"
#include <stdlib.h>
/*
 * N-Tree data type to manage variable amount of members per floor
 * Requires list.h and list.c to work
 */

typedef struct ntree ntree_t;
typedef struct ntree *ntreeptr_t;

struct ntree {
  list_t below_nodes;
  void *data;
};

err_t initntree(ntreeptr_t *const root, void *init_data, size_t nmemb,
                size_t size) {

  return EXIT_SUCCESS;
}
