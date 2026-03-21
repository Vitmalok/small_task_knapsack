#pragma once

#include <cstdlib>



int randint(int a, int b) {
    // Генерирует случайное целое число в полуинтервале [a, b)
    return a + (int)((double)rand() / ((double)RAND_MAX + 1) * (double)(b-a));
}