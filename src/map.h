#ifndef MAP_H
#define MAP_H

#include <stdint.h>

#include "slice.h"

#define _MAP_INITIAL_BUCKETS 16

// typedef struct {

// } Pair;

typedef struct {
    uint32_t seed;
    Slice *bucket; /* Slice<T> */
} Map;

uint64_t _map_murmur3_64(const void* key, int len, uint32_t seed);

#endif