#include "all-tests.h"
#include "csv-printer.h"
#include "data-structures.h"
#include "multiply-matrices.h"
#include "test-utils.h"

static size_t idx(size_t i, size_t j, size_t n) {
  return i * n + j;
}

void test_cpu_transpose_correctness_performance() {
  libParam.ENABLE_CPU_PROFILING = true;
  libParam.ENABLE_CPU_TRANSPOSE = true;

  const char* DIR_CORRECTNESS = "cpu-correctness-transpose/";
  const char* DIR_PERFORMANCE = "cpu-performance-transpose/";
  constexpr size_t SIZE_SIZES = 1;
  constexpr size_t SIZE_RANGE = 1;
  size_t sizes[] = {4};
  size_t randRange[] = {50};

  for (size_t sizeIdx = 0; sizeIdx < SIZE_SIZES; sizeIdx++) {
    char pathCorrectness[BUFF];
    snprintf(pathCorrectness, sizeof(pathCorrectness), "%s%zu", DIR_CORRECTNESS, sizes[sizeIdx]);
    char pathPerformance[BUFF];
    snprintf(pathPerformance, sizeof(pathPerformance), "%s%zu", DIR_PERFORMANCE, sizes[sizeIdx]);

    csvSetPath(pathCorrectness);
    csvClearFile();
    csvAppendLine("range,delta");

    csvSetPath(pathPerformance);
    csvClearFile();
    csvAppendLine("range,time_simple,time_transpose");

    for (size_t rangeIdx = 0; rangeIdx < SIZE_RANGE; rangeIdx++) {
      // set parameters
      size_t size = sizes[sizeIdx];
      randomFactor = randRange[rangeIdx];
      randomShift = randomFactor - randomFactor / 2;
      printf("size = %zu; random range = %zu\n", size, randRange[rangeIdx]);

      // fill data
      size_t m = size;
      size_t n = size;
      size_t k = size;
      RunInfo runInfo = {true, 1};

      MatrixType* a = malloc(sizeof(MatrixType) * m * k);
      MatrixType* b = malloc(sizeof(MatrixType) * k * n);
      MNK mnk = {m, n, k};
      utFillData(mnk, a, b);

      size_t cSize = m * n;
      MatrixType* cpuBasic = malloc(cSize * sizeof(MatrixType));
      MatrixType* cpuTranspose = malloc(cSize * sizeof(MatrixType));

      cl_float trash;
      cl_ulong basicCalcTime;
      cl_ulong transCalcTime;

      ProfilingCPU infoBasic;
      ProfilingCPU infoTrans;

      // calculate
      libParam.ENABLE_CPU_TRANSPOSE = false;
      libMultiplyMatricesCpu(mnk, a, b, cpuBasic);
      libGetCpuProfiling(&infoBasic);
      libParam.ENABLE_CPU_TRANSPOSE = true;
      libMultiplyMatricesCpu(mnk, a, b, cpuTranspose);
      libGetCpuProfiling(&infoTrans);

      for (size_t i = 0; i < mnk.m; i++) {
        for (size_t j = 0; j < mnk.n; j++) {
          printf("%f    ", cpuBasic[idx(i, j, mnk.n)]);
        }
        printf("\n");
      }

      printf("\n");

      for (size_t i = 0; i < mnk.m; i++) {
        for (size_t j = 0; j < mnk.n; j++) {
          printf("%f    ", cpuTranspose[idx(i, j, mnk.n)]);
        }
        printf("\n");
      }

      size_t misses;
      CompareMatricesResult compareRes = compareMatrices(mnk, cpuBasic, cpuTranspose, runInfo);

      free(cpuTranspose);
      free(cpuBasic);
      free(b);
      free(a);

      csvSetPath(pathCorrectness);
      csvAppendFloat(randRange[rangeIdx]);
      csvAppendFloat(compareRes.maxRelativeDiff);
      csvNewLine();

      csvSetPath(pathPerformance);
      csvAppendFloat(randRange[rangeIdx]);
      csvAppendFloat(ns2s(infoBasic.seconds));
      csvAppendFloat(ns2s(infoTrans.seconds));
      csvNewLine();
    }
  }
}
