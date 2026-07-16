#include "cpu-calcs.h"

#include "multiply-matrices.h"
#include "utils.h"

#include <math.h>
#include <omp.h>
#include <time.h>

#include <chrono>
#include <iostream>

bool enableCPUProfiling = false;

size_t idx(size_t i, size_t j, size_t n) {
  return i * n + j;
}

cl_ulong helperGetTime() {
  cl_ulong nanosec = (long long) (omp_get_wtime() * 1e9);
  printf("%llu\n", nanosec);
  return nanosec; // ns
}

cl_int cpuCalc(
    size_t m,
    size_t n,
    size_t k,
    const MatrixType* a,
    const MatrixType* b,
    MatrixType* c,
    cl_ulong* time,
    double* flops
) {
  cl_ulong start;
  if (enableCPUProfiling) {
    start = helperGetTime(); // ns
  }
#pragma omp parallel for
  for (size_t i = 0; i < m; i++) {
#pragma omp parallel for
    for (size_t j = 0; j < n; j++) {
      float sum = 0;
      for (size_t g = 0; g < k; g++) {
        sum += a[idx(i, g, k)] * b[idx(g, j, n)];
      }
      c[idx(i, j, n)] = sum;
    }
  }
  if (enableCPUProfiling) {
    cl_ulong end = helperGetTime(); // ns
    std::cout << "start = " << start << std::endl;
    std::cout << "end = " << end << std::endl;
    *time = end - start;
    *flops = ((double) (m * n * k)) / (((double) *time) / pow(10, 9)); // 1 / s
  }

  return CL_SUCCESS;
}

bool equals(float a, float b) {
  if (fabs(a - b) > 0.01f) {
    return false;
  }
  return true;
}

void printABC(
    const MatrixType* a,
    const MatrixType* b,
    const MatrixType* c,
    const MatrixType* cpuC,
    size_t aSize,
    size_t bSize,
    size_t cSize
) {
  printf("a.size = %zu\n", aSize);
  printf("b.size = %zu\n", bSize);
  if (aSize < 100 && bSize < 100) {
    printf("matrix a:\n|  ");
    for (size_t i = 0; i < aSize; i++) {
      printf("%f ", a[i]);
    }
    printf("\nx\n");

    printf("matrix b:\n|  ");
    for (size_t i = 0; i < bSize; i++) {
      printf("%f ", b[i]);
    }
    printf("\n||\n");

    printf("result on device:\n|  ");
    for (size_t i = 0; i < cSize; i++) {
      printf("%f ", c[i]);
    }
    printf("\n");

    printf("result on host:\n|  ");
    for (size_t i = 0; i < cSize; i++) {
      printf("%f ", cpuC[i]);
    }
    printf("\n");
  }
}

void printPerformance(cl_ulong cpuTime, double cpuFlops, size_t amountOperations) {
  printf("=================================[[performance]]=================================\n");
  printf("amount operations = %zu\n", amountOperations);
  if (enableCPUProfiling) {
    printf("cpu:\n");
    printf("|  time = %llu ns (%f s)\n", cpuTime, cpuTime / pow(10, 9));
    printf("|  flops = %Lf GHz\n", cpuFlops / (pow((long double) 10, (long double) 9)));
  }
  if (enableGPUProfiling) {
    cl_ulong gpuTime;
    double gpuFlops;
    libGetTimeFlops(&gpuTime, &gpuFlops);
    printf("gpu:\n");
    printf("|  time = %llu ns (%f s)\n", gpuTime, gpuTime / pow(10, 9));
    printf("|  flops = %Lf GHz \n", gpuFlops / (pow((long double) 10, (long double) 9)));
  }
}
