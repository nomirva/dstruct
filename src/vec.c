#include "vec.h"
#include <assert.h>

void _vec_init(void *hdr, size_t elem_size, VecOptions params) {
    VecHeader *v = (VecHeader*)hdr;
    v->alloc = params.allocator;
    v->elem_size = elem_size;
    v->growth_fct = params.growth_fct;
    v->cap = 0;
    v->len = 0;
    _vec_data(hdr) = NULL;
    if (params.reserve > 0)
        _vec_reserve(hdr, params.reserve);
}

void _vec_deinit(void *hdr) {
    if (!hdr) return;
    VecHeader *v = (VecHeader*)hdr;
    void *data = _vec_data(hdr);
    if (data) v->alloc.free(data);
    _vec_data(hdr) = NULL;
    v->cap = 0;
    v->len = 0;
}

void _vec_grow(void *hdr) {
    VecHeader *v = (VecHeader*)hdr;
    if (v->len >= v->cap) {
        size_t new_cap = v->cap ? (size_t)(v->cap * v->growth_fct) : 2;
        _vec_reserve(hdr, new_cap);
    }
}

bool _vec_reserve(void *hdr, size_t new_cap) {
    VecHeader *v = (VecHeader*)hdr;
    if (new_cap <= v->cap) return true;
    void *new_data = v->alloc.realloc(_vec_data(hdr), new_cap * v->elem_size);
    if (!new_data) return false;
    _vec_data(hdr) = new_data;
    v->cap = new_cap;
    return true;
}

void *_vec_at(void *hdr, size_t index) {
    VecHeader *v = (VecHeader*)hdr;
    assert(index < v->len);
    void *data = _vec_data(hdr);
    return (char*)data + index * v->elem_size;
}

void _vec_get(void *hdr, size_t index, void *out) {
    VecHeader *v = (VecHeader*)hdr;
    assert(index < v->len);
    void *data = _vec_data(hdr);
    memcpy(out, (char*)data + index * v->elem_size, v->elem_size);
}

size_t _vec_len(const void *hdr) {
    return ((const VecHeader*)hdr)->len;
}
