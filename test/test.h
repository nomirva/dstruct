#ifndef TEST_H
#define TEST_H

#include <assert.h>
#include <stdio.h>

#ifndef TEST_MESSAGE_OFF

#define T(x) do { \
    printf("\x1b[31mRunned\x1b[0m -> %s", #x); \
    fflush(stdout); \
    x(); \
    printf("\r\033[K\x1b[1;32mComplete\x1b[0m -> %s\n", #x); \
} while(0)

#else

#define T(x) x()

#endif

#endif