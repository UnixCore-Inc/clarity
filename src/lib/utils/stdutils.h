#ifndef STDUTILS_H
#define STDUTILS_H

#include <string.h>
#include <stdlib.h>

#define alloc_p(type, n) (type *) malloc(sizeof(type) * n)
typedef const char* string_t;

#endif
