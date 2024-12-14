#ifndef HEAP_MEMORY_MANAGER_H
#define HEAP_MEMORY_MANAGER_H

#include <stddef.h>
#include <stdlib.h>

// Custom memory allocation functions
void* my_malloc(size_t size);
void my_free(void* ptr);
void* my_realloc(void* ptr, size_t size);

#ifdef TEST_CUSTOM_MALLOC
    #define malloc my_malloc
    #define free my_free
    #define realloc my_realloc
#endif

#endif // HEAP_MEMORY_MANAGER_H
