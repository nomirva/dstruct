#include <assert.h>
#include <stdio.h>
#include <string.h>

#define TEST_MESSAGE_OFF
#include "test.h"

#include "map.h"
#include "tallocator.h"

typedef struct { int x; int y; } Point;
typedef const char Str64[64];

void test_map_int_int(void) {
    Map(int, int) m;
    map_init(&m, test_allocator, 0);
    assert(map_len(&m) == 0);

    map_set(&m, 10, 100);
    map_set(&m, 20, 200);
    map_set(&m, 30, 300);
    assert(map_len(&m) == 3);

    assert(map_has(&m, 10));
    assert(map_has(&m, 20));
    assert(map_has(&m, 30));
    assert(!map_has(&m, 99));

    assert(*map_get(&m, 10) == 100);
    assert(*map_get(&m, 20) == 200);
    assert(*map_get(&m, 30) == 300);
    assert(map_get(&m, 99) == NULL);

    *map_get(&m, 20) = 999;
    assert(*map_get(&m, 20) == 999);

    map_remove(&m, 20);
    assert(!map_has(&m, 20));
    assert(map_len(&m) == 2);
    assert(*map_get(&m, 10) == 100);
    assert(*map_get(&m, 30) == 300);

    map_set(&m, 20, 888);
    assert(*map_get(&m, 20) == 888);

    map_deinit(&m);
}

void test_map_update(void) {
    Map(int, int) m;
    map_init(&m, test_allocator, 4);

    map_set(&m, 1, 10);
    map_set(&m, 1, 99);
    assert(*map_get(&m, 1) == 99);
    assert(map_len(&m) == 1);

    map_deinit(&m);
}

void test_map_many(void) {
    Map(int, int) m;
    map_init(&m, test_allocator, 4);

    int n = 1000;
    for (int i = 0; i < n; i++)
        map_set(&m, i, i * 2);

    assert(map_len(&m) == (size_t)n);

    for (int i = 0; i < n; i++)
        assert(*map_get(&m, i) == i * 2);

    for (int i = 0; i < n; i += 2)
        map_remove(&m, i);

    assert(map_len(&m) == (size_t)n / 2);

    for (int i = 0; i < n; i++) {
        if (i % 2 == 0)
            assert(!map_has(&m, i));
        else
            assert(*map_get(&m, i) == i * 2);
    }

    map_deinit(&m);
}

void test_map_str_key(void) {
    Map(Str64, int) m;
    map_init(&m, test_allocator, 4);

    map_set(&m, "hello", 1);
    map_set(&m, "world", 2);
    map_set(&m, "foo", 3);

    assert(*map_get(&m, "hello") == 1);
    assert(*map_get(&m, "world") == 2);
    assert(*map_get(&m, "foo") == 3);
    assert(map_get(&m, "nonexistent") == NULL);

    map_remove(&m, "hello");
    assert(!map_has(&m, "hello"));
    assert(*map_get(&m, "world") == 2);

    map_deinit(&m);
}

void test_map_struct_val(void) {
    Map(int, Point) m;
    map_init(&m, test_allocator, 4);

    map_set(&m, 1, ((Point){10, 20}));
    map_set(&m, 2, ((Point){30, 40}));

    assert(map_get(&m, 1)->x == 10);
    assert(map_get(&m, 1)->y == 20);
    assert(map_get(&m, 2)->x == 30);
    assert(map_get(&m, 2)->y == 40);

    map_get(&m, 1)->x = 99;
    assert(map_get(&m, 1)->x == 99);

    map_deinit(&m);
}

void test_map(void) {
    T(test_map_int_int);
    T(test_map_update);
    T(test_map_many);
    T(test_map_str_key);
    T(test_map_struct_val);
}

int main(void) {
    test_allocator_reset_stats();

    test_map();

    assert(!test_allocator_has_leaks());
    printf("\nAll map tests passed!\n");
    test_allocator_print_stats();
    return 0;
}
