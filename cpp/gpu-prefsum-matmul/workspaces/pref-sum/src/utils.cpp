
#include "../include/utils.h"

#include <chrono>
#include <cstddef>
#include <random>

namespace utils {

size_t potolok(size_t a, size_t b) {
  return (a + b - 1) / b;
}

ull ms2ns(float ms) {
  auto dur_ms = std::chrono::duration<float, std::milli>(ms);
  auto dur_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(dur_ms);
  return static_cast<unsigned long long>(dur_ns.count());
}

float ns2ms(ull ns) {
  return (double) ns / 1000000;
}

unsigned int getRand() {
  thread_local std::mt19937 rng((unsigned)std::chrono::high_resolution_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<unsigned int> dist(0u, 200u);
  return dist(rng);
}

} // namespace utils
