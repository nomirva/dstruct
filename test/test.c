#include <stdio.h>

#include <slice.h>

int main(void) {
    Slice s;

    slice_init(&s , sizeof(int));
    
    for (size_t i = 0; i < 10; i++) {
        slice_push(&s, &(int){i});
    }
    
    for(SliceIter it = slice_iter(&s); slice_next(&it);) {
        int *element = it.value;
        printf("Value: %d\n", *element);
    }

    printf("Cap: %zu\n", s.cap);

    slice_clear(&s);

    printf("Cap: %zu\n", s.cap);

    slice_deinit(&s);

    return 0;
}