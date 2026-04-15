#ifndef SLICE_H
#define SLICE_H

#include "allocator.h"

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define _SLICE_GROWTH_FACTOR 1.5

typedef struct {
	Allocator allocator;
	size_t reserve;
} SliceParams;


typedef struct {
	Allocator allocator;
	size_t size;
	size_t cap;
	size_t len;
} SliceHeader;

#define _slice_header(s) ((SliceHeader*)(void*)(s) - 1)

#define sinit(type, ...) slice_init(type, __VA_ARGS__)

#define sdeinit(slice) slice_deinit(slice)

#define slice_init(type, ...) \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Winitializer-overrides\"") \
    (type*)_slice_init(sizeof(type), (SliceParams){ .allocator = std_allocator, .reserve = 2, __VA_ARGS__}) \
    _Pragma("clang diagnostic pop")

#define slice_deinit(slice) (_slice_header(slice)->allocator.free(_slice_header(slice)))

static inline void *_slice_init(size_t size, SliceParams params) {
	SliceHeader *s = params.allocator.malloc(sizeof(SliceHeader) + (size * params.reserve));
	s->allocator = params.allocator;
	s->cap = params.reserve;
	s->size = size;
	s->len = 0;
	return s + 1;
}

// void slice_clear(Slice *slice);

static inline size_t slice_len(void *slice) {
	assert(slice != NULL);
	return _slice_header(slice)->len; 
}

// void *slice_push(Slice *slice, void *value);

// void *slice_pop(Slice *slice, void *buffer);

// void *slice_shift(Slice *slice, void *buffer);

// void *slice_unshift(Slice *slice, void *buffer);

// void *slice_at(Slice *slice, size_t index);

// void *slice_get(Slice *slice, size_t index, void *buffer);

// void *slice_set(Slice *slice, size_t index, void *value);

// void *slice_insert(Slice *slice, size_t index, void *value);

// void slice_remove(Slice *slice, size_t index);

// size_t slice_shrink(Slice *slice);

// Slice *slice_reserve(Slice *slice, size_t new_cap);

// bool slice_contains(Slice *slice, size_t index);

// SliceIter slice_iter(Slice* slice);

// void *slice_next(SliceIter *iterator);

// Slice *slice_assign(Slice *dest, Slice *src);

/* TODO BLOCK */
// slice_earase

// Slice *slice_fill(Slice *sp, int first, int second, void *data);

// void slice_filter(Comparator comp);

// int slice_swap(Slice *s, size_t first, int second);

// Slice *slice_assign(Slice *dest, Slice* src);

#endif