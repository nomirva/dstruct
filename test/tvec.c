#include <assert.h>
#include <stdio.h>
#include <string.h>

#define TEST_MESSAGE_OFF
#include "test.h"

#include "vec.h"
#include "tallocator.h"

void test_vec_int_push_pop(void) {
    Vector(int) v;
    vec_init(&v, .allocator = test_allocator, .reserve = 2);
    assert(vec_len(&v) == 0);

    vec_push(&v, 42);
    assert(vec_len(&v) == 1);
    assert(vec_get(&v, 0) == 42);

    vec_push(&v, 7);
    assert(vec_len(&v) == 2);
    assert(vec_get(&v, 1) == 7);

    vec_push(&v, 99);
    assert(vec_len(&v) == 3);
    assert(vec_get(&v, 2) == 99);

    assert(vec_pop(&v) == 99);
    assert(vec_len(&v) == 2);

    vec_deinit(&v);
}

void test_vec_float(void) {
    Vector(float) v;
    vec_init(&v, .allocator = test_allocator, .reserve = 4);

    vec_push(&v, 3.14f);
    vec_push(&v, 2.71f);
    assert(vec_len(&v) == 2);
    assert(vec_get(&v, 0) == 3.14f);
    assert(vec_get(&v, 1) == 2.71f);

    *vec_at(&v, 0) = 1.0f;
    assert(vec_get(&v, 0) == 1.0f);

    vec_deinit(&v);
}

void test_vec_double(void) {
    Vector(double) v;
    vec_init(&v, .allocator = test_allocator);

    for (size_t i = 0; i < 100; i++)
        vec_push(&v, (double)i * 1.5);

    assert(vec_len(&v) == 100);
    for (size_t i = 0; i < 100; i++)
        assert(vec_get(&v, i) == (double)i * 1.5);

    vec_deinit(&v);
}

void test_vec_insert_remove(void) {
    Vector(int) v;
    vec_init(&v, .allocator = test_allocator, .reserve = 2);

    vec_push(&v, 10);
    vec_push(&v, 30);
    vec_insert(&v, 1, 20);

    assert(vec_len(&v) == 3);
    assert(vec_get(&v, 0) == 10);
    assert(vec_get(&v, 1) == 20);
    assert(vec_get(&v, 2) == 30);

    vec_remove(&v, 1);
    assert(vec_len(&v) == 2);
    assert(vec_get(&v, 0) == 10);
    assert(vec_get(&v, 1) == 30);

    vec_deinit(&v);
}

void test_vec_reserve(void) {
    Vector(int) v;
    vec_init(&v, .allocator = test_allocator);

    assert(vec_reserve(&v, 64));
    assert(v.hdr.cap >= 64);

    vec_deinit(&v);
}

void test_vec_of_struct(void) {
    typedef struct { int x; int y; } Point;

    Vector(Point) v;
    vec_init(&v, .allocator = test_allocator, .reserve = 4);

    Point a = {1, 2};
    Point b = {3, 4};
    vec_push(&v, a);
    vec_push(&v, b);
    assert(vec_len(&v) == 2);
    assert(vec_at(&v, 0)->x == 1);
    assert(vec_at(&v, 0)->y == 2);
    assert(vec_at(&v, 1)->x == 3);
    assert(vec_at(&v, 1)->y == 4);

    Point p = vec_pop(&v);
    assert(p.x == 3 && p.y == 4);
    assert(vec_len(&v) == 1);

    vec_at(&v, 0)->x = 99;
    assert(vec_get(&v, 0).x == 99);

    vec_deinit(&v);
}

void test_vec_ptr_element(void) {
    typedef const char *String;

    Vector(String) v;
    vec_init(&v, .allocator = test_allocator, .reserve = 4);

    String a = "hello";
    String b = "world";
    vec_push(&v, a);
    vec_push(&v, b);
    assert(vec_len(&v) == 2);
    assert(strcmp(vec_get(&v, 0), "hello") == 0);
    assert(strcmp(vec_get(&v, 1), "world") == 0);

    String s = vec_pop(&v);
    assert(strcmp(s, "world") == 0);
    assert(vec_len(&v) == 1);

    vec_deinit(&v);
}

void test_vec_many_vectors_of_same_type(void) {
    typedef Vector(int) VecInt;

    VecInt a, b;
    vec_init(&a, .allocator = test_allocator, .reserve = 4);
    vec_init(&b, .allocator = test_allocator, .reserve = 4);

    vec_push(&a, 1);
    vec_push(&a, 2);
    vec_push(&b, 10);
    vec_push(&b, 20);

    assert(vec_get(&a, 0) == 1);
    assert(vec_get(&a, 1) == 2);
    assert(vec_get(&b, 0) == 10);
    assert(vec_get(&b, 1) == 20);

    vec_deinit(&a);
    vec_deinit(&b);
}

void test_vec_init_on_stack(void) {
    Vector(int) v;
    vec_init(&v, .allocator = test_allocator, .reserve = 4);

    assert(vec_len(&v) == 0);

    vec_push(&v, 10);
    vec_push(&v, 20);
    vec_push(&v, 30);
    assert(vec_len(&v) == 3);
    assert(vec_get(&v, 0) == 10);
    assert(vec_get(&v, 1) == 20);
    assert(vec_get(&v, 2) == 30);

    assert(vec_pop(&v) == 30);
    assert(vec_len(&v) == 2);

    *vec_at(&v, 1) = 99;
    assert(vec_get(&v, 1) == 99);

    vec_deinit(&v);
    assert(v.hdr.len == 0);
    assert(v.data == NULL);
}

void test_vec(void) {
    T(test_vec_int_push_pop);
    T(test_vec_float);
    T(test_vec_double);
    T(test_vec_insert_remove);
    T(test_vec_reserve);
    T(test_vec_of_struct);
    T(test_vec_ptr_element);
    T(test_vec_many_vectors_of_same_type);
    T(test_vec_init_on_stack);
}

int main(void) {
    test_allocator_reset_stats();

    test_vec();

    assert(!test_allocator_has_leaks());
    printf("\nAll vec tests passed!\n");
    test_allocator_print_stats();
    return 0;
}
