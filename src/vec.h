#ifndef VEC_H
#define VEC_H

#include "allocator.h"
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

typedef struct {
    Allocator alloc;
    size_t elem_size;
    double growth_fct;
    size_t cap;
    size_t len;
} VecHeader;

typedef struct {
    Allocator allocator;
    size_t reserve;
    double growth_fct;
} VecOptions;

#define Vector(T) struct Vector_##T { \
    VecHeader hdr;                    \
    T *data;                          \
}

#define _vec_data(hdr) (*(void**)((char*)(hdr) + sizeof(VecHeader)))

void  _vec_init(void *hdr, size_t elem_size, VecOptions params);
void  _vec_deinit(void *hdr);
void  _vec_grow(void *hdr);
void *_vec_at(void *hdr, size_t index);
void  _vec_get(void *hdr, size_t index, void *out);
bool  _vec_reserve(void *hdr, size_t new_cap);
size_t _vec_len(const void *hdr);

#define vec_init(v, ...) \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Winitializer-overrides\"") \
    _vec_init((void*)(v), sizeof(typeof((v)->data[0])), (VecOptions){ .allocator = std_allocator, .reserve = 0, .growth_fct = 1.5, __VA_ARGS__}) \
    _Pragma("clang diagnostic pop")
#define vec_deinit(v)       _vec_deinit((void*)(v))
#define vec_len(v)          _vec_len((const void*)(v))
#define vec_push(v, e)      (_vec_grow((void*)(v)), (v)->data[(v)->hdr.len++] = (e))
#define vec_pop(v)          ((v)->hdr.len--, (v)->data[(v)->hdr.len])
#define vec_at(v, i)        ((typeof((v)->data))_vec_at((void*)(v), (i)))
#define vec_get(v, i)       (*(typeof((v)->data))_vec_at((void*)(v), (i)))
#define vec_insert(v, i, e) (_vec_grow((void*)(v)), memmove(&(v)->data[(i)+1], &(v)->data[(i)], ((v)->hdr.len - (i)) * sizeof((v)->data[0])), (v)->hdr.len++, (v)->data[(i)] = (e))
#define vec_remove(v, i)    (memmove(&(v)->data[(i)], &(v)->data[(i)+1], ((v)->hdr.len - (i) - 1) * sizeof((v)->data[0])), (v)->hdr.len--)
#define vec_reserve(v, n)   _vec_reserve((void*)(v), (n))

#endif
