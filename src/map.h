#ifndef MAP_H
#define MAP_H

#include "allocator.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define _MAP_INIT_CAP 16
#define _MAP_MAX_LOAD 0.75

typedef struct {
    Allocator alloc;
    size_t key_size;
    size_t val_size;
    size_t val_offset;
    size_t entry_stride;
    size_t cap;
    size_t len;
    uint64_t (*hash)(const void *key, size_t len);
} MapHeader;

#define Map(K, V) struct Map_##K##_##V { \
    MapHeader hdr;                       \
    K *_key;                             \
    V *_val;                             \
    void *entries;                       \
    uint8_t *occupied;                   \
}

#define _map_hdr(m)       ((MapHeader*)(m))
#define _map_entries(m)   (*(void**)((char*)(m) + sizeof(MapHeader) + 2 * sizeof(void*)))
#define _map_occupied(m)  (*(uint8_t**)((char*)(m) + sizeof(MapHeader) + 3 * sizeof(void*)))

uint64_t _map_murmur3_64(const void *key, size_t len);

void  _map_init(void *hdr, size_t key_size, size_t val_size, size_t val_offset, size_t entry_stride, Allocator alloc, size_t reserve);
void  _map_deinit(void *hdr);
bool  _map_set(void *hdr, const void *key, void *val);
void *_map_get(void *hdr, const void *key);
bool  _map_has(void *hdr, const void *key);
void  _map_remove(void *hdr, const void *key);
size_t _map_len(const void *hdr);

#define map_init(m, allocator, reserve) \
    _map_init((void*)(m), \
        sizeof(typeof(*(m)->_key)), \
        sizeof(typeof(*(m)->_val)), \
        offsetof(struct { typeof(*(m)->_key) __k; typeof(*(m)->_val) __v; }, __v), \
        sizeof(struct { typeof(*(m)->_key) __k; typeof(*(m)->_val) __v; }), \
        (allocator), (reserve))
#define map_deinit(m)       _map_deinit((void*)(m))
#define map_set(m, k, v) do { \
    typeof(*(m)->_key) _mk_ = (k); \
    typeof(*(m)->_val) _mv_ = (v); \
    _map_set((void*)(m), &_mk_, &_mv_); \
} while(0)
#define map_get(m, k) \
    ((typeof((m)->_val))_map_get((void*)(m), &(typeof(*(m)->_key)){k}))
#define map_has(m, k) \
    _map_has((void*)(m), &(typeof(*(m)->_key)){k})
#define map_remove(m, k) \
    _map_remove((void*)(m), &(typeof(*(m)->_key)){k})
#define map_len(m)          _map_len((const void*)(m))

#endif
