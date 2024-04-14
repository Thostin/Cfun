#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LIST_H
#include "../list/list.h"
#endif

#ifndef DEFS_H
#include "../defs/defs.h"
#endif

#define next(node) node = node->next

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
static void __freebtree(btreeptr_t *root) {
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
      // printf("EQUAL\n");
      *ret = node;
      return EXIT_SUCCESS;
    }

    if (_compare_ > 0) {
      // printf("RIGHT\n");
      if (nullptr == node->right)
        return EXIT_FAILURE_NOT_FOUND;
      node = node->right;
    } else {
      // printf("LEFT\n");
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
      // printf("EQUAL\n");
      *ret = node;
      return EXIT_SUCCESS;
    }

    if (_compare_ > 0) {
      // printf("RIGHT\n");
      if (nullptr == node->right)
        return EXIT_FAILURE_NOT_FOUND;
      node = node->right;

    } else {
      // printf("LEFT\n");
      if (nullptr == node->left)
        return EXIT_FAILURE_NOT_FOUND;

      node = node->left;
    }
  }
}

static listptr_t __arrbtree_node = nullptr;
static size_t __arrbtree_len = 0;

static inline err_t __arrbtree(btreeptr_t node) {
  if (nullptr == node) {
    // printf("nullptr node\n");
    return EXIT_SUCCESS;
  }
  // printf("HERE\n");
  if (EXIT_SUCCESS != __arrbtree(node->left))
    return EXIT_FAILURE;

  if (nullptr != node->data) {
    ++__arrbtree_len;
    pushl(__arrbtree_node, &(node->data), 8);
    next(__arrbtree_node);
  } else {
    // printf("NULL_POINTER\n");
    return EXIT_FAILURE;
  }

  if (EXIT_SUCCESS != __arrbtree(node->right))
    return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

// Transforma dst en un arreglo de punteros a los datos del árbol binario.
// Es fácil acceder a los punteros que apuntan a las ramaas inferiores del
// árbol, simplemete hay que mirar cómo está declarada la estructura
err_t arrbtree(void **dst, btreeptr_t node, size_t *len) {
  // printf("__ARRBTREE_LEN BEFOREHAND: %ld\n", __arrbtree_len);
  list_t root; // Esta variable es para hackear las funciones que hice, estoy
               // desperdiciando 16 bytes de memoria

  err_t ret = EXIT_SUCCESS;

  __arrbtree_node = &root;
  root.data = (void *)0x1; // Make sure data pointer is not nullptr

  if (EXIT_FAILURE == __arrbtree(node)) {
    freel(&(root.next));
    goto err;
  }

  *dst = malloc(8 * __arrbtree_len);

  if (nullptr == *dst)
    goto err;

  if (EXIT_SUCCESS != compactl(root.next, *dst, 8)) {
    free(*dst);
    *dst = nullptr;
    goto err;
  }
  *len = __arrbtree_len;

  /*
  printf("SEE IF IT WORKS:\n");
  printf("__arrbtree_len = %ld\n", __arrbtree_len);

  type **arr = (type **)*dst;

  for (int i = 0; i < __arrbtree_len; ++i) {
    printf("%ld ", *(arr[i]));
  }
  putchar(10);
  */
  // Este paso es muy importante
  goto success;
err:
  ret = EXIT_FAILURE;
success:
  freel(&(root.next));
  __arrbtree_len = 0;
  __arrbtree_node = nullptr;

  return ret;
}

static listptr_t __nodearrbtree_node;
static size_t __nodearrbtree_len = 0;

static inline err_t __nodearrbtree(btreeptr_t node) {
  if (nullptr == node) {
    // printf("nullptr node\n");
    return EXIT_SUCCESS;
  }
  // printf("HERE\n");
  if (EXIT_SUCCESS != __nodearrbtree(node->left))
    return EXIT_FAILURE;

  if (nullptr != node->data) {
    ++__nodearrbtree_len;
    pushl(__nodearrbtree_node, node, 8);
    next(__nodearrbtree_node);
  } else {
    // printf("NULL_POINTER\n");
    return EXIT_FAILURE;
  }

  return __nodearrbtree(node->right);
}

// Mismo que arrbtree, pero retorna punteros a los nodos en lugar de los datos
err_t nodearrbtree(btreeptr_t **dst, btreeptr_t node, size_t *len) {
  // printf("__ARRBTREE_LEN BEFOREHAND: %ld\n", __arrbtree_len);
  list_t root; // Esta variable es para hackear las funciones que hice, estoy
               // desperdiciando 16 bytes de memoria

  err_t ret = EXIT_SUCCESS;

  __nodearrbtree_node = &root;
  root.data = (void *)0x1; // Make sure data pointer is not nullptr

  if (EXIT_FAILURE == __nodearrbtree(node)) {
    // freel(&(root.next));
    goto err;
  }

  *dst = malloc(8 * __nodearrbtree_len);

  if (nullptr == *dst)
    goto err;

  if (EXIT_SUCCESS != compactl(root.next, *dst, 8)) {
    free(*dst);
    *dst = nullptr;
    goto err;
  }
  *len = __nodearrbtree_len;

  /*
  printf("SEE IF IT WORKS:\n");
  printf("__arrbtree_len = %ld\n", __arrbtree_len);

  type **arr = (type **)*dst;

  for (int i = 0; i < __arrbtree_len; ++i) {
    printf("%ld ", *(arr[i]));
  }
  putchar(10);
  */
  // Este paso es muy importante
  goto success;
err:
  ret = EXIT_FAILURE;
success:
  freel(&(root.next));
  __nodearrbtree_len = 0;
  __nodearrbtree_node = nullptr;

  return ret;
}

static int (*__frehashbtree_cmp)(const void *, const void *);

// Makes the binary tree faster after processing it
static inline btreeptr_t __frehashbtree(btreeptr_t *arr, size_t len) {
  switch (len) {
    // case 0 is nearly never reachen, unless the programmer calls the function
    // with it
  case 0:
    return nullptr;
  case 1:
    // Make sure that left and right pointers are null, othercase we will
    // destroy our computer
    (*arr)->right = nullptr;
    (*arr)->left = nullptr;
    return *arr;
  case 2:
    if (__frehashbtree_cmp((arr[1])->data, (arr[0])->data) > 0) {
      arr[0]->right = arr[1];
      arr[0]->left = nullptr;
    } else {
      arr[0]->left = arr[1];
      arr[0]->right = nullptr;
    }
    return arr[0];
  default:

    /*
     *  o o o
     *  o o o o
     *  o o o o o o o o
     */
    arr[len / 2]->left = __frehashbtree(arr, len / 2);
    arr[len / 2]->right = __frehashbtree(arr + len / 2 + 1, len - len / 2 - 1);
    return arr[len / 2];
  }
  return nullptr;
}

err_t frehashbtree(btreeptr_t *root, int (*comp)(const void *, const void *)) {
  btreeptr_t *arr;
  size_t len;
  __frehashbtree_cmp = comp;
  nodearrbtree(&arr, *root, &len);
  *root = __frehashbtree(arr, len);
  return EXIT_SUCCESS;
}

/*********************************************************************************/
/*
long comp(const void *a, const void *b, size_t size) {
  return *(long *)a - *(long *)b;
}
typedef long type;

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

  while (getchar() != '\n')
    ;

  void *dst;
  size_t size;
  size_t aux2;

  list_t prueba;
  prueba.data = (void *)0x1;
  listptr_t pr = &prueba;

  for (int i = 0; i < 10; ++i) {
    printf("prueba: ");
    if (!scanf(" %ld", &aux))
      return EXIT_FAILURE;
    pushl(pr, &aux, sizeof(type));
    next(pr);
  }

  dst = malloc(80);
  compactl(prueba.next, dst, sizeof(type));

  for (int i = 0; i < 10; ++i) {
    printf("%ld ", ((type *)dst)[i]);
  }
  free(dst);

  while (1) {
    printf(">  ");
    if (!scanf(" %ld", &aux))
      return 0;

    printf("op: ");
    if (!scanf(" %ld", &aux2))
      return 0;

    switch (aux2) {
    case 1:
      if (finsbtree(&root, &aux, sizeof(type), comp))
        printf("MMMMMMM\n");
      break;

    case 2:
      if (ffindbtree(root, &aux, sizeof(type), &ret, comp))
        printf("MMMMMMM\n");
      else
        printf("FOUND\n");
      break;

    case 3:
      if (EXIT_SUCCESS != arrbtree(&dst, root, &size)) {
        printf("FAILED TO COMPACT BINARY TREE\n");
      } else {
        for (int i = 0; i < size; ++i) {
          printf("%ld ", *(((type **)dst)[i]));
        }
        putchar(10);
        free(dst);
      }
      break;
    default:
      goto ex;
    }
  }

ex:
  freebtree(&root);
  return 0;
}
*/
