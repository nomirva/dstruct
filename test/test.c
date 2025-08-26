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
}

void test_slice_reserve() {
    Slice s;
    slice_init(&s, sizeof(int));
    slice_reserve(&s, 67);

    assert(s.data != NULL);
    assert(s.cap >= sizeof(int) * 67);
    assert(s.len == 0);
}

int main(void) {
    test_slice_init();
    test_slice_reserve();

    printf("Test complete!");

    return 0;
}