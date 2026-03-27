#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

typedef struct allocator {
    void* (*malloc)(size_t size);
    void (*free)(void *ptr);
    void* (*realloc)(void *ptr, size_t new_size);
    void* (*calloc)(size_t nmemb, size_t size);
} Allocator;

extern const Allocator std_allocator;

#endif