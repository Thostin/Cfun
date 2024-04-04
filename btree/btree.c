#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LIST_H
#include "../list/list.h"
#endif

#ifndef DEFS_H
#include "../defs/defs.h"
#endif

typedef struct btree btree_t;
typedef struct btree *btreeptr_t;

struct btree {
  btreeptr_t left, right;
  void *data;
};

// En caso de que se haya modificado indebidamente el árbol.

/*
 * Se espera que todos los nodos posean punteros a datos no nulos,
 * caso contrario implica una manipulación externa indebida del árbol.
 *
 * Regla de guardado:
 * Menor (negativo) indica izquierda
 * Mayor (positivo) indica derecha
 * Igual (cero) indica no guardar
 *
 * En ninguno de los casos anteriores se espera un error
 *
 * NOTA: si se desea guardar datos incluso si se repiten, se debe
 * llamar a la funcion finsbtree(...), ya que así se puede especificar
 * una funcion de comparación que retorne, por ejemplo, positivo en caso
 * de igualdad.
 *
 *
 * Casos de error:
 * ---El ya mecionado *data nulo (nullptr).
 * ---Un fallo de void* malloc(size_t size); (sin RAM o ?, muy raro)
 * ---El programa es cerrado abruptamente (no se hace)
 * ---Bug de compilador (muy raro)
 * ---El compilador escoge archivos que han sido modificados previamente
 * (extremadamente raro)
 */

err_t initbtree(btreeptr_t *const root, const void *data, const size_t size) {
  *root = (btreeptr_t)malloc(sizeof(btree_t));
  if (nullptr == *root)
    return EXIT_FAILURE;

  (*root)->data = malloc(size);
  if (nullptr == (*root)->data) {
    free(*root);
    EXIT_FAILURE;
  }

  memcpy((*root)->data, data, size);

  // Este paso es muy importante, ya que previene
  // un posible seg-fault cada vez que se busque o
  // inserte en el árbol.
  (*root)->left = nullptr;
  (*root)->right = nullptr;

  return EXIT_SUCCESS;
}

// Could be recursive, but nah
err_t finsbtree(btreeptr_t *const root, const void *data, const size_t size,
                long (*cmp)(const void *mem1, const void *mem2, size_t size)) {

  if (nullptr == root || nullptr == data)
    return EXIT_FAILURE_IMPROPER_USE;

  if (nullptr == *root) {
    initbtree(root, data, size);
    return EXIT_SUCCESS;
  }

  long _compare_;
  btreeptr_t node = *root;
  btreeptr_t *aux;

  while (1) {
    if (nullptr == node->data)
      return EXIT_FAILURE_IMPROPER_USE;

    _compare_ = cmp(data, node->data, size);
    if (0 == _compare_) {
      // printf("EQUAL\n");
      return EXIT_SUCCESS;
    }
    if (_compare_ > 0) {
      // printf("RIGHT\n");
      if (nullptr == node->right) {
        aux = &(node->right);
        return initbtree(aux, data, size);
      }
      node = node->right;
    } else {
      // printf("LEFT\n");
      if (nullptr == node->left) {
        // Technically, this specific assigment is not necessary:
        aux = &(node->left);

        return initbtree(aux, data, size);
      }
      node = node->left;
    }
  }
}

// Uses memcmp for making comparisions
err_t insbtree(btreeptr_t *const root, const void *data, const size_t size) {

  if (nullptr == root || nullptr == data)
    return EXIT_FAILURE_IMPROPER_USE;

  if (nullptr == *root) {
    initbtree(root, data, size);
    return EXIT_SUCCESS;
  }

  int _compare_;
  btreeptr_t node = *root;
  btreeptr_t *aux;

  while (1) {
    if (nullptr == node->data)
      return EXIT_FAILURE_IMPROPER_USE;

    _compare_ = __builtin_memcmp(data, node->data, size);
    if (0 == _compare_) {
      // printf("EQUAL\n");
      return EXIT_SUCCESS_REPEATED;
    }
    if (_compare_ > 0) {
      // printf("RIGHT\n");
      if (nullptr == node->right) {
        aux = &(node->right);
        return initbtree(aux, data, size);
      }
      node = node->right;
    } else {
      // printf("LEFT\n");
      if (nullptr == node->left) {
        // Technically, this specific assigment is not necessary:
        aux = &(node->left);

        return initbtree(aux, data, size);
      }
      node = node->left;
    }
  }
}

// Assumes non-null root, not recommended using directly
//
void __freebtree(btreeptr_t *root) {
  if ((*root)->left)
    __freebtree(&((*root)->left));
  if ((*root)->left)
    __freebtree(&((*root)->right));

  free((*root)->data);
  free(*root);
  *root = nullptr;
}

// Following same way as C standard: If ptr is nullptr, no action is made
void freebtree(btreeptr_t *root) {
  if (root)
    __freebtree(root);
}

// uses memcmp
// Stores node address to *ret
err_t findbtree(btreeptr_t root, void *data, size_t size, btreeptr_t *ret) {

  if (nullptr == root || nullptr == data)
    return EXIT_FAILURE_IMPROPER_USE;

  int _compare_;
  btreeptr_t node = root;

  while (1) {
    if (nullptr == node->data)
      return EXIT_FAILURE_IMPROPER_USE;

    _compare_ = __builtin_memcmp(data, node->data, size);
    if (0 == _compare_) {
      printf("EQUAL\n");
      *ret = node;
      return EXIT_SUCCESS;
    }

    if (_compare_ > 0) {
      printf("RIGHT\n");
      if (nullptr == node->right)
        return EXIT_FAILURE_NOT_FOUND;
      node = node->right;
    } else {
      printf("LEFT\n");
      if (nullptr == node->left)
        return EXIT_FAILURE_NOT_FOUND;

      node = node->left;
    }
  }
}

err_t ffindbtree(btreeptr_t root, void *data, size_t size, btreeptr_t *ret,
                 long (*cmp)(const void *, const void *, size_t size)) {

  if (nullptr == root || nullptr == data)
    return EXIT_FAILURE_IMPROPER_USE;

  int _compare_;
  btreeptr_t node = root;

  while (1) {
    if (nullptr == node->data)
      return EXIT_FAILURE_IMPROPER_USE;

    _compare_ = cmp(data, node->data, size);
    if (0 == _compare_) {
      printf("EQUAL\n");
      *ret = node;
      return EXIT_SUCCESS;
    }

    if (_compare_ > 0) {
      printf("RIGHT\n");
      if (nullptr == node->right)
        return EXIT_FAILURE_NOT_FOUND;
      node = node->right;

    } else {
      printf("LEFT\n");
      if (nullptr == node->left)
        return EXIT_FAILURE_NOT_FOUND;

      node = node->left;
    }
  }
}

static listptr_t __arrbtree_root = nullptr;
static size_t __arrbtree_len = 0;

static err_t __arrbtree(btreeptr_t node) {
  if (nullptr == node)
    return EXIT_SUCCESS;

  __arrbtree(node->left);

  if (nullptr != node->data) {
    ++__arrbtree_len;
  } else {
    freel(&__arrbtree_root);
    return EXIT_FAILURE;
  }

  __arrbtree(node->right);

  return EXIT_SUCCESS;
}

err_t arrbtree(void **dst, btreeptr_t *node) { return EXIT_SUCCESS; }

/*
typedef long type;

long comp(const void *a, const void *b, size_t size) {
  return *(long *)a - *(long *)b;
}

// Para probar si las funciones funcionan xd
int main(void) {
  printf("Well, compiles\n");

  btreeptr_t root;

  printf("Introduzca entreros:\n");
  type aux;

  printf("> ");
  if (!scanf("%ld", &aux))
    return 1;

  if (initbtree(&root, &aux, sizeof(type)))
    printf("MMMMMMM\n");
  while (1) {
    printf("> ");
    if (!scanf("%ld", &aux))
      break;
    if (finsbtree(&root, &aux, sizeof(type), comp))
      printf("MMMMMMMM\n");
  }

  btreeptr_t ret;
  while (getchar() != '\n')
    ;

  while (1) {
    printf("> ");
    if (!scanf("%ld", &aux))
      break;
    if (ffindbtree(root, &aux, sizeof(type), &ret, comp))
      printf("MMMMMMMM\n");
    else
      printf("FOUND\n");
  }
  return 0;
}

*/
