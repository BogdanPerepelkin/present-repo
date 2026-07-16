#include "all-tests.h"
#include "data-structures.h"
#include "multiply-matrices.h"
#include "test-utils.h"
#include "utils.h"

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include <assert.h>
#include <omp.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RELEASE 0

void parametersLibInitializing() {
  // RANDOM
  srand((unsigned) time(NULL));

  // GLOBAL_VARIABLES_GPU
  libParam.DEVICE_INDEX = 0;

  // V_1
  libParam.SHORT_TILE_SIZE = 16;
  libParam.SHORT_THREAD_RANGE = 16;

  // V_2
  // 1 версия - группа 64
  // libParam.v2Amd.TILE_SIZE_W = 192;
  // libParam.v2Amd.TILE_SIZE_H = 40;
  // libParam.v2Amd.TILE_SIZE_K = 12;
  // libParam.v2Amd.THREAD_RANGE_W = 12;
  // libParam.v2Amd.THREAD_RANGE_H = 10;
  // libParam.v2Amd.N_SIZE = 4;
  
  // 2 версия - группа 128 (64 x 2)
  libParam.v2Amd.TILE_SIZE_W = 320;
  libParam.v2Amd.TILE_SIZE_H = 64;
  libParam.v2Amd.TILE_SIZE_K = 16;
  libParam.v2Amd.THREAD_RANGE_W = 20;
  libParam.v2Amd.THREAD_RANGE_H = 8;
  libParam.v2Amd.N_SIZE = 4;

  libParam.v2Nvidia.TILE_SIZE_W = 256;
  libParam.v2Nvidia.TILE_SIZE_H = 48;
  libParam.v2Nvidia.TILE_SIZE_K = 4;
  libParam.v2Nvidia.THREAD_RANGE_W = 16;
  libParam.v2Nvidia.THREAD_RANGE_H = 12;
  libParam.v2Nvidia.N_SIZE = 4;

  libParam.v2IntelIngegra.TILE_SIZE_W = 64;
  libParam.v2IntelIngegra.TILE_SIZE_H = 24;
  libParam.v2IntelIngegra.TILE_SIZE_K = 4;
  libParam.v2IntelIngegra.THREAD_RANGE_W = 8;
  libParam.v2IntelIngegra.THREAD_RANGE_H = 6;
  libParam.v2IntelIngegra.N_SIZE = 2;

  libParam.v2IntelCpu.TILE_SIZE_W = 128;
  libParam.v2IntelCpu.TILE_SIZE_H = 40;
  libParam.v2IntelCpu.TILE_SIZE_K = 4;
  libParam.v2IntelCpu.THREAD_RANGE_W = 16;
  libParam.v2IntelCpu.THREAD_RANGE_H = 10;
  libParam.v2IntelCpu.N_SIZE = 4;

  // Optimization
  libParam.NOUNROLL = false;

  // Logg
  libParam.ENABLE_GPU_PROFILING = true;
  libParam.ENABLE_GPU_LOGG = false;

  // Mode
  libParam.CALCULATING_MODE = V_2;
  libParam.TEST_IGNORE = false;

  // GLOBAL_VARIABLES_CPU
  libParam.ENABLE_CPU_PROFILING = false;
  libParam.ENABLE_CPU_TRANSPOSE = false;
}

void myBusinesLogic() {
  printf("===================&& hello from main &&===================\n");
  parametersLibInitializing();

  libIni.deviceType = DISCRETE_GPU;
  libIni.deviceIndex = 0;

  // INFO
  libInitializeContext();
  printf("max frequency (from developer) = %u MHz\n", libGetMaxFrequency());
  printf("max work group size = %zu\n", libGetMaxWorkGroupSize());
  printf("local mem size = %llu\n", libGetLocalMemSize());
  libDestroyContext();

  // TESTS
  test_stress_v2();
}

cl_int checkGenericInfo(int argc, char* argv[], size_t i, size_t* futureI) {
  if (equals(argv[i], "--help")) {
    logg(
        "lab0.exe < --input file_name > \n < --output file_name > \n [ --device-type { dgpu | igpu | gpu | cpu | all } "
        "] \n [ --device-index index ]"
    );
    return 1;
  }

  _Bool isNotFoundAny = false;

  if (equals(argv[i], "--input")) {
    if (i + 1 < argc) {
      files.input = argv[i + 1];
    } else {
      return releaseContext(1, "expected value after --input", "");
    }
  }

  else if (equals(argv[i], "--output")) {
    if (i + 1 < argc) {
      files.output = argv[i + 1];
    } else {
      return releaseContext(1, "expected value after --output", "");
    }
  }

  else if (equals(argv[i], "--device-type")) {
    DeviceType deviceType = DISCRETE_GPU;
    if (i + 1 < argc) {
      if (equals(argv[i + 1], "dgpu")) {
        deviceType = DISCRETE_GPU;
      } else if (equals(argv[i + 1], "igpu")) {
        deviceType = INTEGRATED_GPU;
      } else if (equals(argv[i + 1], "gpu")) {
        deviceType = DISCRETE_GPU;
      } else if (equals(argv[i + 1], "cpu")) {
        deviceType = CPU;
      } else if (equals(argv[i + 1], "all")) {
        deviceType = DISCRETE_GPU;
      } else {
        return releaseContext(1, "wrong device-type", "");
      }
    } else {
      return releaseContext(1, "expected value after --device-type key", "");
    }
    libIni.deviceType = deviceType;
  }

  else if (equals(argv[i], "--device-index")) {
    size_t deviceIndex = 0;
    if (i + 1 < argc) {
      deviceIndex = atoi(argv[i + 1]);
    } else {
      return releaseContext(1, "expected value after --device-index key", "");
    }
    libIni.deviceIndex = deviceIndex;
  } else {
    isNotFoundAny = true;
  }

  if (!isNotFoundAny) {
    *futureI = i + 1;
  }
  return CL_SUCCESS;
}

cl_int checkSpecializeInfo(int argc, char* argv[], size_t i, size_t* futureI) {
  _Bool isNotFoundAny = false;

  if (equals(argv[i], "--verify")) {
    libParam.ENABLE_CPU_PROFILING = true;
  } else if (equals(argv[i], "--realization")) {
    if (i + 1 < argc) {
      GPUCalculatingMode mode = atoi(argv[i + 1]);
      // todo увеличить когда разрастется
      if (mode > 2 || mode < 0) {
        return releaseContext(1, "gpu calculating mode not in [0,2]", "");
      }
      libParam.CALCULATING_MODE = mode;
    } else {
      return releaseContext(1, "expected value after --device-index key", "");
    }
  } else {
    isNotFoundAny = true;
  }

  if (!isNotFoundAny) {
    *futureI = i + 1;
  }
  return CL_SUCCESS;
}

cl_int processInitializing(int argc, char* argv[]) {
  parametersLibInitializing();

  for (size_t i = 0; i < argc; i++) {
    cl_int resGeneric = checkGenericInfo(argc, argv, i, &i);
    if (resGeneric != CL_SUCCESS) {
      return resGeneric;
    }

    cl_int resSpecial = checkSpecializeInfo(argc, argv, i, &i);
    if (resSpecial != CL_SUCCESS) {
      return resSpecial;
    }
  }

  return CL_SUCCESS;
}

cl_int readInputFile(SourceMatrices* dst) {
  FILE* inputFile = fopen(files.input, "rb");
  if (inputFile == NULL) {
    return releaseContext(1, "cannot open input file", files.input);
  }

  fread(&dst->mnk.n, sizeof(dst->mnk.n), 1, inputFile);
  fread(&dst->mnk.k, sizeof(dst->mnk.k), 1, inputFile);
  fread(&dst->mnk.m, sizeof(dst->mnk.m), 1, inputFile);

  dst->a = malloc(dst->mnk.m * dst->mnk.k * sizeof(MatrixType));
  if (dst->a == NULL) {
    return releaseContext(1, "malloc read matrices", "");
  }
  fread(dst->a, sizeof(MatrixType), dst->mnk.m * dst->mnk.k, inputFile);

  dst->b = malloc(dst->mnk.k * dst->mnk.n * sizeof(MatrixType));
  if (dst->b == NULL) {
    return releaseContext(1, "malloc read matrices", "");
  }
  fread(dst->b, sizeof(MatrixType), dst->mnk.k * dst->mnk.n, inputFile);

  fclose(inputFile);

  if (ferror(inputFile) || feof(inputFile)) {
    return releaseContext(1, "error reading from file", files.output);
  }

  return CL_SUCCESS;
}

cl_int processCalculating(SourceMatrices* src) {
  src->cGraphic = malloc(src->mnk.m * src->mnk.n * sizeof(MatrixType));
  if (src->cGraphic == NULL) {
    return releaseContext(1, "malloc resultin matrix", "");
  }
  src->cCentral = malloc(src->mnk.m * src->mnk.n * sizeof(MatrixType));
  if (src->cCentral == NULL) {
    return releaseContext(1, "malloc resultin matrix", "");
  }

  cl_int resMulGpu = libMultiplyMatricesGpu(src->mnk, src->a, src->b, src->cGraphic);
  if (resMulGpu != CL_SUCCESS) {
    return releaseContext(1, "error on gpu calculating", "");
  }

  if (libParam.ENABLE_CPU_PROFILING) {
    ProfilingCPU profilingCpu;

    cl_int resMulCpu = libMultiplyMatricesCpu(src->mnk, src->a, src->b, src->cCentral);
    if (resMulCpu != CL_SUCCESS) {
      return releaseContext(1, "error on cpu calculating", "");
    }
    RunInfo runInfo = {false, 1};
    CompareMatricesResult compareResult = compareMatrices(src->mnk, src->cCentral, src->cGraphic, runInfo);
    printf("compare result: misses = %zu\n", compareResult.missesAmount);
    printf("compare result: summ relative diff = %f\n", compareResult.sumRelativeDiff);
    printf("compare result: max relative diff = %f\n", compareResult.maxRelativeDiff);

    if (compareResult.missesAmount != 0) {
      // return releaseContext(1, "discovered diff between cpu and gpu results", "");
    }
  }

  if (libParam.ENABLE_GPU_LOGG) {
    ProfilingGPU profilingGpu;
    libGetGPUProfiling(&profilingGpu);
    printf("time gpu = %f\n", ns2ms(libGetEndSta(profilingGpu.calculating)));
  }

  return CL_SUCCESS;
}

inline static size_t potolok(const size_t a, const size_t b) {
  return a / b + (a % b != 0 ? 1 : 0);
}

cl_int printInfo() {
  const char* nameDevice = libGetChoosenDeviceName();
  const char* platformName = libGetChoosenPlatformName();
  ProfilingGPU profilingGpu;
  libGetGPUProfiling(&profilingGpu);
  cl_float rawCalcTime = libGetEndSta(profilingGpu.calculating);
  cl_float fullTime = libGetRealyFullTime(profilingGpu.transportToDev) + libGetRealyFullTime(profilingGpu.calculating) +
                      libGetRealyFullTime(profilingGpu.transportFromDev);

  printf("Device: %s\tPlatform: %s", nameDevice, platformName);
  printf("Time: %g\t", ns2ms(rawCalcTime));
  if (libParam.ENABLE_CPU_PROFILING) {
    printf("%g", ns2ms(fullTime));
  }
  printf("\n");
  printf(
      "BLOCK_THREAD_SIZE [%zu,    %zu]\n",
      potolok(libParam.v2Choosen.TILE_SIZE_W, libParam.v2Choosen.THREAD_RANGE_W),
      potolok(libParam.v2Choosen.TILE_SIZE_H, libParam.v2Choosen.THREAD_RANGE_H)
  );
  printf("THREAD_WORK_SIZE [%zu,    %zu]\n", libParam.v2Choosen.THREAD_RANGE_W, libParam.v2Choosen.THREAD_RANGE_H);

  return CL_SUCCESS;
}

cl_int writeOutputFile() {
  FILE* outputFile = fopen(files.output, "wb");
  if (outputFile == NULL) {
    return releaseContext(1, "cannot open output file", files.output);
  }

  fwrite(&sourceMatrices.mnk.n, sizeof(sourceMatrices.mnk.n), 1, outputFile);
  fwrite(&sourceMatrices.mnk.m, sizeof(sourceMatrices.mnk.m), 1, outputFile);
  fwrite(sourceMatrices.cGraphic, sizeof(MatrixType), sourceMatrices.mnk.m * sourceMatrices.mnk.n, outputFile);

  fclose(outputFile);

  if (ferror(outputFile) || feof(outputFile)) {
    return releaseContext(1, "error writing to file", files.output);
  }

  return CL_SUCCESS;
}

int main(int argc, char* argv[]) {
#if RELEASE == 1
  if (argc < 2) {
    return releaseContext(1, "use --help to start", "");
  }

  cl_int resParseInput = processInitializing(argc, argv);
  if (resParseInput != CL_SUCCESS) {
    return releaseContext(resParseInput, "parseInput", "");
  }

  cl_int resRead = readInputFile(&sourceMatrices);
  if (resRead != CL_SUCCESS) {
    return resRead;
  }

  cl_int resLibInit = libInitializeContext();
  if (resLibInit != CL_SUCCESS) {
    return releaseContext(resLibInit, "unsuccess lib init", "");
  }

  cl_int resCalc = processCalculating(&sourceMatrices);
  if (resCalc != CL_SUCCESS) {
    return resCalc;
  }

  printInfo();

  cl_int resWrite = writeOutputFile();
  if (resWrite != CL_SUCCESS) {
    return resWrite;
  }

  libDestroyContext();

#else
  myBusinesLogic();
#endif

  return releaseContext(CL_SUCCESS, "success", "");
}
