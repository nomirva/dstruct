#include "allocator.h"

#include <stdlib.h>

const Allocator std_allocator = {
    .malloc = malloc,
    .free = free,
    .realloc = realloc,
    .calloc = calloc
};