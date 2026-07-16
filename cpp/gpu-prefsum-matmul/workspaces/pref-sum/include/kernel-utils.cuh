#pragma once

#ifdef __APPLE__
// #include "device_atomic_functions.h"
// #include "device_functions.h"
#include "host_defines.h"
#endif

#include "../include/descriptor-types.h"

#include <cstddef>

using ull = unsigned long long;
using uint = unsigned int;

struct KernelKoggeStoneParameters {
  KernelKoggeStoneParameters(size_t n, Descriptor* d, size_t l)
      : n(n)
      , descriptors(d)
      , localGroupSize(l) {}

  KernelKoggeStoneParameters() = delete;

  size_t n;
  Descriptor* descriptors;
  size_t localGroupSize;
};

struct KernelBrentStoneParameters {
  KernelBrentStoneParameters(size_t n, Descriptor* d, size_t l, size_t p)
      : n(n)
      , descriptors(d)
      , localGroupSize(l)
      , partitioin(p) {}

  KernelBrentStoneParameters() = delete;

  size_t n;
  Descriptor* descriptors;
  size_t localGroupSize;
  size_t partitioin;
};

struct KernelVolatileParameters {
  KernelVolatileParameters() = delete;

  size_t n;
  volatile Descriptor* descriptors;
  size_t localGroupSize;
  size_t partitioin;
};

inline static __device__ size_t ceilLog2(size_t n) {
  if (n <= 1) {
    return 0;
  }
  --n;
  size_t r = 0;
  while (n) {
    n >>= 1;
    ++r;
  }
  return r;
}

inline static __device__ size_t pow(size_t base, size_t exp) {
  size_t result = 1;
  while (exp) {
    if (exp & 1) {
      result *= base;
    }
    base *= base;
    exp >>= 1;
  }
  return result;
}

namespace atomic {
inline static __device__ Condition read(int* condition) {
  // return static_cast<Condition>(atomicAdd(condition, 0));
  return static_cast<Condition>(atomicCAS(condition, 0, 0));
}

inline static __device__ void set(int* condition, Condition newValue) {
  atomicExch(condition, static_cast<int>(newValue));
}
} // namespace atomic
