#include <string.h>
#include <time.h>

#include "map.h"

void map_init(Map *map) {
    map->seed = (uint32_t)time(NULL);
    map->bucket = NULL;
}

void map_insert(Map *map) {

}

void map_set(Map *map) {

}

void map_delete(Map *map) {

}

uint64_t map_hash(Map *map, void *data) {
    return _map_murmur3_64(data, 0/* TODO: FIX IT */, map->seed);
}

uint64_t _map_murmur3_64(void* key, int len, uint32_t seed) {
    const uint8_t* data = (const uint8_t*)key;
    const int nblocks = len / 8;
    
    uint64_t h1 = seed;
    uint64_t h2 = 0;

    const uint64_t c1 = 0x87c37b91114253d5ULL;
    const uint64_t c2 = 0x4cf5ad432745937fULL;

    const uint64_t* blocks = (const uint64_t*)(data);
    for (int i = 0; i < nblocks; i++) {
        uint64_t k1 = blocks[i];
        
        k1 *= c1;
        k1 = (k1 << 31) | (k1 >> (64 - 31));
        k1 *= c2;
        
        h1 ^= k1;
        h1 = (h1 << 27) | (h1 >> (64 - 27));
        h1 += h2;
        h1 = h1 * 5 + 0x52dce729;
    }

    const uint8_t* tail = (const uint8_t*)(data + nblocks * 8);
    uint64_t k1 = 0;
    
    switch (len & 7) {
        case 7: k1 ^= ((uint64_t)tail[6]) << 48;
        case 6: k1 ^= ((uint64_t)tail[5]) << 40;
        case 5: k1 ^= ((uint64_t)tail[4]) << 32;
        case 4: k1 ^= ((uint64_t)tail[3]) << 24;
        case 3: k1 ^= ((uint64_t)tail[2]) << 16;
        case 2: k1 ^= ((uint64_t)tail[1]) << 8;
        case 1: k1 ^= tail[0];
                k1 *= c1;
                k1 = (k1 << 31) | (k1 >> (64 - 31));
                k1 *= c2;
                h1 ^= k1;
    }

    h1 ^= len;
    h1 ^= h1 >> 33;
    h1 *= 0xff51afd7ed558ccdULL;
    h1 ^= h1 >> 33;
    h1 *= 0xc4ceb9fe1a85ec53ULL;
    h1 ^= h1 >> 33;

    return h1;
}