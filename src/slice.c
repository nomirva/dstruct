// #include <stdlib.h>
// #include <string.h>
// #include <stdio.h>

// #include "slice.h"

// // void slice_init(Slice *slice, size_t size) {
// // 	assert(slice != NULL);
// // 	assert(size != 0);

// // 	slice->allocator = std_allocator;

// // 	slice->data = NULL;
// // 	slice->size = size;
// // 	slice->cap = 0;
// // 	slice->len = 0;
// // }

// void slice_deinit(Slice *slice) {
// 	assert(slice != NULL);
	
// 	if(slice->data) {
// 		slice->allocator.free(slice->data);
// 		slice->data = NULL;
// 	}
// }

// Slice *slice_assign(Slice *dest, Slice *src) {
// 	assert(dest != NULL);
// 	assert(src != NULL);
// 	assert(src != dest);

// 	if(!slice_reserve(dest, slice_len(src))) 
// 		return NULL;

// 	memcpy(dest->data, src->data, src->len);
// 	return dest;
// }

// void *slice_push(Slice *slice, void *value) {
// 	assert(slice != NULL);
// 	assert(value != NULL);
	
// 	return slice_insert(slice, slice_len(slice), value);
// }

// void *slice_unshift(Slice *slice, void *value) {
// 	if(!slice || !value) return NULL;
// 	return slice_insert(slice, 0, value);
// }

// void *slice_pop(Slice *slice, void *buffer) {
// 	if(!slice || !slice_len(slice)) return NULL;

// 	if(buffer) {
// 		size_t size = slice->size;
// 		memcpy(buffer, (char*)slice->data + slice->len * size - size, size);
// 	}
// 	slice_remove(slice, slice_len(slice) - 1);
// 	return buffer;
// }

// void *slice_shift(Slice *slice, void *buffer) {
// 	if(!slice || !buffer || !slice_len(slice)) return NULL;
	
// 	if(buffer) {
// 		memcpy(buffer, slice->data, slice->size);
// 	}
// 	slice_remove(slice, 0);
// 	return buffer;
// }

// void *slice_at(Slice *slice, size_t index) {
// 	if (!slice_contains(slice, index)) return NULL;
//     return (char*)slice->data + index * slice->size;
// }

// void *slice_get(Slice *slice, size_t index, void *buffer) {
//     if (!slice_contains(slice, index) || !buffer) return NULL;
//     memcpy(buffer, (char*)slice->data + index * slice->size, slice->size);
//     return buffer;
// }

// void *slice_set(Slice *slice, size_t index, void *data) {
// 	if(!slice_contains(slice, index)) return NULL;
// 	memcpy((char*)slice->data + index * slice->size, data, slice->size);
// 	return data;
// }

// void *slice_insert(Slice *slice, size_t index, void *value) {
// 	assert(slice != NULL);
// 	assert(value != NULL);
// 	assert(!(index > slice_len(slice)));

// 	size_t offset = index * slice->size;
// 	size_t size = slice->size;
// 	size_t len = slice->len;

// 	if(++len > slice->cap && !slice_reserve(slice, len ? len : 1 * _SLICE_GROWTH_FACTOR))
// 		return NULL;

// 	char *pos = (char*)slice->data + offset;
//     memmove(pos + size, pos, slice->len * size - offset);
//     memcpy(pos, value, size);
//     slice->len++;
//     return value;
// }

// void slice_remove(Slice *slice, size_t index) {
// 	assert(slice != NULL);
// 	assert(slice_contains(slice, index));

// 	size_t offset = index * slice->size;
// 	size_t size = slice->size;
// 	if(slice->len > index) {
// 		void *dp = (char*)slice->data + offset;
// 		memmove(dp, (char*)dp + size, slice->len * size - size - offset);
// 		slice->len--;
// 		if (slice->cap >= 1.25 * slice->len)
// 			slice_shrink(slice);
// 	}
// }

// void slice_clear(Slice *slice) {
// 	assert(slice != NULL);
	
// 	slice->len = 0;
// 	slice_shrink(slice);
// }

// size_t slice_shrink(Slice *slice) {
// 	assert(slice != NULL);

//     size_t new_cap = slice->len;
// 	if(slice->cap > new_cap) {
//         void *new_data = slice->allocator.realloc(slice->data, new_cap * slice->size);
//         if (new_data == NULL && new_cap != 0)
// 			return slice->cap;

//         slice->data = new_data;
//         slice->cap = new_cap;
//     }
//     return slice->cap;
// }

// // Slice *slice_reserve(Slice *slice, size_t n) {
// //     assert(slice != NULL);
// //     assert(n != 0);

// //     if (n > slice->cap) {
// //         size_t new_cap = slice->cap ? slice->cap : 1;
// //         while (new_cap < n) {
// //             if (new_cap > SIZE_MAX / 2) {
// //                 new_cap = n;
// //                 break;
// //             }
// //             new_cap *= 2;
// //         }

// // 		if (slice->size != 0 && new_cap > SIZE_MAX / slice->size)
// //             return NULL;

// //         void *new_data = slice->data 
// // 			? realloc(slice->data, new_cap * slice->size) 
// // 			: malloc(new_cap * slice->size);

// //         if (new_data == NULL)
// //             return NULL;

// //         slice->data = new_data;
// //         slice->cap = new_cap;
// //     }
// //     return slice;
// // }

// Slice *slice_reserve(Slice *slice, size_t new_cap) {
//     assert(slice != NULL);
//     assert(new_cap != 0);

//     if (new_cap > slice->cap) {
//         void *new_data = slice->data 
// 			? slice->allocator.realloc(slice->data, new_cap * slice->size) 
// 			: slice->allocator.malloc(new_cap * slice->size);

//         if (new_data == NULL)
//             return NULL;

//         slice->data = new_data;
//         slice->cap = new_cap;
//     }
//     return slice;
// }

// bool slice_contains(Slice *slice, size_t index) {
// 	assert(slice != NULL);
// 	return slice_len(slice) > index;
// }

// SliceIter slice_iter(Slice *slice) {
// 	assert(slice != NULL);

//     void *first = slice->len ? slice->data : NULL;
//     return (SliceIter){slice, 0, first};
// }

// void *slice_next(SliceIter *it) {
//     if (it->next_index >= it->slice->len / it->slice->size) {
//         return it->value = NULL;
//     }
//     return it->value = (char*)it->slice->data + it->slice->size * it->next_index++;
// }