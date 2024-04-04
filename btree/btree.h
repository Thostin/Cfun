/*
 * Written by Thostin
 * Github: https://github.com/Thostin
 */

#include "btree.c"

// I managed to not use recursion
// Create btree
err_t initbtree(btreeptr_t *const root, const void *data, const size_t size);

// Insert an element with a provided compare function
err_t finsbtree(btreeptr_t *const root, const void *data, const size_t size,
                long (*cmp)(const void *mem1, const void *mem2, size_t size));

// Insert an element using __builtin_memcmp for comparision
err_t insbtree(btreeptr_t *const root, const void *data, const size_t size);

// Find element using __builtin_memcmp for comparision
err_t findbtree(btreeptr_t root, void *data, size_t size, btreeptr_t *ret);

// Find element using a provided function for comparission
err_t ffindbtree(btreeptr_t root, void *data, size_t size, btreeptr_t *ret,
                 long (*cmp)(const void *, const void *, size_t size));
/**/
