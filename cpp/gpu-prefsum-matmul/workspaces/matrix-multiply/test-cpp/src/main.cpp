extern "C" {
#include "multiply-matrices.h"
#include "utils.h"
}

#include "cpu-calcs.h"

// #include <cmath.h>
#include <gtest/gtest.h>
#include <stdlib.h>

#include <iostream>
#include <type_traits>
#include <vector>

void testImpl(
    const std::string& name,
    size_t m,
    size_t n,
    size_t k,
    const std::vector<MatrixType>& srcA,
    const std::vector<MatrixType>& srcB
) {
  std::cout << "==============================================================================================="
               "========="
               "=======\n====================================================[["
            << name
            << "]]========================"
               "====="
               "===============\n";

  size_t cSize = m * n;
  std::vector<MatrixType> gpuResult;
  gpuResult.resize(cSize, 0);
  libMultiplyMatrices(m, n, k, srcA.data(), srcB.data(), gpuResult.data());

  std::vector<MatrixType> cpuResult;
  cpuResult.resize(cSize, 0);
  cl_ulong cpuTime;
  double cpuFlops;

  if (enableCPUProfiling) {
    cpuCalc(m, n, k, srcA.data(), srcB.data(), cpuResult.data(), &cpuTime, &cpuFlops);

    for (size_t i = 0; i < cSize; i++) {
      if (cpuResult[i] != gpuResult[i]) {
        printf("correct = %f, gpu = %f", cpuResult[i], gpuResult[i]);
      }
    }
    std::cout << std::endl;
  }

  printABC(srcA.data(), srcB.data(), gpuResult.data(), cpuResult.data(), m * k, n * k, cSize);
  printPerformance(cpuTime, cpuFlops, m * n * k);
}

TEST(LibMulTest, T1) {
  size_t m = 4;
  size_t n = 2;
  size_t k = 3;

  std::vector<MatrixType> a;
  a.resize(m * k);

  std::vector<MatrixType> b;
  b.resize(n * k);

  for (size_t i = 0; i < m * k; i++) {
    float rNum = ((float) rand()) / ((float) rand());
    a[i] = rNum;
  }
  for (size_t i = 0; i < n * k; i++) {
    float rNum = ((float) rand()) / ((float) rand());
    b[i] = rNum;
  }
  testImpl("test_01", m, n, k, a, b);
}

TEST(LibMulTest, T2) {
  size_t size = 1500;
  size_t m = size;
  size_t n = size;
  size_t k = size;

  std::vector<MatrixType> a;
  a.resize(m * k);

  std::vector<MatrixType> b;
  b.resize(n * k);

  for (size_t i = 0; i < m * k; i++) {
    float rNum = ((float) rand()) / ((float) rand());
    a[i] = rNum;
  }
  for (size_t i = 0; i < n * k; i++) {
    float rNum = ((float) rand()) / ((float) rand());
    b[i] = rNum;
  }
  testImpl("test_02", m, n, k, a, b);
}

int main() {
  std::cout << "=====================================================================&& hello from main "
               "&&=====================================================================\n";

  MatrixMulLoggingFlag = false;
  enableGPUProfiling = true;
  enableCPUProfiling = true;

  libInitializeContext();

  printf("max frequency (from developer) = %u MHz\n", libGetMaxFrequency());

  ::testing::InitGoogleTest();
  RUN_ALL_TESTS();

  libDestroyContext();
}
