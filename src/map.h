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

typedef struct {
    Allocator allocator;
    size_t reserve;
    uint64_t (*hash)(const void *key, size_t len);
} MapOptions;

#define Map(K, V) struct Map_##K##_##V { \
    MapHeader hdr;                       \
    struct { K key; V val; } *entries;   \
    uint8_t *occupied;                   \
}

#define _map_entries(m)   (*(void**)((char*)(m) + sizeof(MapHeader)))
#define _map_occupied(m)  (*(uint8_t**)((char*)(m) + sizeof(MapHeader) + sizeof(void*)))

uint64_t _map_murmur3_64(const void *key, size_t len);

void  _map_init(void *hdr, size_t key_size, size_t val_size, size_t val_offset, size_t entry_stride, MapOptions params);
void  _map_deinit(void *hdr);
bool  _map_set(void *hdr, const void *key, void *val);
void *_map_get(void *hdr, const void *key);
bool  _map_has(void *hdr, const void *key);
void  _map_remove(void *hdr, const void *key);
size_t _map_len(const void *hdr);

#define map_init(m, ...) \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Winitializer-overrides\"") \
    _map_init((void*)(m), \
        sizeof((m)->entries->key), \
        sizeof((m)->entries->val), \
        offsetof(typeof(*(m)->entries), val), \
        sizeof(typeof(*(m)->entries)), \
        (MapOptions){ .allocator = std_allocator, .reserve = 0, .hash = _map_murmur3_64, __VA_ARGS__ }) \
    _Pragma("clang diagnostic pop")
#define map_deinit(m)       _map_deinit((void*)(m))
#define map_set(m, k, v) do { \
    typeof((m)->entries->key) _mk_ = (k); \
    typeof((m)->entries->val) _mv_ = (v); \
    _map_set((void*)(m), &_mk_, &_mv_); \
} while(0)
#define map_get(m, k) \
    ((typeof(&(m)->entries->val))_map_get((void*)(m), &(typeof((m)->entries->key)){k}))
#define map_has(m, k) \
    _map_has((void*)(m), &(typeof((m)->entries->key)){k})
#define map_remove(m, k) \
    _map_remove((void*)(m), &(typeof((m)->entries->key)){k})
#define map_len(m)          _map_len((const void*)(m))

#endif
