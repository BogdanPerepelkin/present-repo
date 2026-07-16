#include "all-tests.h"
#include "data-structures.h"
#include "multiply-matrices.h"
#include "test-utils.h"

#ifdef __APPLE__
#include <OpenCL/cl_platform.h>
#else
#include <CL/cl_platform.h>
#endif

#include <assert.h>
#include <complex.h>
#include <omp.h>

void test_small() {
  RunInfo runInfo = {true, 1};

  libParam.CALCULATING_MODE = V_2;
  libParam.v2Choosen.TILE_SIZE_W = 1;
  libParam.v2Choosen.TILE_SIZE_H = 4;
  libParam.v2Choosen.THREAD_RANGE_W = 3;
  libParam.v2Choosen.THREAD_RANGE_H = 2;

  TargetResult res = utRunTest(4, 5, 4, "small", runInfo);
  printf("misses amount = %zu\n", res.diff.missesAmount);
}

void test_big() {
  size_t size = 2000;
  size_t m = size;
  size_t n = size;
  size_t k = size;
  RunInfo runInfo = {true, 1};

  utRunTest(m, n, k, "test_big", runInfo);
}

void test_stress_v1() {
  int rememberMe = libParam.ENABLE_CPU_PROFILING;
  libParam.ENABLE_CPU_PROFILING = true;
  RunInfo runInfo = {false, 1};
  for (size_t i = 5; i < 10; i++) {
    for (size_t j = 5; j < 10; j++) {
      for (size_t k = 5; k < 10; k++) {
        TargetResult result = utRunTest(i, j, k, nullptr, runInfo);
        size_t misses = result.diff.missesAmount;
        assert(misses == 0); // NDEBUG
        if (misses != 0) {
          exit(0);
        }
      }
    }
  }
  printf("success correctness test v1\n");
  libParam.ENABLE_CPU_PROFILING = rememberMe;
}

size_t min_func(size_t a, size_t b) {
  return a < b ? a : b;
}

void test_tiles() {
  size_t rememberMe = libParam.SHORT_TILE_SIZE;
  size_t mSize = 2000;
  size_t m, n, k;
  m = n = k = mSize;
  RunInfo runInfo = {true, 1};
  for (size_t tileSize = 5; tileSize < 17; tileSize++) {
    libParam.SHORT_TILE_SIZE = tileSize;
    utRunTest(m, n, k, nullptr, runInfo);
  }
  libParam.SHORT_TILE_SIZE = rememberMe;
}
