#include "../defs/defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list_t list_t;
typedef struct list_t *listptr_t;

struct list_t {
  listptr_t next;
  void *data;
};

// Toma como parámetro una dirección de memoria de donde se
// guardará el puntero al inicio de la lista
err_t initl(listptr_t *const root, const void *init_data, const size_t size) {
  if (nullptr == root)
    return EXIT_FAILURE;
  *root = (listptr_t)malloc(sizeof(list_t));
  if (nullptr == *root)
    goto err0;

  (*root)->data = malloc(size);
  if (nullptr == (*root)->data)
    goto err1;

  memcpy((*root)->data, init_data, size);

  return EXIT_SUCCESS;

err1:
  free(*root);
err0:
  return EXIT_FAILURE;
}

// El valor de retorno es para ver si hubo errores
// Aloja un puntero con el dato que se le especifica
// Overwrites, if written before, the *next pointer
// No revisa si el programador se equivoca al pasar los punteros
err_t pushl(listptr_t const where, const void *data, const size_t size) {

  if (nullptr == where || nullptr == data || 0 == size) {
    return EXIT_FAILURE;
  }

  listptr_t aux = (listptr_t)malloc(sizeof(list_t));

  if (nullptr == aux)
    goto err0;

  aux->data = malloc(size);
  if (nullptr == aux->data)
    goto err1;

  memcpy(aux->data, data, size);

  where->next = aux;
  return EXIT_SUCCESS;

err1:
  free(aux);
err0:
  return EXIT_FAILURE;
}

// Chequeará la lista desde *node, si el elemento ya estaba, no genera un nuevo
// nodo no_repeat_push_list(...)
err_t nrpushl(listptr_t *const _node, const void *data, const size_t size) {
  if (nullptr == _node || nullptr == data || 0 == size)
    return EXIT_FAILURE_IMPROPER_USE;

  if (nullptr == *_node)
    initl(_node, data, size);

  listptr_t node = *_node;
  while (1) {
    if (nullptr == node->data)
      return EXIT_FAILURE_IMPROPER_USE;
    if (!__builtin_memcmp(data, node->data, size))
      return EXIT_SUCCESS_REPEATED;
    if (nullptr == node->next) {
      pushl(node, data, size);
      return EXIT_SUCCESS;
    }
    node = node->next;
  }
}

// Liberar una lista desde *node
// No hay manera de que falle, salvo que se le pase un
// nullptr como argumento.
// Repito: NO hay manera de que falle salvo esa.
// Toma como argumento un puntero a otro puntero para borrar el puntero original
err_t freel(listptr_t *node) {
  if (nullptr == node)
    return EXIT_FAILURE;

  listptr_t aux1 = *node;
  *node = nullptr; // Chau puntero

  listptr_t aux2;
  while (aux1) {
    aux2 = aux1->next;
    // C standard: "If ptr is a null pointer, no action occurs."
    free(aux1->data);
    free(aux1);
    aux1 = aux2;
  }

  return EXIT_SUCCESS;
}

// Searches for *ret in *data member from node
// Returns EXIT_SUCCESS if found element and EXIT_FAILURE if not,
// in case of EXIT_FAILURE, leaves *ret unchanged.
// Stores pointer where comparission whith target success in ret
// Also returns EXIT_FAILURE if *node or its *data are nullptr
// Note: Checks both *data and *next pointers for a nullptr, just to be safe
// Will not segfault unless a non-nullptr non-malloc/calloc/realloc-allocated
// pointer occurs
err_t findl(listptr_t node, const void *target, const size_t size,
            listptr_t *const ret) {

  while (node && node->data) {
    if (!memcmp(node->data, target, size)) {
      *ret = node;
      return EXIT_SUCCESS;
    }
    node = node->next;
  }

  return EXIT_FAILURE;
}

// Same as findl, but requires a comparision function
// Will not segfault unless a non-nullptr non-malloc/calloc/realloc-allocated
// pointer occurs
// '0' return value will be threated as 'equal'
err_t ffindl(listptr_t node, const void *target, listptr_t *const ret,
             int (*cmp)(const void *, const void *)) {

  while (node && node->data) {
    if (!cmp(node->data, target)) {
      *ret = node;
      return EXIT_SUCCESS;
    }
    node = node->next;
  }

  return EXIT_FAILURE;
}

// Reads the list from *node and stores *data values
// to an unknown fixed size array
// dst should be allocated before calling compactl
err_t compactl(listptr_t node, void *dst, const size_t size) {
  while (node && node->data) {
    memcpy(dst, node->data, size);
    node = node->next;
    dst = (char *)dst + size;
  }
  return EXIT_SUCCESS;
}

// Initialize a list from an array
// requires mem to root and final nodes respectuvely
// I'm not sure if I have to check pushl return values, maybe yes
// If final node is nullptr, it will not give error, just ignore it
err_t initarrl(listptr_t *const root, listptr_t *const final_node,
               void *init_data, size_t nmemb, const size_t size) {

  if (initl(root, init_data, size))
    return EXIT_FAILURE;

  listptr_t node = *root;
  while (--nmemb) {
    if (pushl(node, (init_data = (char *)init_data + size), size)) {
      freel(root);
      return EXIT_FAILURE;
    }
    node = node->next;
  }

  if (final_node)
    *final_node = node;

  return EXIT_SUCCESS;
}
