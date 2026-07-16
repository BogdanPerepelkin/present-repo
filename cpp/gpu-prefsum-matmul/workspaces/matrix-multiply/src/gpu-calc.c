#include "data-structures.h"
#include "lib-utils.h"
#include "multiply-matrices.h"

#ifdef __APPLE__
#include <OpenCL/cl.h>
#include <OpenCL/cl_platform.h>
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#include <CL/cl_platform.h>
#include <CL/opencl.h>
#endif
#include <math.h>
#include <omp.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Пояснения для меня:
(нет блокирующих вызовов)
libInitialize:
  программа (просто код, не привязан к очереди)
  очередь
(есть блокирующие вызовы)
libMultiply:
  буферы
  kernel (задает связь рпограммы и аргументов)
  транспортировка
  положит в очередь kernel (задает связь рабочей функции и места ее исполнения)
*/

UtLibContext ctx = {0};
GyperParametersLibrary libParam = {0};
LibInitializeParameters libIni = {0};

typedef enum {
  AMD,
  NVIDIA,
  INTEL_INTEGRA,
  INTEL_CPU,
} ThisLabDevices;

cl_int libInitializeContext() {
  libUtLogg("===[[context initialization]]===");

  cl_int resGet = libUtReadPlatformDeviceInfo(&ctx);
  if (resGet != CL_SUCCESS) {
    return resGet;
  }

  libUtProcGetDeviceInfo(&ctx);

  // print
  libPrintDevInfo(&ctx);

  cl_int resCreateAll = libUtcreateAllDevs();
  if (resCreateAll != CL_SUCCESS) {
    return resCreateAll;
  }

  libUtfillPriorityMap();

  qsort(ctx.allDevices.devicesPtrs, ctx.allDevices.size, sizeof(UtDevice*), libUtcompareAllDevices);

  ctx.allDevices.choosenDevs.size = 1;
  ctx.allDevices.choosenDevs.devices = malloc(1 * sizeof(UtDevice));
  ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX] =
      *ctx.allDevices.devicesPtrs[ctx.allDevices.choosenDeviceIdx];

  ctx.allDevices.choosenIdsList = malloc(1 * sizeof(cl_device_id));
  ctx.allDevices.choosenIdsList[libParam.DEVICE_INDEX] = ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].id;

  ThisLabDevices device;
  if (ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].deviceType == DISCRETE_GPU) {
    if (ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].isAmd) {
      device = AMD;
    } else {
      device = NVIDIA;
    }
  } else {
    if (ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].clHostUniMem) {
      device = INTEL_INTEGRA;
    } else {
      device = INTEL_CPU;
    }
  }

  if (!libParam.TEST_IGNORE) {
    if (device == AMD) {
      libParam.v2Choosen = libParam.v2Amd;
    } else if (device == NVIDIA) {
      libParam.v2Choosen = libParam.v2Nvidia;
    } else if (device == INTEL_INTEGRA) {
      libParam.v2Choosen = libParam.v2IntelIngegra;
    } else {
      libParam.v2Choosen = libParam.v2IntelCpu;
    }
  }

  // print
  libPrintChoosenDevice(&ctx);

  cl_int resCreateContext = libUtcreateClContext(&ctx);
  if (resCreateContext != CL_SUCCESS) {
    return resCreateContext;
  }

  cl_int resCreateProgram = libUtcreateProgram(&ctx);
  if (resCreateProgram != CL_SUCCESS) {
    return resCreateProgram;
  }

  cl_int resBuild = libUtProcBuild(&ctx);
  if (resBuild != CL_SUCCESS) {
    return resBuild;
  }

  cl_int resCreateQueue = libUtcreateQueue(&ctx);
  if (resCreateQueue != CL_SUCCESS) {
    return resCreateQueue;
  }
  return CL_SUCCESS;
}

cl_int libMultiplyMatricesGpu(MNK mnk, const float* restrict a, const float* restrict b, float* restrict c) {
  return libArtificialMultiplyMatricesGpu(mnk, a, b, c, 1);
}

cl_ulong libHelperGetTime() {
  cl_ulong nanosec = (long long) (omp_get_wtime() * 1e9);
  return nanosec; // ns
}

static size_t closestUp(size_t a, size_t b) {
  return a / b * b + (a % b != 0 ? 1 : 0);
}

cl_uint libArtificialMultiplyMatricesGpu(
    MNK mnk,
    const MatrixType* restrict a,
    const MatrixType* restrict b,
    float* restrict c,
    cl_uint artificialFactor
) {
  libUtLogg("===[[new calculation]]===\n");

  if (libParam.CALCULATING_MODE == V_2 &&
      (mnk.n % libParam.v2Choosen.N_SIZE != 0 || mnk.k % libParam.v2Choosen.N_SIZE != 0)) {
    ctx.data.deviceInfo.nDim = closestUp(mnk.n, libParam.v2Choosen.N_SIZE);
    ctx.data.deviceInfo.kDim = closestUp(mnk.k, libParam.v2Choosen.N_SIZE);

    MatrixType* newA = calloc(mnk.m * ctx.data.deviceInfo.kDim, sizeof(MatrixType));
    MatrixType* newB = calloc(ctx.data.deviceInfo.kDim * ctx.data.deviceInfo.nDim, sizeof(MatrixType));

    for (size_t row = 0; row < mnk.m; row++) {
      memcpy(newA + row * ctx.data.deviceInfo.kDim, a + row * mnk.k, mnk.k * sizeof(MatrixType));
    }

    for (size_t row = 0; row < mnk.k; row++) {
      memcpy(newB + row * ctx.data.deviceInfo.nDim, b + row * mnk.n, mnk.n * sizeof(MatrixType));
    }

    ctx.data.a = newA;
    ctx.data.b = newB;
    ctx.data.c = c;
  } else {
    ctx.data.deviceInfo.nDim = mnk.n;
    ctx.data.deviceInfo.kDim = mnk.k;

    ctx.data.a = a;
    ctx.data.b = b;
    ctx.data.c = c;
  }

  ctx.data.deviceInfo.mDim = mnk.m;
  ctx.data.deviceInfo.artificialFactorIterations = artificialFactor;
  ctx.data.sizeA = ctx.data.deviceInfo.mDim * ctx.data.deviceInfo.kDim;
  ctx.data.sizeB = ctx.data.deviceInfo.kDim * ctx.data.deviceInfo.nDim;
  ctx.data.sizeC = ctx.data.deviceInfo.mDim * ctx.data.deviceInfo.nDim;

  cl_int resCreateBuffers = libUtcreateBuffers(&ctx);
  if (resCreateBuffers != CL_SUCCESS) {
    return resCreateBuffers;
  }

  cl_event eventTransportTo[2];
  cl_int resEnqueue2Dev = libUtenqueue2Dev(&ctx, &eventTransportTo[0], &eventTransportTo[1]);
  if (resEnqueue2Dev != CL_SUCCESS) {
    return resEnqueue2Dev;
  }

  cl_int resCreateKernelSetArgs = libUtcreateKernel(&ctx);
  if (resCreateKernelSetArgs != CL_SUCCESS) {
    return resCreateKernelSetArgs;
  }

  cl_int resWaitBuf = clWaitForEvents(2, eventTransportTo);
  if (resWaitBuf != CL_SUCCESS) {
    return resWaitBuf;
  }

  if (libParam.ENABLE_GPU_PROFILING) {
    ProfTimes timeFst = libUtgetProfilingInfo(&eventTransportTo[0]);
    ProfTimes timeSec = libUtgetProfilingInfo(&eventTransportTo[1]);
    ProfTimes summarize = {
        timeFst.queued + timeSec.queued,
        timeFst.submit + timeSec.submit,
        timeFst.start + timeSec.start,
        timeFst.end + timeSec.end
    };
    ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].profilingGpu.transportToDev = summarize;
  }
  clReleaseEvent(eventTransportTo[0]);
  clReleaseEvent(eventTransportTo[1]);

  cl_event eventCalculating;
  cl_int resRunKernel = libUtenqueueRunKernel(&ctx, &eventCalculating);
  if (resRunKernel != CL_SUCCESS) {
    return resRunKernel;
  }

  cl_ulong start = libHelperGetTime();
  cl_int resWaitKernel = clWaitForEvents(1, &eventCalculating);
  if (resWaitKernel != CL_SUCCESS) {
    return resWaitKernel;
  }
  cl_ulong end = libHelperGetTime();
  ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].profilingGpu.timeWaitingKernel = end - start;

  if (libParam.ENABLE_GPU_PROFILING) {
    ProfTimes times = libUtgetProfilingInfo(&eventCalculating);
    ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].profilingGpu.calculating = times;

    ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].profilingGpu.amountOperations = mnk.m * mnk.n * mnk.k;

    ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].profilingGpu.artificialFactor = artificialFactor;
  }
  clReleaseEvent(eventCalculating);

  cl_event eventTransportFrom;
  cl_int resEnq2Host = libUtenqueue2Host(&ctx, &eventTransportFrom);
  if (resEnq2Host != CL_SUCCESS) {
    return resEnq2Host;
  }
  if (libParam.ENABLE_GPU_PROFILING) {
    ProfTimes timeFrom = libUtgetProfilingInfo(&eventTransportFrom);
    ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].profilingGpu.transportFromDev = timeFrom;
  }
  if (eventTransportFrom) {
    clReleaseEvent(eventTransportFrom);
  }

  return CL_SUCCESS;
}

cl_uint libGetMaxFrequency() {
  return ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].clMaxClockFr;
}

void libGetGPUProfiling(ProfilingGPU* result) {
  *result = ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].profilingGpu;
}

size_t libGetMaxWorkGroupSize() {
  return ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].clMaxWorkGroupSize;
}

cl_ulong libGetLocalMemSize() {
  return ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].clDeviceLocalMemSize;
}

cl_int libDestroyContext() {
  libUtLogg("===[[context destroying]]===\n");
  return libUtReleaseContext(&ctx, 0, "");
}

cl_ulong libGetSubQue(ProfTimes profTimes) {
  libUtLogg("libGetSubQue");
  return profTimes.submit - profTimes.queued;
}

cl_ulong libGetStaSub(ProfTimes profTimes) {
  libUtLogg("libGetStaSub");
  return profTimes.start - profTimes.submit;
}

cl_ulong libGetEndSta(ProfTimes profTimes) {
  libUtLogg("libGetEndSta");
  return profTimes.end - profTimes.start;
}

cl_ulong libGetRealyFullTime(ProfTimes profTimes) {
  libUtLogg("libGetRealyFullTime");
  return libGetSubQue(profTimes) + libGetStaSub(profTimes) + libGetEndSta(profTimes);
}

cl_float libGetFlops(ProfilingGPU profTimes) {
  libUtLogg("libGetFlops");

  cl_float flops =
      profTimes.amountOperations * profTimes.artificialFactor / (libGetEndSta(profTimes.calculating) / pow(10, 9));

  return flops;
}

void libPrintPlatsNum(UtLibContext* ctx) {
  if (!libParam.ENABLE_GPU_LOGG) {
    return;
  }
  printf("$num of platforms\n");
  printf("num platform ids = %u\n", ctx->allPlatIDs.size);
}

void libPrintPlatsInfo(UtLibContext* ctx) {
  if (!libParam.ENABLE_GPU_LOGG) {
    return;
  }
  printf("$print platforms info\n");
  for (size_t i = 0; i < ctx->allPlatIDs.size; i++) {
    printf("platform_{%zu} = %s\n", i, ctx->allPlats.names[i].string);
  }
}

void libPrintDevIDs(UtLibContext* ctx) {
  if (!libParam.ENABLE_GPU_LOGG) {
    return;
  }
  printf("$print devices ids\n");
  for (size_t i = 0; i < ctx->allPlats.size; i++) {
    printf("for %zu platform (id %zu) num devices = %zu\n", i, ctx->allPlatIDs.arr[i], ctx->allPlats.platsDevs[i].size);
    for (size_t j = 0; j < ctx->allPlats.platsDevs[i].size; j++) {
      printf("| device_{%zu} id = %i\n", j, ctx->allPlats.platsDevs[i].devices[j].id);
    }
  }
}

void libPrintDevInfo(UtLibContext* ctx) {
  if (!libParam.ENABLE_GPU_LOGG) {
    return;
  }
  printf("$print devices info\n");
  printf("type map \n| {\n");
  printf("|  DISCRETE_GPU <-> %i\n", DISCRETE_GPU);
  printf("|  INTEGRATED_GPU <-> %i\n", INTEGRATED_GPU);
  printf("|  CPU <-> %i\n", CPU);
  printf("|  OTHER <-> %i\n", OTHER);
  printf("| }\n");

  for (size_t i = 0; i < ctx->allPlats.size; i++) {
    UtPlatformDevices* curDevices = ctx->allPlats.platsDevs + i;
    printf("for platform_{%zu} device names:\n", i);
    for (size_t j = 0; j < curDevices->size; j++) {
      printf("| device_{%zu} (id = %i)\n", j, curDevices->devices[j].id);
      printf("|   name = %s\n", curDevices->devices[j].clName.string);
      printf("|   clDevType = %llu\n", curDevices->devices[j].clDeviceType);
      printf("|   clHostMem = %u\n", curDevices->devices[j].clHostUniMem);
      printf("|   clVersion = %s\n", curDevices->devices[j].clVersion.string);
      printf("|   clDriverVersion = %s\n", curDevices->devices[j].clDriverVersion.string);
      printf("|   type = %i\n", curDevices->devices[j].deviceType);
    }
  }
}

void libPrintChoosenDevice(UtLibContext* ctx) {
  if (!libParam.ENABLE_GPU_LOGG) {
    return;
  }
  printf("$print choosen device\n");
  printf("choosen device id = %i\n", ctx->allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].id);
  printf("choosen device id = %i\n", ctx->allDevices.choosenIdsList[libParam.DEVICE_INDEX]);
  printf("choosen devices amount = %zu\n", ctx->allDevices.choosenDevs.size);
}

const char* libGetChoosenDeviceName() {
  return ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].clName.string;
}

const char* libGetChoosenPlatformName() {
  return ctx.allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].clPlatformName.string;
}
