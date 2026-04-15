#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "slice.h"
#include "packed.h"

Packed *packed_init(Packed *pkd, size_t size) {
    assert(pkd != NULL);
    assert(size != 0);
    
    pkd->sparse = (size_t**)calloc(_PACKED_PAGE_SIZE, sizeof(void*)); /* FIXME: do not alloc on init*/

    if (!pkd->sparse) return NULL;

    slice_init(&pkd->dense, size);
    slice_init(&pkd->back, sizeof(size_t));
    
    return pkd;
}

void packed_deinit(Packed *pkd) {
    assert(pkd != NULL);
    assert(pkd->sparse != NULL);

    slice_deinit(&pkd->dense);
    slice_deinit(&pkd->back);

    for (size_t i = 0; i < _PACKED_PAGE_SIZE; ++i) {
        if (pkd->sparse[i]) 
            free(pkd->sparse[i]);
    }
    free(pkd->sparse);
}

void *packed_at(Packed *packed, size_t index) {
    if(!packed_contains(packed, index)) return NULL;
    return slice_at(&packed->dense, packed->sparse[index / _PACKED_PAGE_SIZE][index % _PACKED_PAGE_SIZE]);
}

void *packed_get(Packed *packed, size_t index, void *buffer) {
    if(!packed_contains(packed, index) || !buffer) return NULL;

    if(!slice_get(&packed->dense, packed->sparse[index / _PACKED_PAGE_SIZE][index % _PACKED_PAGE_SIZE], buffer)) return NULL;
    return buffer;
}


void* packed_set(Packed* packed, size_t index, void* value) {
    if (!packed || !value || index > _PACKED_MAX_INDEX) return NULL;

    const size_t page_index = index / _PACKED_PAGE_SIZE;
    const size_t slot_index = index % _PACKED_PAGE_SIZE;

    if (!packed->sparse[page_index]) {
        packed->sparse[page_index] = malloc(_PACKED_PAGE_SIZE * sizeof(size_t));
        if (!packed->sparse[page_index]) return NULL;
        
        for (size_t i = 0; i < _PACKED_PAGE_SIZE; i++) {
            packed->sparse[page_index][i] = _PACKED_TOMBSTONE;
        }
    }

    size_t *page = packed->sparse[page_index];
    size_t *slot = &page[slot_index];

    if (*slot == _PACKED_TOMBSTONE) {
        if (!slice_push(&packed->dense, value)) {
            return NULL;
        }

        size_t new_index = slice_len(&packed->dense) - 1;
        
        *slot = new_index;
        
        if (!slice_push(&packed->back, &index)) {
            slice_pop(&packed->dense, NULL);
            *slot = _PACKED_TOMBSTONE;
            return NULL;
        }

    } else {
        if (!slice_set(&packed->dense, *slot, value)) {
            return NULL;
        }
    }

    return value;
}

void packed_remove(Packed *packed, size_t index) {
    if(!packed_contains(packed, index)) return;

    size_t *page = packed->sparse[index / _PACKED_PAGE_SIZE];
    size_t *slot = &page[index % _PACKED_PAGE_SIZE];
    if (*slot == _PACKED_TOMBSTONE) return;

    size_t dense_len = slice_len(&packed->dense);
    size_t back_len = slice_len(&packed->back);

    if (dense_len > 1) {
        void *last_dense = slice_at(&packed->dense, dense_len - 1);
        size_t swap_index = *(size_t*)slice_at(&packed->back, back_len - 1);

        slice_set(&packed->dense, *slot, last_dense);
        packed->sparse[swap_index / _PACKED_PAGE_SIZE][swap_index % _PACKED_PAGE_SIZE] = *slot;

        slice_remove(&packed->dense, dense_len - 1);
        slice_remove(&packed->back, back_len - 1);
    } else {
        slice_remove(&packed->dense, 0);
        slice_remove(&packed->back, 0);
    }

    *slot = _PACKED_TOMBSTONE;
}

bool packed_contains(Packed *packed, size_t index) {
    if(index > _PACKED_MAX_INDEX || !packed) return false;

    size_t *page = packed->sparse[index / _PACKED_PAGE_SIZE];
    
    if(!page || page[index % _PACKED_PAGE_SIZE] == _PACKED_TOMBSTONE) return false;
    
    return true;
}

size_t packed_len(Packed *packed) {
    if(!packed) return 0;
    return slice_len(&packed->dense);
}