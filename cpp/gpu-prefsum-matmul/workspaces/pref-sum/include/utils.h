#pragma once

#include <cstddef>
namespace utils {

using ull = unsigned long long;

size_t potolok(size_t a, size_t b);

ull ms2ns(float ms);

float ns2ms(ull ns);

// return random integer in [0,200]
unsigned int getRand();



} // namespace utils
