#include <stdio.h>

#include "allocator.h"
#include "vec.h"

int main(void) {
    Vector(int) v;
    vec_init(&v, .allocator = std_allocator, .reserve = 2);

    vec_push(&v, 10);
    vec_push(&v, 20);
    vec_push(&v, 30);

    printf("len=%zu, [0]=%d, [1]=%d, [2]=%d\n",
        vec_len(&v), vec_get(&v, 0), vec_get(&v, 1), vec_get(&v, 2));

    *vec_at(&v, 1) = 99;
    printf("after *at(1)=99: get(1)=%d\n", vec_get(&v, 1));

    while (vec_len(&v))
        printf("pop: %d\n", vec_pop(&v));

    vec_deinit(&v);
    return 0;
}
