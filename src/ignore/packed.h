#ifndef PACKED_H
#define PACKED_H

#include "slice.h"

#define _PACKED_PAGE_SIZE 2048

#define _PACKED_MAX_INDEX (size_t) (_PACKED_PAGE_SIZE - 1) * (_PACKED_PAGE_SIZE - 1)

#define _PACKED_TOMBSTONE (size_t) 0 - 1

typedef struct {
    size_t **sparse;
    Slice dense;
    Slice back;
} Packed;

#define PACKED_EACH(packed) SLICE_EACH(packed->dense)

Packed *packed_init(Packed *pkd, size_t size);

void packed_deinit(Packed *pkd);

void *packed_at(Packed *packed, size_t index);

void *packed_get(Packed *packed, size_t index, void *buffer);

void *packed_set(Packed *pkd, size_t index, void *value);

size_t packed_len(Packed *packed);

void packed_remove(Packed *pkd, size_t index);

bool packed_contains(Packed *pkd, size_t index);

#endif
// for remove
// TODO: если удаляется последний элемент на странице, то начинается цикличная проверка всех элементов к началу слайса на инвалидность, до первого валидного элемента по который произойдёт resize 