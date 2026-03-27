#ifndef RTTI_H
#define RTTI_H

#include <stdint.h>
#include <stddef.h>
#include <stdalign.h>

typedef struct {
    uint64_t hash;
    size_t size;
    size_t align;
} TypeInfo;

static inline uint64_t _rtti_hash(const char *str) {
    uint64_t hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + (unsigned char)c; // hash * 33 + c
    return hash;
}

#define typeof(x) (TypeInfo){ \
    .hash =  _rtti_hash(#x),   \
    .size = sizeof(x),         \
    .align = alignof(x)        \
}

#define typeeq(a, b) ( \
    (a).hash == (b).hash && \
    (a).size == (b).size && \
    (a).align == (b).align \
)

#endif // RTTI_H