#include <stdio.h>

#include "allocator.h"
#include "map.h"

typedef const char Str64[64];
typedef const char* CStr;
typedef struct { int x; int y; } Point;

static void example_int_int(void) {
    Map(int, int) m;
    map_init(&m, std_allocator, 0);

    map_set(&m, 1, 100);
    map_set(&m, 2, 200);
    map_set(&m, 3, 300);

    printf("int->int: len=%zu, get(1)=%d, get(2)=%d, get(3)=%d\n",
        map_len(&m), *map_get(&m, 1), *map_get(&m, 2), *map_get(&m, 3));

    *map_get(&m, 2) = 999;
    printf("  after *get(2)=999: get(2)=%d\n", *map_get(&m, 2));

    map_remove(&m, 1);
    printf("  after remove(1): len=%zu, has(1)=%d\n\n",
        map_len(&m), map_has(&m, 1));

    map_deinit(&m);
}

static void example_str_int(void) {
    Map(Str64, int) m;
    map_init(&m, std_allocator, 0);

    map_set(&m, "hello", 1);
    map_set(&m, "world", 2);
    map_set(&m, "foo", 3);

    printf("str->int: len=%zu, get(hello)=%d, get(world)=%d, get(foo)=%d\n",
        map_len(&m), *map_get(&m, "hello"), *map_get(&m, "world"), *map_get(&m, "foo"));

    *map_get(&m, "world") = 999;
    printf("  after *get(world)=999: get(world)=%d\n", *map_get(&m, "world"));

    map_remove(&m, "hello");
    printf("  after remove(hello): len=%zu, has(hello)=%d\n\n",
        map_len(&m), map_has(&m, "hello"));

    map_deinit(&m);
}

static void example_cstr_int(void) {
    Map(CStr, int) m;
    map_init(&m, std_allocator, 0);

    map_set(&m, "one", 1);
    map_set(&m, "two", 2);
    map_set(&m, "three", 3);

    printf("cstr->int: len=%zu\n", map_len(&m));
    printf("  get(one)=%d\n", *map_get(&m, "one"));
    printf("  get(two)=%d\n", *map_get(&m, "two"));
    printf("  get(three)=%d\n", *map_get(&m, "three"));

    *map_get(&m, "two") = 999;
    printf("  after *get(two)=999: get(two)=%d\n\n", *map_get(&m, "two"));

    map_deinit(&m);
}

static void example_int_struct(void) {
    Map(int, Point) m;
    map_init(&m, std_allocator, 0);

    map_set(&m, 1, ((Point){10, 20}));
    map_set(&m, 2, ((Point){30, 40}));

    printf("int->Point: len=%zu\n", map_len(&m));
    printf("  get(1): x=%d, y=%d\n", map_get(&m, 1)->x, map_get(&m, 1)->y);
    printf("  get(2): x=%d, y=%d\n", map_get(&m, 2)->x, map_get(&m, 2)->y);

    map_get(&m, 1)->x = 99;
    printf("  after get(1)->x=99: x=%d, y=%d\n\n",
        map_get(&m, 1)->x, map_get(&m, 1)->y);

    map_deinit(&m);
}

static void example_int_double(void) {
    Map(int, double) m;
    map_init(&m, std_allocator, 0);

    map_set(&m, 1, 10.5);
    map_set(&m, 2, 20.5);
    map_set(&m, 3, 30.5);

    printf("int->double: len=%zu\n", map_len(&m));
    printf("  get(1)=%.1f\n", *map_get(&m, 1));
    printf("  get(2)=%.1f\n", *map_get(&m, 2));
    printf("  get(3)=%.1f\n", *map_get(&m, 3));

    *map_get(&m, 2) = 99.9;
    printf("  after *get(2)=99.9: get(2)=%.1f\n\n", *map_get(&m, 2));

    map_deinit(&m);
}

int main(void) {
    example_int_int();
    example_str_int();
    example_cstr_int();
    example_int_struct();
    example_int_double();
    return 0;
}
