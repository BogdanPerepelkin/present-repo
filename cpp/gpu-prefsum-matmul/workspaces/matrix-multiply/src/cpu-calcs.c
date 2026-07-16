#include "data-structures.h"
#include "lib-utils.h"
#include "multiply-matrices.h"

#include <omp.h>
#include <time.h>

size_t idx(size_t i, size_t j, size_t n) {
  return i * n + j;
}

cl_ulong helperGetTime() {
  cl_ulong nanosec = (long long) (omp_get_wtime() * 1e9);
  return nanosec; // ns
}

#pragma GCC target("avx2")

cl_int libMultiplyMatricesCpu(MNK mnk, const MatrixType* restrict a, const MatrixType* restrict b, MatrixType* c) {
  libUtLogg("libMultiplyMatricesCpu");
  cl_ulong start;
  if (libParam.ENABLE_CPU_PROFILING) {
    start = helperGetTime(); // ns
  }

  MatrixType* trB = NULL;
  if (libParam.ENABLE_CPU_TRANSPOSE) {
    trB = malloc(mnk.k * mnk.n * sizeof(MatrixType));
    if (trB == NULL) {
      return CL_OUT_OF_HOST_MEMORY;
    }
    for (size_t i = 0; i < mnk.k; i++) {
      for (size_t j = 0; j < mnk.n; j++) {
        trB[idx(j, i, mnk.k)] = b[idx(i, j, mnk.n)];
      }
    }
  }

#pragma omp parallel for schedule(static) collapse(2)
  for (size_t i = 0; i < mnk.m; i++) {
    for (size_t j = 0; j < mnk.n; j++) {
      float sum = 0.0;
#pragma omp simd reduction(+ : sum)
      for (size_t g = 0; g < mnk.k; g++) {
        if (libParam.ENABLE_CPU_TRANSPOSE) {
          sum += a[idx(i, g, mnk.k)] * trB[idx(j, g, mnk.k)];
        } else {
          sum += a[idx(i, g, mnk.k)] * b[idx(g, j, mnk.n)];
        }
      }
      c[idx(i, j, mnk.n)] = (MatrixType) sum;
    }
  }
  if (libParam.ENABLE_CPU_PROFILING) {
    cl_ulong end = helperGetTime(); // ns

    ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].profilingCpu.seconds = end - start;
    ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].profilingCpu.flops =
        (mnk.m * mnk.n * mnk.k) / (cl_float) (end - start);
  } else {
    ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].profilingCpu.seconds = 67;
    ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].profilingCpu.flops = 67;
  }

  if (libParam.ENABLE_CPU_TRANSPOSE) {
    free(trB);
  }

  return CL_SUCCESS;
}

void libGetCpuProfiling(ProfilingCPU* result) {
  *result = ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].profilingCpu;
}
