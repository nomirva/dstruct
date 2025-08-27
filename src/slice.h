#ifndef SLICE_H
#define SLICE_H

#include <stdbool.h>

typedef bool (*Comparator)(void*);

typedef struct {
	void *data;
	size_t size; // element size in byte
	size_t cap; // capacity
	size_t len; // length in byte
} Slice;

typedef struct {
	Slice *slice;
    size_t next_index;
	void *value;
} SliceIter;

void slice_init(Slice *slice, size_t size);

void slice_deinit(Slice *slice);

void slice_clear(Slice *slice);

size_t slice_len(Slice *slice);

void *slice_push(Slice *slice, void *value);

void *slice_pop(Slice *slice, void *buffer);

void *slice_shift(Slice *slice, void *buffer);

void *slice_unshift(Slice *slice, void *buffer);

void *slice_at(Slice *slice, size_t index);

void *slice_get(Slice *slice, size_t index, void *buffer);

void *slice_set(Slice *slice, size_t index, void *value);

void *slice_insert(Slice *slice, size_t index, void *value);

void slice_remove(Slice *slice, size_t index);

size_t slice_shrink(Slice *slice);

Slice *slice_reserve(Slice *slice, size_t new_cap);

bool slice_contains(Slice *slice, size_t index);

SliceIter slice_iter(Slice* slice);

void *slice_next(SliceIter *iterator);

Slice *slice_assign(Slice *dest, Slice *src);

/* TODO BLOCK */
// slice_earase

// Slice *slice_fill(Slice *sp, int first, int second, void *data);

// void slice_filter(Comparator comp);

// int slice_swap(Slice *s, size_t first, int second);

// Slice *slice_assign(Slice *dest, Slice* src);

#endif