#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <slice.h>

#define TEST_MESSAGE_OFF
#include "test.h"

#include "tallocator.h"

// #include "slice.h"

int main(void) {

    int *numbers_s = sinit(int, .allocator = test_allocator);

    numbers_s[0] = 45;
    printf("%d\n", numbers_s[0]);

    // sdeinit(numbers_s);

    printf("All tests complete!");

    test_allocator_print_stats();
    return 0;
}