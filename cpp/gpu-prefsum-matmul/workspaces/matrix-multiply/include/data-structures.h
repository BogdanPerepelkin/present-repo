#ifndef UTILS_H
#define UTILS_H

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <stdbool.h>
#include <stdio.h>

#define BUFF 256
#define LONG_BUFF 1024

// PUBLIC:

typedef cl_float MatrixType;

typedef enum {
  INVAID_TILE_THREAD_SIZES = 67,
} ERROR_CODES;

typedef enum DevType {
  DISCRETE_GPU,
  INTEGRATED_GPU,
  CPU,
  OTHER,
} DeviceType;

typedef enum GPUCalculatingMode {
  V_0,
  V_1,
  V_2,
  V_2_SHORT,
  V_2_ACCESS,
  V_2_BANKS,
} GPUCalculatingMode;

typedef struct {
  size_t TILE_SIZE_W;
  size_t TILE_SIZE_H;
  size_t TILE_SIZE_K;
  size_t THREAD_RANGE_W;
  size_t THREAD_RANGE_H;
  size_t N_SIZE;
} KernelInfoV2;

typedef struct {
  // non zero if choosen many devices (not in this lab, but potential)
  size_t DEVICE_INDEX;

  // old
  size_t SHORT_TILE_SIZE;
  size_t SHORT_THREAD_RANGE;

  // new
  KernelInfoV2 v2Amd;
  KernelInfoV2 v2Nvidia;
  KernelInfoV2 v2IntelIngegra;
  KernelInfoV2 v2IntelCpu;

  KernelInfoV2 v2Choosen;

  // optimizatioins
  _Bool NOUNROLL;

  // log-profiling flags
  _Bool ENABLE_GPU_PROFILING;
  _Bool ENABLE_GPU_LOGG;
  _Bool ENABLE_CPU_PROFILING;
  _Bool ENABLE_CPU_TRANSPOSE;

  GPUCalculatingMode CALCULATING_MODE;

  _Bool TEST_IGNORE;

} GyperParametersLibrary;

typedef struct {
  DeviceType deviceType;
  size_t deviceIndex;
  size_t mapDevType2Priority[4];
} LibInitializeParameters;

typedef struct {
  uint32_t m;
  uint32_t n;
  uint32_t k;
} MNK;

typedef struct {
  cl_uint mDim;
  cl_uint nDim;
  cl_uint kDim;
  cl_uint artificialFactorIterations;
} DeviceInfo;

typedef struct {
  cl_ulong queued;
  cl_ulong submit;
  cl_ulong start;
  cl_ulong end;
} ProfTimes;

typedef struct {
  ProfTimes calculating;
  ProfTimes transportToDev;
  ProfTimes transportFromDev;

  size_t artificialFactor;
  size_t amountOperations;

  cl_ulong timeWaitingKernel;

} ProfilingGPU;

typedef struct {
  cl_ulong seconds; // ns
  cl_float flops; // TFps
} ProfilingCPU;

// PRIVATE:

typedef struct {
  cl_platform_id* arr;
  cl_uint size;
} UtVecPlatIDs;

typedef struct {
  char* string;
  cl_uint size;
} UtString;

typedef struct {
  cl_device_id id;
  cl_device_type clDeviceType;
  cl_bool clHostUniMem;
  UtString clName;
  UtString clVersion;
  UtString clDriverVersion;
  UtString clPlatformName; // not own
  DeviceType deviceType;
  cl_bool isAmd;
  cl_uint clMaxClockFr; // MHz = (1 / s) * 10**6
  cl_uint clAmountCores;
  size_t clMaxWorkGroupSize;
  cl_ulong clDeviceLocalMemSize; // bytes

  ProfilingGPU profilingGpu;
  ProfilingCPU profilingCpu;
} UtDevice;

typedef struct {
  UtDevice* devices;
  size_t size;
} UtPlatformDevices;

// not own devices
// own array of pointers
typedef struct {
  UtDevice** devicesPtrs;
  size_t size;

  // for удобство
  size_t choosenDeviceIdx;
  UtPlatformDevices choosenDevs;
  cl_device_id* choosenIdsList;
} UtAllDevices; // new

typedef struct {
  UtString* names;
  UtPlatformDevices* platsDevs;
  size_t size;
} UtPlatforms;

typedef struct {
  cl_mem buf1;
  cl_mem buf2;
  cl_mem buf3;
} UtBuffers;

typedef struct {
  size_t* arr;
} UtArrayDimensions;

typedef struct {
  cl_uint size;
  UtArrayDimensions global;
  UtArrayDimensions local;
} UtRunDimsInfo;

typedef struct {
  DeviceInfo deviceInfo;
  cl_uint sizeA;
  cl_uint sizeB;
  cl_uint sizeC;
  const MatrixType* a;
  const MatrixType* b;
  MatrixType* c;

  // dimensions
  UtRunDimsInfo runDimsInfo;
} UtData;

typedef struct {
  UtVecPlatIDs allPlatIDs;
  UtPlatforms allPlats;
  // AllDevices allDevices; // new
  UtAllDevices allDevices; // new_2
  size_t mapDev2Priority[4];
  // not own __
  UtData data;
  // not own __
  cl_context clContext;
  cl_program clProgram;
  cl_command_queue clCommandQueue;
  UtBuffers buffers;
  cl_kernel clKernel;
} UtLibContext;

#endif
