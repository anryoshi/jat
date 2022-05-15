#ifndef JAT_PLATFORM_H
#define JAT_PLATFORM_H

#include <stdlib.h>

static inline void *jat_alloc(size_t size) {
    return malloc(size);
}

static inline void jat_free(void *ptr) {
    free(ptr);
}

#endif // JAT_PLATFORM_H
