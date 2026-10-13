/*
MALLOC.H

MSVC <malloc.h>.
*/

#ifndef __HALO_LINUX_MALLOC_H
#define __HALO_LINUX_MALLOC_H

#include <stdlib.h>

#define _alloca __builtin_alloca
#define alloca __builtin_alloca

size_t _msize(void *pointer);

#endif
