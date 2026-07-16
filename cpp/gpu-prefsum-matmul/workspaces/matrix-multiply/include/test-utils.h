#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "data-structures.h"

extern float randomFactor;
extern float randomShift;

typedef struct {
  int enableLog;
  size_t artificialFactorIterations;
} RunInfo;

typedef struct {
  size_t missesAmount;
  MatrixType maxRelativeDiff;
  MatrixType sumRelativeDiff;
} CompareMatricesResult;

typedef struct {
  cl_int errorCode;
  CompareMatricesResult diff;
  ProfilingCPU cpu;
  ProfilingGPU gpu;
} TargetResult;

TargetResult
utTestImpl(const char* name, MNK mnk, MatrixType* srcA, MatrixType* srcB, RunInfo runInfo, MatrixType* alignedA, MatrixType* alignedB);

float utGetRandomValue();

void utFillData(MNK mnk, MatrixType* a, MatrixType* b);

TargetResult utRunTest(size_t m, size_t n, size_t k, const char* name, RunInfo runInfo);

float ns2ms(size_t nanosec);

float ns2s(size_t nanosec);

CompareMatricesResult compareMatrices(MNK mnk, MatrixType* cpuResult, MatrixType* gpuResult, RunInfo runInfo);

void printABC(
    const MatrixType* restrict a,
    const MatrixType* restrict b,
    const MatrixType* restrict c,
    const MatrixType* restrict cpuC,
    size_t aSize,
    size_t bSize,
    size_t cSize
);

void printPerformance(ProfilingCPU profiling, size_t amountOperations);

void printGPUTimes(const char* name, ProfTimes times);

int compare_binary_files(
    const char *path1,
    const char *path2,
    uintmax_t *first_diff_offset
);

#endif /* TEST_UTILS_H */
