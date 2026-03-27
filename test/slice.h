#include "test.h"

#include <slice.h>

void test_slice_init() {
    Slice s;
    slice_init(&s, sizeof(double));

    assert(s.data == NULL);
    assert(s.size == sizeof(double));
    assert(s.cap == 0);
    assert(s.len == 0);

    slice_deinit(&s);
}

void test_slice_reserve() {
    Slice s;
    slice_init(&s, sizeof(int));
    slice_reserve(&s, 2);
    slice_reserve(&s, 9);

    assert(s.data != NULL);
    assert(s.cap >= 8);
    // printf("\n%zu\n", s.cap);
    assert(s.cap == 8 * 2);
    assert(s.len == 0);

    slice_deinit(&s);
}

void test_slice_insert() {
    Slice s;
    slice_init(&s, sizeof(double));

    slice_insert(&s, 0, &(double){0.23});
    assert(*(double*)s.data == 0.23);
    assert(s.len == 1);

    slice_insert(&s, 0, &(double){2.7});
    assert(*(double*)s.data == 2.7);
    assert(s.len == 2);

    slice_insert(&s, 2, &(double){5.7});
    assert((double*)slice_at(&s, 2) != NULL);
    assert(*(double*)slice_at(&s, 2) == 5.7);

    slice_deinit(&s);
}

void test_slice_at() {
    Slice s;
    slice_init(&s, sizeof(char));

    slice_insert(&s, 0, &(char){'s'});
    
    assert(*(char*)slice_at(&s, 0) == 's');

    slice_deinit(&s);
}

void test_slice_deinit() {
    Slice s;
    slice_init(&s, sizeof(int));
    slice_insert(&s, 0, &(int){7});

    assert(*(int*)s.data == 7);
    
    slice_deinit(&s);

    assert(s.data == NULL);
}

void test_slice_push() {
    Slice s;
    slice_init(&s, sizeof(int));

    slice_push(&s, &(int){1});
    assert(*(int*)slice_at(&s, 0) == 1);

    slice_push(&s, &(int){3});
    assert((int*)slice_at(&s, 1) != NULL);
    assert(*(int*)slice_at(&s, 1) == 3);

    slice_deinit(&s);
}

void test_slice_remove() {
    Slice s;
    slice_init(&s, sizeof(int));

    slice_push(&s, &(int){1});
    assert(slice_len(&s) == 1);

    slice_remove(&s, 0);
    assert(slice_len(&s) == 0);
    assert(s.cap == s.len);

    slice_deinit(&s);

}

void test_slice_unshift() {
    Slice s;
    slice_init(&s, sizeof(int));

    slice_unshift(&s, &(int){3});
    assert(*(int*)slice_at(&s, 0) == 3);

    slice_unshift(&s, &(int){5});
    assert(*(int*)slice_at(&s, 0) == 5);

    slice_deinit(&s);
}

void test_slice_shift() {
    Slice s;
    slice_init(&s, sizeof(int));

    slice_push(&s, &(int){4});
    slice_push(&s, &(int){10});
    assert(slice_len(&s) == 2);

    int *element = slice_shift(&s, &(int){0});
    assert(element != NULL);
    assert(*element == 4);

    assert(slice_len(&s) == 1);

    slice_deinit(&s);
}

void test_slice_pop() {
    Slice s;
    slice_init(&s, sizeof(int));

    slice_push(&s, &(int){4});
    slice_push(&s, &(int){10});
    assert(slice_len(&s) == 2);

    int *element = slice_pop(&s, &(int){0});
    assert(element != NULL);
    assert(*element == 10);

    assert(slice_len(&s) == 1);

    slice_deinit(&s);
}

void test_slice_shrink() {
    Slice s;
    slice_init(&s, sizeof(int));

    for (size_t i = 0; i < 3; i++) {
        slice_push(&s, &(int){i});
    }

    assert(s.cap >= s.len);
    slice_shrink(&s);
    assert(s.cap == s.len);
}

void test_slice_iter() {
    Slice s;
    slice_init(&s, sizeof(int));

    // printf("\n1\n");
    
    for (size_t i = 0; i < 10; i++) {
        // printf("I: %zu\n", i);
        slice_push(&s, &(int){i + 1});
    }

    // printf("2\n");

    int count = 0;
    for(SliceIter it = slice_iter(&s); slice_next(&it);) {
        assert(it.next_index == (size_t)count + 1);
        assert(*(int*)it.value == count + 1);
        
        count++;
    }

    // printf("3\n");
}

void test_slice() {
    T(test_slice_init);
    // T(test_slice_reserve);
    T(test_slice_insert);
    T(test_slice_at);
    T(test_slice_push);
    T(test_slice_remove);
    T(test_slice_unshift);
    T(test_slice_shift);
    T(test_slice_pop);
    T(test_slice_shrink);
    T(test_slice_iter);
}