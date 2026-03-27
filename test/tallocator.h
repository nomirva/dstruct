#ifndef TALLOCATOR_H
#define TALLOCATOR_H

#include "../src/allocator.h"

#include <stddef.h>
#include <stdbool.h>

// Глобальный тестовый аллокатор
extern const Allocator test_allocator;

// Функции для отладки
void test_allocator_print_stats(void);
void test_allocator_reset_stats(void);
bool test_allocator_has_leaks(void);
size_t test_allocator_get_allocation_count(void);
size_t test_allocator_get_active_allocations(void);

#endif