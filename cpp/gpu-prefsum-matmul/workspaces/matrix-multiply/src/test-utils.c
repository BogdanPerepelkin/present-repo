#include "test-utils.h"

#include "data-structures.h"
#include "multiply-matrices.h"

#include <errno.h>
#include <inttypes.h>
#include <math.h>
#include <omp.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

float randomFactor = 100;
float randomShift = -50;

float utGetRandomValue() {
  return ((float) rand() / (float) RAND_MAX) * randomFactor - randomShift;
}

TargetResult utTestImpl(
    const char* name,
    MNK mnk,
    MatrixType* srcA,
    MatrixType* srcB,
    RunInfo runInfo,
    MatrixType* alignedA,
    MatrixType* alignedB
) {
  if (runInfo.enableLog) {
    printf("===================\n==[[%s]]==\n===================\n", name);
  }
  size_t cSize = mnk.m * mnk.n;
  MatrixType* gpuResult = malloc(cSize * sizeof(MatrixType));

  cl_int gpuCalcRes;
  if (alignedA != NULL && alignedB != NULL) {
    gpuCalcRes = libArtificialMultiplyMatricesGpu(mnk, alignedA, alignedB, gpuResult, runInfo.artificialFactorIterations);
  } else {
    gpuCalcRes = libArtificialMultiplyMatricesGpu(mnk, srcA, srcB, gpuResult, runInfo.artificialFactorIterations);
  }
  if (gpuCalcRes != CL_SUCCESS) {
    TargetResult result = {gpuCalcRes, 0, 0, 0};
    return result;
  }

  MatrixType* cpuResult = malloc(cSize * sizeof(MatrixType));

  ProfilingCPU cpuProfiling;

  CompareMatricesResult compareRes;

  if (libParam.ENABLE_CPU_PROFILING) {
    libMultiplyMatricesCpu(mnk, srcA, srcB, cpuResult);
    libGetCpuProfiling(&cpuProfiling);

    compareRes = compareMatrices(mnk, cpuResult, gpuResult, runInfo);
  }

  if (runInfo.enableLog) {
    printABC(srcA, srcB, gpuResult, cpuResult, mnk.m * mnk.k, mnk.n * mnk.k, cSize);
    printPerformance(cpuProfiling, mnk.m * mnk.n * mnk.k);
  }

  free(cpuResult);
  free(gpuResult);

  ProfilingGPU gpuProfiling;
  size_t trash;
  libGetGPUProfiling(&gpuProfiling);

  TargetResult result = {CL_SUCCESS, compareRes, cpuProfiling, gpuProfiling};
  return result;
}

void utFillData(MNK mnk, MatrixType* a, MatrixType* b) {
  for (size_t i = 0; i < mnk.m * mnk.k; i++) {
    a[i] = utGetRandomValue();
    // a[i] = 1;
  }
  for (size_t i = 0; i < mnk.n * mnk.k; i++) {
    b[i] = utGetRandomValue();
    // b[i] = 1;
  }
}

TargetResult utRunTest(size_t m, size_t n, size_t k, const char* name, RunInfo runInfo) {
  cl_int initRes = libInitializeContext();
  if (initRes != CL_SUCCESS) {
    printf("error libInitializeContext in test %s", name);
  }
  static size_t cnt = 0;
  if (cnt == 125) {
    cnt = 0;
  }
  MatrixType* a = malloc(sizeof(MatrixType) * m * k);
  MatrixType* b = malloc(sizeof(MatrixType) * k * n);
  MNK mnk = {m, n, k};
  utFillData(mnk, a, b);
  char buf[BUFF];
  snprintf(buf, sizeof(buf), "test_%zu", cnt++);
  const char* nameCalcuted = name == nullptr ? buf : name;
  TargetResult result = utTestImpl(nameCalcuted, mnk, a, b, runInfo, NULL, NULL);
  free(a);
  free(b);
  libDestroyContext();
  return result;
}

float ns2ms(size_t nanosec) {
  return (float) nanosec / 1000000.0f;
}

float ns2s(size_t nanosec) {
  return (float) nanosec / 1000000000.0f;
}

CompareMatricesResult compareMatrices(MNK mnk, MatrixType* lhsPtr, MatrixType* rhsPtr, RunInfo runInfo) {
  CompareMatricesResult result = {0, 0};
  size_t cSize = mnk.m * mnk.n;

  static const MatrixType REL_EPS = 0.01;
  static const MatrixType ABS_EPS = 0.01;
  // #pragma omp parallel
  for (size_t i = 0; i < cSize; i++) {
    // float diff = lhs[i] - rhs[i];
    MatrixType lhs = lhsPtr[i];
    MatrixType rhs = rhsPtr[i];
    float absDiff = fabs(lhs - rhs);
    float scale = fmaxf(fabsf(lhs), fabsf(rhs));
    float limit = ABS_EPS + REL_EPS * scale;

    float relativeDiff = fabs(lhsPtr[i] - rhsPtr[i]) / fmaxf(fabs(lhsPtr[i]), fabs(rhsPtr[i]));
    result.sumRelativeDiff += relativeDiff;
    result.maxRelativeDiff = relativeDiff > result.maxRelativeDiff ? relativeDiff : result.maxRelativeDiff;
    if (absDiff > limit) {
      result.missesAmount++;
    }
  }

  if (runInfo.enableLog) {
    // result.maxRelativeDiff = result.maxRelativeDiff / mnk.m / mnk.n;
    result.maxRelativeDiff = result.maxRelativeDiff;
    // printf("max relative difference = %f\n", result.maxRelativeDiff);
  }
  return result;
}

void printABC(
    const MatrixType* restrict a,
    const MatrixType* restrict b,
    const MatrixType* restrict c,
    const MatrixType* restrict cpuC,
    size_t aSize,
    size_t bSize,
    size_t cSize
) {
  printf("a.size = %zu\n", aSize);
  printf("b.size = %zu\n", bSize);
  if (aSize < 65 && bSize < 65) {
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

void printPerformance(ProfilingCPU profilingCpu, size_t amountOperations) {
  printf("===[[performance]]===\n");
  printf("amount operations = %zu\n", amountOperations);
  if (libParam.ENABLE_CPU_PROFILING) {
    printf("cpu:\n");
    printf("|  time = %llu ns (%f s)\n", profilingCpu.seconds, profilingCpu.seconds / pow(10, 9));
    printf("|  flops = %f TFps\n", profilingCpu.flops / (pow((long double) 10, (long double) 12)));
  }
  if (libParam.ENABLE_GPU_PROFILING) {
    ProfilingGPU profilingGpu;
    libGetGPUProfiling(&profilingGpu);
    printf("gpu:\n");
    printf("|  artificialFactor = %zu\n", profilingGpu.artificialFactor);
    printf(
        "|  time = %llu ns (%f s)\n",
        libGetEndSta(profilingGpu.calculating),
        libGetEndSta(profilingGpu.calculating) / pow(10, 9)
    );
    printf("|  flops = %f TFps \n", libGetFlops(profilingGpu) / (pow((long double) 10, (long double) 12)));
  }
}

void printGPUTimes(const char* name, ProfTimes times) {
  printf("time %s sub-que = %f\n", name, ns2s(libGetSubQue(times)));
  printf("time %s sta-sub = %f\n", name, ns2s(libGetStaSub(times)));
  printf("time %s end-sta = %f\n", name, ns2s(libGetEndSta(times)));
}

#define SABAKA_BUFFER_SIZE 8192

int compare_binary_files(const char* path1, const char* path2, uintmax_t* first_diff_offset) {
  FILE* file1 = fopen(path1, "rb");
  if (file1 == NULL) {
    return -1;
  }

  FILE* file2 = fopen(path2, "rb");
  if (file2 == NULL) {
    int saved_errno = errno;
    fclose(file1);
    errno = saved_errno;
    return -1;
  }

  unsigned char buffer1[SABAKA_BUFFER_SIZE];
  unsigned char buffer2[SABAKA_BUFFER_SIZE];

  uintmax_t offset = 0;

  while (1) {
    size_t read1 = fread(buffer1, 1, sizeof(buffer1), file1);
    size_t read2 = fread(buffer2, 1, sizeof(buffer2), file2);

    if (ferror(file1) || ferror(file2)) {
      int saved_errno = errno;
      fclose(file1);
      fclose(file2);
      errno = saved_errno;
      return -1;
    }

    size_t min_read = read1 < read2 ? read1 : read2;

    if (memcmp(buffer1, buffer2, min_read) != 0) {
      for (size_t i = 0; i < min_read; ++i) {
        if (buffer1[i] != buffer2[i]) {
          if (first_diff_offset != NULL) {
            *first_diff_offset = offset + i;
          }

          fclose(file1);
          fclose(file2);
          return 1;
        }
      }
    }

    if (read1 != read2) {
      if (first_diff_offset != NULL) {
        *first_diff_offset = offset + min_read;
      }

      fclose(file1);
      fclose(file2);
      return 1;
    }

    if (read1 == 0) {
      break;
    }

    offset += read1;
  }

  if (fclose(file1) != 0) {
    int saved_errno = errno;
    fclose(file2);
    errno = saved_errno;
    return -1;
  }

  if (fclose(file2) != 0) {
    return -1;
  }

  return 0;
}
