/******/
#define DEFS_H
/******/

#ifndef nullptr
#define nullptr 0x0
#endif /* ifndef nullptr */

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif // !EXIT_SUCCESS

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

enum {
  EXIT_FAILURE_IMPROPER_USE = 2,
  EXIT_FAILURE_NOT_FOUND = 3,
  EXIT_SUCCESS_REPEATED = 4
};

typedef int err_t;
