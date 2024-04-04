#include "list.c"
#include <stdlib.h>
#include <string.h>

#define LIST_H

// Basic functions
err_t initl(listptr_t *const root, const void *init_data, const size_t size);
err_t pushl(listptr_t const where, const void *data, const size_t size);
err_t freel(listptr_t *node);

// Search functions
err_t findl(listptr_t root, const void *target, const size_t size,
            listptr_t *const ret);
err_t ffindl(listptr_t node, const void *target, listptr_t *const ret,
             int (*cmp)(const void *, const void *));

// Array interaction
err_t compactl(listptr_t node, void *dst, const size_t size);
err_t initarrl(listptr_t *const root, listptr_t *const final_node,
               void *init_data, size_t nmemb, const size_t size);
err_t nrpushl(listptr_t *const node, const void *data, const size_t size);
