#include "all-tests.h"
#include "csv-printer.h"
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

static cl_ulong helperGetTime() {
  cl_ulong nanosec = (long long) (omp_get_wtime() * 1e9);
  return nanosec; // ns
}

void test_artificial_factor_iterations() {
  libParam.ENABLE_CPU_PROFILING = true;
  libParam.CALCULATING_MODE = V_1;
  libParam.ENABLE_GPU_LOGG = false;
  size_t mSize = 2000;
  size_t m, n, k;
  m = n = k = mSize;

  csvSetPath("gpu-performance-artificial-factor/times_1k_2k");
  csvClearFile();
  csvAppendLine("artificial_factor,time_await_kernel,raw_calculating_end_start");

  for (size_t artificialFactor = 1000; artificialFactor < 2000 + 1; artificialFactor++) {
    RunInfo runInfo = {true, artificialFactor};

    cl_ulong start = helperGetTime();
    TargetResult profiling = utRunTest(m, n, k, "artificial_factor_iterations", runInfo);
    cl_ulong end = helperGetTime();
    cl_ulong globalTime = end - start;

    printGPUTimes("host->dev", profiling.gpu.transportToDev);
    printGPUTimes("calculating", profiling.gpu.calculating);
    printGPUTimes("dev->host", profiling.gpu.transportFromDev);
    printf("time await kernel %f\n", ns2s(profiling.gpu.timeWaitingKernel));

    printf("time global = %f\n", ns2s(globalTime));

    csvAppendFloat(artificialFactor);
    csvAppendFloat(ns2s(profiling.gpu.timeWaitingKernel));
    csvAppendFloat(ns2s(libGetEndSta(profiling.gpu.calculating)));
    csvNewLine();
  }
}
