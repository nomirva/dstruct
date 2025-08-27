#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "slice.h"

void slice_init(Slice *slice, size_t size) {
	assert(slice != NULL);
	assert(size != 0);

	slice->data = NULL;
	slice->size = size;
	slice->cap = 0;
	slice->len = 0;
}

void slice_deinit(Slice *slice) {
	assert(slice != NULL);
	
	free(slice->data);
	slice->data = NULL;
}

Slice *slice_assign(Slice *dest, Slice *src) {
	assert(dest != NULL);
	assert(src != NULL);
	assert(src != dest);

	if(!slice_reserve(dest, slice_len(src))) 
		return NULL;

	memcpy(dest->data, src->data, src->len);
	return dest;
}

size_t slice_len(Slice *slice) {
	assert(slice != NULL);
	return slice->len / slice->size;
}

void *slice_push(Slice *slice, void *value) {
	assert(slice != NULL);
	assert(value != NULL);
	
	return slice_insert(slice, slice_len(slice), value);
}

void *slice_unshift(Slice *slice, void *value) {
	if(!slice || !value) return NULL;
	return slice_insert(slice, 0, value);
}

void *slice_pop(Slice *slice, void *buffer) {
	if(!slice || !slice_len(slice)) return NULL;

	if(buffer) {
		memcpy(buffer, (char*)slice->data + slice->len - slice->size, slice->size);
	}
	slice_remove(slice, slice_len(slice) - 1);
	return buffer;
}

void *slice_shift(Slice *slice, void *buffer) {
	if(!slice || !buffer || !slice_len(slice)) return NULL;
	
	if(buffer) {
		memcpy(buffer, slice->data, slice->size);
	}
	slice_remove(slice, 0);
	
	return buffer;
}

void *slice_at(Slice *slice, size_t index) {
	if (!slice_contains(slice, index)) return NULL;
    return (char*)slice->data + index * slice->size;
}

void *slice_get(Slice *slice, size_t index, void *buffer) {
    if (!slice_contains(slice, index) || !buffer) return NULL;
    memcpy(buffer, (char*)slice->data + index * slice->size, slice->size);
    return buffer;
}

void *slice_set(Slice *slice, size_t index, void *data) {
	if(!slice_contains(slice, index)) return NULL;
	memcpy((char*)slice->data + index * slice->size, data, slice->size);
	return data;
}

void *slice_insert(Slice *slice, size_t index, void *value) {
	assert(slice != NULL);
	assert(value != NULL);

    if(!(slice_len(slice) >= index) ||
        (slice->cap < slice->len + slice->size && !slice_reserve(slice, 2 * (slice_len(slice) ? slice_len(slice) : 1)))) return NULL;

    size_t offset = index * slice->size;
    memmove((char*)slice->data + offset + slice->size, (char*)slice->data + offset, slice->len - offset);
    memcpy((char*)slice->data + offset, value, slice->size);
    slice->len += slice->size;
    return value;
}

void slice_remove(Slice *slice, size_t index) {
	size_t offset = index * slice->size;
	if(slice->len > offset) {
		void *dp = (char*)slice->data + offset;
		memcpy(dp, (char*)dp + slice->size, slice->len - slice->size - offset);
		slice->len -= slice->size;
		if (slice->cap >= 1.25 * slice->len) slice_shrink(slice);
	}
}

void slice_clear(Slice *slice) {
	assert(slice != NULL);
	
	slice->len = 0;
	slice_shrink(slice);
}

size_t slice_shrink(Slice *slice) {
	assert(slice != NULL);

    size_t new_cap = slice->len;
	if(slice->cap > new_cap) {
        void *new_data = realloc(slice->data, !new_cap ? slice->size : new_cap);
        if (!new_data) return slice->cap;
        slice->data = new_data;
        slice->cap = new_cap;
    }
    return slice->cap;
}

Slice *slice_reserve(Slice *slice, size_t n) {
	assert(slice != NULL);
	assert(slice != 0);

	size_t new_cap = slice->size * n;
	if(new_cap > slice->cap) {
		void *new_data = slice->data ? realloc(slice->data, new_cap) : malloc(new_cap);
		if(new_data == NULL) return NULL;
		slice->data = new_data;
		slice->cap = new_cap;
	}
	return slice;
}

bool slice_contains(Slice *slice, size_t index) {
	if(!slice || !(slice_len(slice) > index)) {
		return false;
	}
	return true;
}

SliceIter slice_iter(Slice *slice) {
	assert(slice != NULL);

    void *first = slice->len ? slice->data : NULL;
    return (SliceIter){slice, 0, first};
}

void *slice_next(SliceIter *it) {
    if (it->next_index >= it->slice->len / it->slice->size) {
        return it->value = NULL;
    }
    return it->value = (char*)it->slice->data + it->slice->size * it->next_index++;
}