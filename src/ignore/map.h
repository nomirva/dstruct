#ifndef MAP_H
#define MAP_H

#include <stdint.h>

#include "slice.h"

#define _MAP_INITIAL_BUCKETS 16

#define _MAX_LOAD 0.75

// typedef struct {
//     void *key;
//     void* value;
// } Pair;

typedef struct {
    uint32_t seed;
    size_t size;
    size_t k_size;
    size_t v_size;
    Slice *store;
    
    Slice *buckets; /* Slice<Slice<T>> */
} Map;

void map_init(Map *map, size_t k_size, size_t v_size);

void map_deinit(Map *map);

void map_set(Map *map);

void map_get(Map *map, void *key);

uint64_t _map_murmur3_64(const void* key, int len, uint32_t seed);

#endif