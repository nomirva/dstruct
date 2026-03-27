#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <slice.h>

#define TEST_MESSAGE_OFF
#include "test.h"


#include "slice.h"

int main(void) {

    test_slice();

    printf("All tests complete!");
    test_allocator_print_stats();

    return 0;
}