#include "map.h"
#include <string.h>

#define KEY(m, i)  ((char*)_map_entries(m) + (i) * (m)->entry_stride)
#define VAL(m, i)  (KEY(m, i) + (m)->val_offset)
#define OCC(m, i)  _map_occupied(m)[i]

uint64_t _map_murmur3_64(const void *key, size_t len) {
    const uint8_t *data = (const uint8_t*)key;
    size_t nblocks = len / 8;
    uint64_t h1 = 0;
    const uint64_t c1 = 0x87c37b91114253d5ULL;
    const uint64_t c2 = 0x4cf5ad432745937fULL;

    for (size_t i = 0; i < nblocks; i++) {
        uint64_t k1;
        memcpy(&k1, data + i * 8, 8);
        k1 *= c1;
        k1 = (k1 << 31) | (k1 >> 33);
        k1 *= c2;
        h1 ^= k1;
        h1 = (h1 << 27) | (h1 >> 37);
        h1 = h1 * 5 + 0x52dce729;
    }

    const uint8_t *tail = data + nblocks * 8;
    uint64_t k1 = 0;
    switch (len & 7) {
        case 7: k1 ^= (uint64_t)tail[6] << 48;
        case 6: k1 ^= (uint64_t)tail[5] << 40;
        case 5: k1 ^= (uint64_t)tail[4] << 32;
        case 4: k1 ^= (uint64_t)tail[3] << 24;
        case 3: k1 ^= (uint64_t)tail[2] << 16;
        case 2: k1 ^= (uint64_t)tail[1] << 8;
        case 1: k1 ^= tail[0];
                k1 *= c1;
                k1 = (k1 << 31) | (k1 >> 33);
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

static bool _map_resize(MapHeader *m, size_t new_cap) {
    void *old_entries = _map_entries(m);
    uint8_t *old_occ = _map_occupied(m);
    size_t old_cap = m->cap;

    void *new_entries = m->alloc.calloc(new_cap, m->entry_stride);
    uint8_t *new_occ = m->alloc.calloc(new_cap, 1);
    if (!new_entries || !new_occ) {
        if (new_entries) m->alloc.free(new_entries);
        if (new_occ) m->alloc.free(new_occ);
        return false;
    }

    _map_entries(m) = new_entries;
    _map_occupied(m) = new_occ;
    m->cap = new_cap;
    m->len = 0;

    for (size_t i = 0; i < old_cap; i++) {
        if (old_occ[i] == 1) {
            void *key = (char*)old_entries + i * m->entry_stride;
            void *val = (char*)key + m->val_offset;
            _map_set(m, key, val);
        }
    }

    m->alloc.free(old_entries);
    m->alloc.free(old_occ);
    return true;
}

void _map_init(void *hdr, size_t key_size, size_t val_size, size_t val_offset, size_t entry_stride, Allocator alloc, size_t reserve) {
    MapHeader *m = (MapHeader*)hdr;
    m->alloc = alloc;
    m->key_size = key_size;
    m->val_size = val_size;
    m->val_offset = val_offset;
    m->entry_stride = entry_stride;
    m->hash = _map_murmur3_64;
    m->len = 0;

    size_t cap = reserve > 0 ? reserve : _MAP_INIT_CAP;
    m->cap = cap;
    _map_entries(m) = alloc.calloc(cap, m->entry_stride);
    _map_occupied(m) = alloc.calloc(cap, 1);
}

void _map_deinit(void *hdr) {
    MapHeader *m = (MapHeader*)hdr;
    m->alloc.free(_map_entries(m));
    m->alloc.free(_map_occupied(m));
}

bool _map_set(void *hdr, const void *key, void *val) {
    MapHeader *m = (MapHeader*)hdr;
    if (m->len >= m->cap * _MAP_MAX_LOAD) {
        if (!_map_resize(m, m->cap * 2))
            return false;
    }

    size_t hash = m->hash(key, m->key_size);
    size_t idx = hash & (m->cap - 1);
    size_t first_tomb = m->cap;

    for (size_t i = 0; i < m->cap; i++) {
        size_t slot = (idx + i) & (m->cap - 1);
        uint8_t state = OCC(m, slot);

        if (state == 0) {
            size_t target = first_tomb < m->cap ? first_tomb : slot;
            OCC(m, target) = 1;
            memcpy(KEY(m, target), key, m->key_size);
            memcpy(VAL(m, target), val, m->val_size);
            m->len++;
            return true;
        }
        if (state == 2) {
            if (first_tomb == m->cap)
                first_tomb = slot;
            continue;
        }
        if (memcmp(KEY(m, slot), key, m->key_size) == 0) {
            memcpy(VAL(m, slot), val, m->val_size);
            return false;
        }
    }
    return false;
}

void *_map_get(void *hdr, const void *key) {
    MapHeader *m = (MapHeader*)hdr;
    if (m->cap == 0) return NULL;

    size_t hash = m->hash(key, m->key_size);
    size_t idx = hash & (m->cap - 1);

    for (size_t i = 0; i < m->cap; i++) {
        size_t slot = (idx + i) & (m->cap - 1);
        uint8_t state = OCC(m, slot);
        if (state == 0) return NULL;
        if (state == 2) continue;
        if (memcmp(KEY(m, slot), key, m->key_size) == 0)
            return VAL(m, slot);
    }
    return NULL;
}

bool _map_has(void *hdr, const void *key) {
    return _map_get(hdr, key) != NULL;
}

void _map_remove(void *hdr, const void *key) {
    MapHeader *m = (MapHeader*)hdr;
    if (m->cap == 0) return;

    size_t hash = m->hash(key, m->key_size);
    size_t idx = hash & (m->cap - 1);

    for (size_t i = 0; i < m->cap; i++) {
        size_t slot = (idx + i) & (m->cap - 1);
        uint8_t state = OCC(m, slot);
        if (state == 0) return;
        if (state == 2) continue;
        if (memcmp(KEY(m, slot), key, m->key_size) == 0) {
            OCC(m, slot) = 2;
            m->len--;
            return;
        }
    }
}

size_t _map_len(const void *hdr) {
    return ((const MapHeader*)hdr)->len;
}
