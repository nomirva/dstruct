#include <assert.h>
#include <stdio.h>

#define TEST_MESSAGE_OFF
#include "test.h"

#include "tallocator.h"
#include "vec.h"

int main(void) {
    Vector(int) v;
    vec_init(&v, .allocator = test_allocator, .reserve = 2);

    vec_push(&v, 42);
    assert(vec_get(&v, 0) == 42);

    vec_push(&v, 7);
    assert(vec_pop(&v) == 7);

    vec_deinit(&v);

    printf("All tests complete!");

    test_allocator_print_stats();
    return 0;
}
