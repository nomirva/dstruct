#include <assert.h>
#include <stdio.h>

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
    slice_reserve(&s, 8);

    assert(s.data != NULL);
    assert(s.cap >= sizeof(int) * 8);
    assert(s.len == 0);

    slice_deinit(&s);
}

void test_slice_insert() {
    Slice s;
    slice_init(&s, sizeof(double));

    slice_insert(&s, 0, &(double){0.23});
    assert(*(double*)s.data == 0.23);
    assert(s.len == s.size);

    slice_insert(&s, 0, &(double){2.7});
    assert(*(double*)s.data == 2.7);
    assert(s.len == s.size * 2);

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

    assert(s.cap > s.len);
    slice_shrink(&s);
    assert(s.cap == s.len);
}

void test_slice_iter() {
    Slice s;
    slice_init(&s, sizeof(int));
    
    for (size_t i = 0; i < 10; i++) {
        slice_push(&s, &(int){i + 1});
    }

    int count = 0;
    for(SliceIter it = slice_iter(&s); slice_next(&it);) {
        assert(it.next_index == (size_t)count + 1);
        assert(*(int*)it.value == count + 1);
        
        count++;
    }
}

int main(void) {

    test_slice_init();
    test_slice_reserve();
    test_slice_insert();
    test_slice_at();
    test_slice_push();
    test_slice_remove();
    test_slice_unshift();
    test_slice_shift();
    test_slice_pop();
    test_slice_shrink();
    test_slice_iter();

    printf("Test complete!");

    return 0;
}