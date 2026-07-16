#include "all-tests.h"
#include "csv-printer.h"
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
#include <string.h>

// AMD:
// max frequency (from developer) = 1755 MHz
// max work group size = 256
// local mem size = 65536 bytes (64 Kb) => tile size = 16384 cl_floats

// константы из ci
const size_t MAX_MEM = 16384;

const size_t M_SIZE = 1536;
const size_t N_SIZE = 8192;
const size_t K_SIZE = 2048;

const size_t DIMS_LOW = 2000;
const size_t DIMS_HIGH = 2001;

// перебор
// static const size_t LOC_W_ARR[] = {16, 32};
// static const size_t LOC_H_ARR[] = {2, 4, 8};
// static const size_t TS_K_ARR[] = {4, 8, 12, 16};
// static const size_t TR_W_ARR[] = {8, 12, 16, 20};
// static const size_t TR_H_ARR[] = {2, 4, 6, 8};
// static const size_t VECTOR_ARR[] = {2, 4};

// ideal AMD = 16x4
static const size_t LOC_W_ARR[] = {16};
static const size_t LOC_H_ARR[] = {8};
static const size_t TS_K_ARR[] = {16};
static const size_t TR_W_ARR[] = {20};
static const size_t TR_H_ARR[] = {8};
static const size_t VECTOR_ARR[] = {4};
static const size_t VECTOR_ART[] = {90,100,110,120,130,140,150};

// ideal NVIDIA = 16x4
// static const size_t LOC_W_ARR[] = {16, 32};
// static const size_t LOC_H_ARR[] = {2, 4, 8};
// static const size_t TS_K_ARR[] = {4,12};
// static const size_t TR_W_ARR[] = {12, 16};
// static const size_t TR_H_ARR[] = {24, 12, 6};
// static const size_t VECTOR_ARR[] = {4};

// single
// static const size_t LOC_W_ARR[] = {32};
// static const size_t LOC_H_ARR[] = {8};
// static const size_t TS_K_ARR[] = {8};
// static const size_t TR_W_ARR[] = {8};
// static const size_t TR_H_ARR[] = {12,13,14,15,16,17,18,19,20};
// static const size_t VECTOR_ARR[] = {8};

const size_t TS_W_SIZE = sizeof(LOC_W_ARR) / sizeof(size_t);

const size_t TS_H_SIZE = sizeof(LOC_H_ARR) / sizeof(size_t);

const size_t TS_K_SIZE = sizeof(TS_K_ARR) / sizeof(size_t);

const size_t TR_W_SIZE = sizeof(TR_W_ARR) / sizeof(size_t);

const size_t TR_H_SIZE = sizeof(TR_H_ARR) / sizeof(size_t);

const size_t VECTOR_SIZE = sizeof(VECTOR_ARR) / sizeof(size_t);

const size_t VECTOR_ART_SIZE = sizeof(VECTOR_ART) / sizeof(size_t);

// рассматриваемые реализации
static const GPUCalculatingMode REALIZATION_2_OBSERVE[] = {V_2};

static const size_t NUM_REALIZATIONS = sizeof(REALIZATION_2_OBSERVE) / sizeof(GPUCalculatingMode);

// код
static size_t potolok(size_t mSize, size_t tileSize) {
  return mSize / tileSize + (mSize % tileSize != 0);
}

static _Bool checkValidationMaxWorkGroupSize(size_t localX, size_t localY, size_t maxWorkGroupSize) {
  return localX * localY <= maxWorkGroupSize;
}

static _Bool checkValidationMaxMem(size_t tileH, size_t tsk, size_t tileW) {
  return tsk * (tileH + tileW) <= MAX_MEM;
}

static _Bool checkValidationVector(size_t nSize, size_t tsw, size_t tsk, size_t trw) {
  return tsw % nSize == 0 && tsk % nSize == 0 && trw % nSize == 0;
}

static _Bool checkTR(size_t ts, size_t tr) {
  return tr < ts;
}

static _Bool checkWorkGroup(size_t localW, size_t localH, size_t maxWorkGroupSize) {
  return localW * localH >= maxWorkGroupSize * 3 / 4;
}

static _Bool
checkGuaranteeCompromise(size_t tsw, size_t tsh, size_t tsk, size_t trw, size_t trh, size_t localX, size_t localY) {
  return tsw % trw == 0 && tsh % trh == 0;
}

static size_t defineWinner(cl_float* arr, size_t size) {
  size_t res = 0;
  for (size_t i = 1; i < size; i++) {
    if (arr[i] < arr[res]) {
      res = i;
    }
  }
  return res;
}

static size_t closestUp(size_t a, size_t b) {
  return a / b * b + (a % b != 0 ? 1 : 0);
}

void test_stress_v2() {
  libParam.TEST_IGNORE = true;

  const char* MAIN_PATH = "gpu-perf-v2";

  libIni.deviceIndex = 0;
  libParam.CALCULATING_MODE = V_2;
  libParam.NOUNROLL = false;

  libParam.ENABLE_CPU_PROFILING = false;

  libParam.ENABLE_GPU_LOGG = false;
  libParam.ENABLE_CPU_TRANSPOSE = true;

  csvSetPath(MAIN_PATH);
  csvClearFile();

  size_t maxWorkGropuSize = libGetMaxWorkGroupSize();

  size_t numAll = 0;
  size_t numSkippedEfficient = 0;
  size_t numSkippedValidation = 0;

  csvSetPath(MAIN_PATH);
  if (libParam.ENABLE_CPU_PROFILING) {
    csvAppendLine(
        "localW,localH,local-size,tsk,trw,trh,tsw,tsh,n-size,realization,art-factor,nounroll,whole-time,"
        "gpu-raw-time,"
        "flops,cpu-time,sum-relative-diff,max-relative-diff"
    );
  } else {
    csvAppendLine(
        "localW,localH,local-size,tsk,trw,trh,tsw,tsh,n-size,realization,"
        "art-factor,nounroll,whole-time,gpu-raw-time,flops"
    );
  }

  GPUCalculatingMode globalWinners[LONG_BUFF];

  RunInfo runInfo = {false, 1};
  for (size_t m = M_SIZE; m < M_SIZE + 1; m++) {
    for (size_t n = N_SIZE; n < N_SIZE + 1; n++) {
      for (size_t k = K_SIZE; k < K_SIZE + 1; k++) {
        MatrixType* aMat = malloc(sizeof(MatrixType) * m * k);
        MatrixType* bMat = malloc(sizeof(MatrixType) * k * n);
        MNK mnk = {m, n, k};
        utFillData(mnk, aMat, bMat);

        // optimization for less alloc and copy matrices in libCalcGpu
        size_t alignedN = closestUp(n, 16);
        size_t alignedK = closestUp(k, 16);
        MatrixType* aAligned = malloc(sizeof(MatrixType) * m * alignedK);
        MatrixType* bAligned = malloc(sizeof(MatrixType) * alignedK * alignedN);
        for (size_t row = 0; row < mnk.m; row++) {
          memcpy(aAligned + row * alignedK, aMat + row * mnk.k, mnk.k * sizeof(MatrixType));
        }
        for (size_t row = 0; row < mnk.k; row++) {
          memcpy(bAligned + row * alignedN, bMat + row * mnk.n, mnk.n * sizeof(MatrixType));
        }

        for (size_t localWIdx = 0; localWIdx < TS_W_SIZE; localWIdx++) {
          size_t localW = LOC_W_ARR[localWIdx];
          for (size_t localHIdx = 0; localHIdx < TS_H_SIZE; localHIdx++) {
            size_t localH = LOC_H_ARR[localHIdx];
            for (size_t tskIdx = 0; tskIdx < TS_K_SIZE; tskIdx++) {
              size_t tsk = TS_K_ARR[tskIdx];
              for (size_t trwIdx = 0; trwIdx < TR_W_SIZE; trwIdx++) {
                size_t trw = TR_W_ARR[trwIdx];
                for (size_t trhIdx = 0; trhIdx < TR_H_SIZE; trhIdx++) {
                  size_t trh = TR_H_ARR[trhIdx];
                  for (size_t nSizeIdx = 0; nSizeIdx < VECTOR_SIZE; nSizeIdx++) {
                    size_t nSize = VECTOR_ARR[nSizeIdx];
                    for (size_t artFactorIdx = 0; artFactorIdx < VECTOR_ART_SIZE; artFactorIdx++) {
                      size_t artFactor = VECTOR_ART[artFactorIdx];

                      printf("localW = %zu, localH = %zu, tsk = %zu, trw = %zu, ", localW, localH, tsk, trw);
                      printf("trh = %zu, nSize = %zu, artFactor = %zu\n", trh, nSize, artFactor);

                      size_t tileW = localW * trw;
                      size_t tileH = localH * trh;
                      size_t localX = localW;
                      size_t localY = localH;
                      printf("localX = %zu\n", localX);
                      printf("localY = %zu\n", localY);

                      if (!checkValidationMaxWorkGroupSize(localX, localY, maxWorkGropuSize)) {
                        printf("[skipped] [validation] local group > max group size\n");
                        numSkippedValidation++;
                        continue;
                      }

                      if (!checkValidationMaxMem(tileH, tsk, tileW)) {
                        printf("[skipped] [validation] local group size > local mem size\n");
                        numSkippedValidation++;
                        continue;
                      }

                      libParam.v2Choosen.TILE_SIZE_W = tileW;
                      libParam.v2Choosen.TILE_SIZE_H = tileH;
                      libParam.v2Choosen.TILE_SIZE_K = tsk;
                      libParam.v2Choosen.THREAD_RANGE_W = trw;
                      libParam.v2Choosen.THREAD_RANGE_H = trh;
                      libParam.v2Choosen.N_SIZE = nSize;

                      cl_float localResults[NUM_REALIZATIONS];

                      _Bool noNextLine = false;

                      for (size_t realization = 0; realization < NUM_REALIZATIONS; realization++) {
                        MatrixType* sendAlignedA = NULL;
                        MatrixType* sendAlignedB = NULL;
                        if (REALIZATION_2_OBSERVE[realization] == V_2) {
                          if (!checkValidationVector(nSize, tileW, tsk, trw)) {
                            printf("[skipped] [validation] vector\n");
                            numSkippedValidation++;
                            noNextLine = true;
                            continue;
                          }
                        }

                        libParam.CALCULATING_MODE = REALIZATION_2_OBSERVE[realization];

                        cl_int resInit = libInitializeContext();
                        if (resInit != CL_SUCCESS) {
                          printf("[skipped] unhandled exception lib initialize\n");
                          noNextLine = true;
                          numSkippedValidation++;
                          continue;
                        }

                        RunInfo runInfo = {false, artFactor};
                        TargetResult result = utTestImpl("YAC", mnk, aMat, bMat, runInfo, sendAlignedA, sendAlignedB);
                        libDestroyContext();

                        if (result.errorCode != CL_SUCCESS) {
                          printf("[skipped] unhandled exception lib run\n");
                          noNextLine = true;
                          numSkippedValidation++;
                          continue;
                        }

                        printf("missedsAmount = %zu\n", result.diff.missesAmount);
                        printf("max relative diff = %f\n", result.diff.maxRelativeDiff);
                        if (libParam.ENABLE_CPU_PROFILING) {
                          if (result.diff.missesAmount != 0) {
                            printf("err: misses != 0\n");
                            exit(1);
                          }
                        }
                        csvSetPath(MAIN_PATH);
                        csvAppendFloat(localW);
                        csvAppendFloat(localH);
                        csvAppendFloat(localW * localH);
                        csvAppendFloat(tsk);
                        csvAppendFloat(trw);
                        csvAppendFloat(trh);
                        csvAppendFloat(tileW);
                        csvAppendFloat(tileH);
                        csvAppendFloat(nSize);
                        csvAppendFloat(REALIZATION_2_OBSERVE[realization]);
                        csvAppendFloat(artFactor);
                        csvAppendFloat(libParam.NOUNROLL);
                        csvAppendFloat(ns2ms(result.gpu.timeWaitingKernel));
                        csvAppendFloat(ns2ms(libGetEndSta(result.gpu.calculating)));
                        csvAppendFloat(
                            1.0 * m * n * k * runInfo.artificialFactorIterations /
                            libGetEndSta(result.gpu.calculating) / 1000
                        );

                        if (libParam.ENABLE_CPU_PROFILING) {
                          csvAppendFloat(ns2ms(result.cpu.seconds));
                          csvAppendFloat(result.diff.sumRelativeDiff);
                          csvAppendFloat(result.diff.maxRelativeDiff);
                        }
                        csvNewLine();
                        localResults[realization] = ns2ms(libGetEndSta(result.gpu.calculating));
                      }

                      if (!noNextLine) {
                        csvNewLine();
                        size_t winnerIndex = defineWinner(localResults, NUM_REALIZATIONS);
                        GPUCalculatingMode winnerMode = REALIZATION_2_OBSERVE[winnerIndex];
                        globalWinners[numAll] = winnerMode;
                        numAll++;
                      }
                    }
                  }
                }
              }
            }
          }
        }
        free(bMat);
        free(aMat);
      }
    }

    printf("num all = %zu\n", numAll);
    printf("num skipped cause efficient = %zu\n", numSkippedEfficient);
    printf("num skipped cause validation = %zu\n", numSkippedValidation);
    printf("success correctness test v2\n");
    printf("winners:\n");
    for (size_t i = 0; i < numAll; i++) {
      printf("%i ", globalWinners[i]);
    }
    printf("\n");
  }
}
