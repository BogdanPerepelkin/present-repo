#include "data-structures.h"
#include "lib-utils.h"
#include "multiply-matrices.h"

#include <assert.h>
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

#ifndef LIB_SRC_DIR
#define LIB_SRC_DIR "include"
#endif

const char* LIB_CLCODE_PATHS[] = {
    LIB_SRC_DIR "/mul-0.cl",
    LIB_SRC_DIR "/mul-1.cl",
    LIB_SRC_DIR "/mul-2.cl",
    LIB_SRC_DIR "/mul-2-short.cl",
    LIB_SRC_DIR "/mul-2-access.cl"
};

cl_uint libUtProcGetPlatforms(UtLibContext* ctx) {
  libUtLogg("#____________[procGetPlatforms]");
  cl_uint numPlatforms;
  cl_int resIDs = clGetPlatformIDs(0, NULL, &numPlatforms);
  if (resIDs != CL_SUCCESS) {
    return libUtReleaseContext(ctx, resIDs, "1 clGetPlatformIDs");
  }

  ctx->allPlatIDs.arr = malloc((size_t) numPlatforms * sizeof(cl_platform_id));
  ctx->allPlatIDs.size = numPlatforms;
  ctx->allPlats.size = numPlatforms;
  resIDs = clGetPlatformIDs(numPlatforms, ctx->allPlatIDs.arr, NULL);
  if (resIDs != CL_SUCCESS) {
    return libUtReleaseContext(ctx, resIDs, "2 clGetPlatformIDs");
  }
  return CL_SUCCESS;
}

cl_int libUtProcGetPlatformInfo(UtLibContext* ctx) {
  libUtLogg("#____________[procGetPlatformInfo]");
  ctx->allPlats.names = malloc(ctx->allPlats.size * sizeof(UtString));
  for (size_t i = 0; i < ctx->allPlatIDs.size; i++) {
    cl_platform_id platformID = ctx->allPlatIDs.arr[i];
    cl_platform_info paramName = CL_PLATFORM_NAME;
    size_t paramValueSizeRet;

    cl_int resPlatInfo = clGetPlatformInfo(platformID, paramName, 0, NULL, &paramValueSizeRet);

    // todo error handling
    if (resPlatInfo != CL_SUCCESS) {
      return libUtReleaseContext(ctx, resPlatInfo, "1 getPlInf");
    }

    size_t paramValueSize = paramValueSizeRet;
    void* paramValue = malloc(paramValueSizeRet * sizeof(char));

    resPlatInfo = clGetPlatformInfo(platformID, paramName, paramValueSize, paramValue, NULL);

    // todo error handling
    if (resPlatInfo != CL_SUCCESS) {
      return libUtReleaseContext(ctx, resPlatInfo, "2 getPlInf");
    }

    ctx->allPlats.names[i].string = paramValue;
    ctx->allPlats.names[i].size = paramValueSize;
  }
  return CL_SUCCESS;
}

cl_int libUtProcGetDeviceIDs(UtLibContext* ctx) {
  libUtLogg("#____________[procGetDeviceIDs]");
  for (size_t i = 0; i < ctx->allPlats.size; i++) {
    cl_platform_id platform = ctx->allPlatIDs.arr[i];
    cl_device_type device_type = CL_DEVICE_TYPE_ALL;
    cl_device_id devices = NULL;
    cl_uint num_devices;
    cl_int resGetDevIds = clGetDeviceIDs(platform, device_type, 0, NULL, &num_devices);
    if (resGetDevIds != CL_SUCCESS) {
      return libUtReleaseContext(ctx, resGetDevIds, "1 getDevIDs");
    }

    ctx->allPlats.platsDevs[i].size = num_devices;
    ctx->allPlats.platsDevs[i].devices = calloc(num_devices, sizeof(UtDevice));

    cl_device_id tempIds[num_devices];
    resGetDevIds = clGetDeviceIDs(platform, device_type, num_devices, tempIds, NULL);
    if (resGetDevIds != CL_SUCCESS) {
      return libUtReleaseContext(ctx, resGetDevIds, "2 getDevIDs");
    }
    for (cl_uint k = 0; k < num_devices; k++) {
      ctx->allPlats.platsDevs[i].devices[k].id = tempIds[k];
    }
  }
  return CL_SUCCESS;
}

cl_int
libUtHelperAllocatingGet(UtLibContext* ctx, cl_device_id id, cl_device_info info, UtString* arr, const char* message) {
  size_t clVersionsSizeRet;
  cl_int resGetDevInfo = clGetDeviceInfo(id, info, 0, NULL, &clVersionsSizeRet);
  if (resGetDevInfo != CL_SUCCESS) {
    char buf[BUFF];
    snprintf(buf, sizeof(buf), "1 %s", message);
    return libUtReleaseContext(ctx, resGetDevInfo, buf);
  }

  arr->size = clVersionsSizeRet;
  arr->string = malloc(clVersionsSizeRet * sizeof(char));

  resGetDevInfo = clGetDeviceInfo(id, info, clVersionsSizeRet, arr->string, NULL);
  if (resGetDevInfo != CL_SUCCESS) {
    return libUtReleaseContext(ctx, resGetDevInfo, "2 getDevInfo-CL_DEVICE_VERSION");
  }

  return CL_SUCCESS;
}

cl_int libUtProcGetDeviceInfo(UtLibContext* ctx) {
  libUtLogg("#____________[procGetDeviceInfo]");
  for (size_t i = 0; i < ctx->allPlats.size; i++) {
    UtPlatformDevices* curDevices = ctx->allPlats.platsDevs + i;

    for (size_t j = 0; j < curDevices->size; j++) {
      cl_device_id device = curDevices->devices[j].id;
      size_t clNameSizeRet;
      size_t clVersionsSizeRet;

      cl_int resGetDevInfo =
          clGetDeviceInfo(device, CL_DEVICE_TYPE, sizeof(cl_device_type), &curDevices->devices[j].clDeviceType, NULL);
      if (resGetDevInfo != CL_SUCCESS) {
        return libUtReleaseContext(ctx, resGetDevInfo, "getDevInfo-CL_DEVICE_TYPE");
      }

      if (resGetDevInfo != CL_SUCCESS) {
        return resGetDevInfo;
      }

      resGetDevInfo = clGetDeviceInfo(
          device,
          CL_DEVICE_MAX_CLOCK_FREQUENCY,
          sizeof(cl_uint),
          &curDevices->devices[j].clMaxClockFr,
          NULL
      );
      if (resGetDevInfo != CL_SUCCESS) {
        return libUtReleaseContext(ctx, resGetDevInfo, "getDevInfo-CL_DEVICE_MAX_CLOCK_FREQUENCY");
      }

      resGetDevInfo = clGetDeviceInfo(
          device,
          CL_DEVICE_MAX_COMPUTE_UNITS,
          sizeof(cl_uint),
          &curDevices->devices[j].clAmountCores,
          NULL
      );
      if (resGetDevInfo != CL_SUCCESS) {
        return libUtReleaseContext(ctx, resGetDevInfo, "getDevInfo-CL_DEVICE_MAX_COMPUTE_UNITS");
      }

      resGetDevInfo = clGetDeviceInfo(
          device,
          CL_DEVICE_MAX_WORK_GROUP_SIZE,
          sizeof(size_t),
          &curDevices->devices[j].clMaxWorkGroupSize,
          NULL
      );
      if (resGetDevInfo != CL_SUCCESS) {
        return libUtReleaseContext(ctx, resGetDevInfo, "getDevInfo-CL_DEVICE_MAX_WORK_GROUP_SIZE");
      }

      resGetDevInfo = clGetDeviceInfo(
          device,
          CL_DEVICE_LOCAL_MEM_SIZE,
          sizeof(cl_ulong),
          &curDevices->devices[j].clDeviceLocalMemSize,
          NULL
      );
      if (resGetDevInfo != CL_SUCCESS) {
        return libUtReleaseContext(ctx, resGetDevInfo, "getDevInfo-CL_DEVICE_MAX_WORK_GROUP_SIZE");
      }

      resGetDevInfo = clGetDeviceInfo(
          device,
          CL_DEVICE_HOST_UNIFIED_MEMORY,
          sizeof(cl_bool),
          &curDevices->devices[j].clHostUniMem,
          NULL
      );
      if (resGetDevInfo != CL_SUCCESS) {
        return libUtReleaseContext(ctx, resGetDevInfo, "getDevInfo-CL_DEVICE_HOST_UNIFIED_MEMORY");
      }

      resGetDevInfo = libUtHelperAllocatingGet(
          ctx,
          device,
          CL_DEVICE_NAME,
          &curDevices->devices[j].clName,
          "getDevInfo-CL_DEVICE_NAME"
      );
      if (resGetDevInfo != CL_SUCCESS) {
        return resGetDevInfo;
      }

      resGetDevInfo = libUtHelperAllocatingGet(
          ctx,
          device,
          CL_DEVICE_VERSION,
          &curDevices->devices[j].clVersion,
          "getDevInfo-CL_DEVICE_VERSION"
      );
      if (resGetDevInfo != CL_SUCCESS) {
        return resGetDevInfo;
      }

      resGetDevInfo = libUtHelperAllocatingGet(
          ctx,
          device,
          CL_DRIVER_VERSION,
          &curDevices->devices[j].clDriverVersion,
          "getDevInfo-CL_DRIVER_VERSION"
      );

      curDevices->devices[j].clPlatformName = ctx->allPlats.names[i];

      if (resGetDevInfo != CL_SUCCESS) {
        return resGetDevInfo;
      }

      size_t vendorSizeRet;
      resGetDevInfo = clGetDeviceInfo(device, CL_DEVICE_VENDOR, 0, NULL, &vendorSizeRet);
      if (resGetDevInfo != CL_SUCCESS) {
        return libUtReleaseContext(ctx, resGetDevInfo, "getDevInfo-CL_DEVICE_VENDOR-1");
      }

      char* vendor = malloc(vendorSizeRet);
      if (vendor == NULL) {
        return libUtReleaseContext(ctx, CL_OUT_OF_HOST_MEMORY, "malloc for vendor");
      }
      resGetDevInfo = clGetDeviceInfo(device, CL_DEVICE_VENDOR, vendorSizeRet, vendor, NULL);
      if (resGetDevInfo != CL_SUCCESS) {
        free(vendor);
        return libUtReleaseContext(ctx, resGetDevInfo, "getDevInfo-CL_DEVICE_VENDOR");
      }

      // isAmd
      curDevices->devices[j].isAmd =
          (strstr(vendor, "Advanced Micro Devices") != NULL) || (strstr(vendor, "AMD") != NULL);
      free(vendor);

      // deviceType
      if (curDevices->devices[j].clDeviceType == CL_DEVICE_TYPE_CPU) {
        curDevices->devices[j].deviceType = CPU;
      } else if (curDevices->devices[j].clDeviceType == CL_DEVICE_TYPE_GPU) {
        if (curDevices->devices[j].clHostUniMem) {
          curDevices->devices[j].deviceType = INTEGRATED_GPU;
        } else {
          curDevices->devices[j].deviceType = DISCRETE_GPU;
        }
      } else {
        curDevices->devices[j].deviceType = OTHER;
      }
    }
  }
  return CL_SUCCESS;
}

cl_int libUtReadPlatformDeviceInfo(UtLibContext* ctx) {
  cl_int resGetPl = libUtProcGetPlatforms(ctx);
  if (resGetPl != CL_SUCCESS) {
    return resGetPl;
  }
  libPrintPlatsNum(ctx);

  cl_int resGetInfo = libUtProcGetPlatformInfo(ctx);
  if (resGetInfo != CL_SUCCESS) {
    return resGetInfo;
  }
  libPrintPlatsInfo(ctx);

  ctx->allPlats.platsDevs = malloc(ctx->allPlats.size * sizeof(UtPlatformDevices));

  cl_int resGetDevIDs = libUtProcGetDeviceIDs(ctx);
  if (resGetDevIDs != CL_SUCCESS) {
    return resGetDevIDs;
  }
  libPrintDevIDs(ctx);

  return CL_SUCCESS;
}

cl_int libUtcreateClContext(UtLibContext* ctx) {
  cl_int resCrContext;
  ctx->clContext = clCreateContext(
      NULL,
      ctx->allDevices.choosenDevs.size,
      ctx->allDevices.choosenIdsList,
      NULL,
      NULL,
      &resCrContext
  );

  if (resCrContext != CL_SUCCESS) {
    return libUtReleaseContext(ctx, resCrContext, "clCreateContext");
  }
  return CL_SUCCESS;
}

cl_int libUtcreateProgram(UtLibContext* ctx) {
  libUtLogg("#____________[createProgram]");
  const char* cl_code = LIB_CLCODE_PATHS[libParam.CALCULATING_MODE];
  FILE* file = fopen(cl_code, "rb");
  if (file == NULL) {
    return libUtReleaseContext(ctx, 1, "opencl-c code: cannot open file");
  }
  int resFseek = fseek(file, 0, SEEK_END);
  if (resFseek != 0) {
    return libUtReleaseContext(ctx, 1, "opencl-c code: fseek err");
  }
  const long fileLen = ftell(file);
  rewind(file);
  char* sourceForOpencl = malloc(fileLen + 1);
  if (sourceForOpencl == NULL) {
    fclose(file);
    return libUtReleaseContext(ctx, 1, "opencl-c code: malloc for file.cl err");
  }
  size_t resFread = fread(sourceForOpencl, 1, fileLen, file);
  if (resFread != fileLen) {
    free(sourceForOpencl);
    fclose(file);
    return libUtReleaseContext(ctx, 1, "opencl-c code: fread err while reading file for opencl source");
  }
  sourceForOpencl[fileLen] = '\0';
  fclose(file);

  const char* constSrc = sourceForOpencl;
  const size_t constFileLen = (size_t) fileLen;
  cl_int resCrProgram;
  ctx->clProgram = clCreateProgramWithSource(ctx->clContext, 1, &constSrc, &constFileLen, &resCrProgram);
  if (resCrProgram != CL_SUCCESS) {
    return libUtReleaseContext(ctx, resCrProgram, "clCreateProgramWithSource");
  }
  free(sourceForOpencl);
  return CL_SUCCESS;
}

size_t libUtPotolok(size_t mSize, size_t tileSize) {
  return mSize / tileSize + (mSize % tileSize != 0);
}

void setForTrueV_2(char* buf, size_t size, _Bool isUnroll) {
  snprintf(
      buf,
      size,
      "-DTILE_SIZE_W=%zu -DTILE_SIZE_H=%zu -DTILE_SIZE_K=%zu -DTHREAD_RANGE_W=%zu -DTHREAD_RANGE_H=%zu "
      " -DLOCAL_GR_SIZE_W=%zu -DLOCAL_GR_SIZE_H=%zu -DN_SIZE=%zu",
      libParam.v2Choosen.TILE_SIZE_W,
      libParam.v2Choosen.TILE_SIZE_H,
      libParam.v2Choosen.TILE_SIZE_K,
      libParam.v2Choosen.THREAD_RANGE_W,
      libParam.v2Choosen.THREAD_RANGE_H,
      libUtPotolok(libParam.v2Choosen.TILE_SIZE_W, libParam.v2Choosen.THREAD_RANGE_W),
      libUtPotolok(libParam.v2Choosen.TILE_SIZE_H, libParam.v2Choosen.THREAD_RANGE_H),
      libParam.v2Choosen.N_SIZE
  );
  if (isUnroll) {
    size_t len = strlen(buf);
    if (len < size - 1) {
      snprintf(buf + len, size - len, " -DUNROLL=1");
    }
  }
}

void libUtmakeOptionsDefines(char* buf, size_t size) {
  switch (libParam.CALCULATING_MODE) {
  case V_0:
    buf[0] = '\0';
    break;
  case V_1:
    snprintf(buf, size, "-DTILE_SIZE=%zu", libParam.SHORT_TILE_SIZE);
    break;
  case V_2:
    setForTrueV_2(buf, size, libParam.NOUNROLL);
    break;
  case V_2_SHORT:
    snprintf(
        buf,
        size,
        "-DTILE_SIZE=%zu -DSINGLE_THREAD_RANGE=%zu",
        libParam.SHORT_TILE_SIZE,
        libParam.SHORT_THREAD_RANGE
    );
    break;
  case V_2_ACCESS:
    setForTrueV_2(buf, size, libParam.NOUNROLL);
    break;
  case V_2_BANKS:
    assert(false);
    break;
  default:
    libUtErrLog("wrong, just absolutely wrong value on CALCULATING_MODE");
    break;
  }
}

void libUtmakeOptionsIncludes(char* buf, size_t size) {
  snprintf(buf, size, "-I%s", LIB_SRC_DIR);
}

cl_int libUtProcBuild(UtLibContext* ctx) {
  libUtLogg("#____________[ProcBuild]");
  char programOptions[LONG_BUFF];
  char includes[LONG_BUFF];
  libUtmakeOptionsIncludes(includes, sizeof(includes));
  char defines[LONG_BUFF];
  libUtmakeOptionsDefines(defines, sizeof(defines));

  snprintf(programOptions, sizeof(programOptions), "%s %s", includes, defines);

  cl_int resBuild = clBuildProgram(ctx->clProgram, 1, ctx->allDevices.choosenIdsList, programOptions, NULL, NULL);
  if (resBuild != CL_SUCCESS) {
    for (size_t i = 0; i < ctx->allDevices.choosenDevs.size; i++) {
      size_t sizeRet;
      cl_int resBuildInfo = clGetProgramBuildInfo(
          ctx->clProgram,
          ctx->allDevices.choosenDevs.devices[i].id,
          CL_PROGRAM_BUILD_LOG,
          0,
          NULL,
          &sizeRet
      );
      if (resBuildInfo != CL_SUCCESS) {
        return libUtReleaseContext(ctx, resBuildInfo, "1 clGetProgramBuildInfo");
      }

      char* errorMessage = malloc(sizeRet + 1);
      if (errorMessage == NULL) {
        return libUtReleaseContext(ctx, 1, "malloc for build log");
      }

      resBuildInfo = clGetProgramBuildInfo(
          ctx->clProgram,
          ctx->allDevices.choosenDevs.devices[i].id,
          CL_PROGRAM_BUILD_LOG,
          sizeRet,
          errorMessage,
          NULL
      );
      if (resBuildInfo != CL_SUCCESS) {
        free(errorMessage);
        return libUtReleaseContext(ctx, resBuildInfo, "2 clGetProgramBuildInfo");
      }
      errorMessage[sizeRet] = '\0';
      char errorLog[BUFF];
      snprintf(errorLog, BUFF, "build log for device_{%zu} with id %i", i, ctx->allDevices.choosenDevs.devices[i].id);
      libUtErrLog(errorLog);
      libUtErrLog(errorMessage);

      free(errorMessage);
    }
    return libUtReleaseContext(ctx, resBuild, "clBuildProgram");
  }
  return CL_SUCCESS;
}

cl_int libUtcreateQueue(UtLibContext* ctx) {
  cl_int resCrQueue;
  cl_command_queue_properties properties;
  if (libParam.ENABLE_GPU_PROFILING) {
    properties = CL_QUEUE_PROFILING_ENABLE;
  } else {
    properties = 0;
  }
  ctx->clCommandQueue = clCreateCommandQueue(
      ctx->clContext,
      ctx->allDevices.choosenDevs.devices[libParam.DEVICE_INDEX].id,
      properties,
      &resCrQueue
  );

  if (resCrQueue != CL_SUCCESS) {
    return libUtReleaseContext(ctx, resCrQueue, "clCreateCommandQueue");
  }
  return CL_SUCCESS;
}

// abstract bullsheet
cl_int libUtcreateBuffers(UtLibContext* ctx) {
  libUtLogg("createBuffers");
  cl_int resBuff;
  ctx->buffers.buf1 =
      clCreateBuffer(ctx->clContext, CL_MEM_READ_ONLY, sizeof(MatrixType) * ctx->data.sizeA, NULL, &resBuff);
  if (resBuff != CL_SUCCESS) {
    return libUtReleaseContext(ctx, resBuff, "clCreateBuffer 1");
  }
  ctx->buffers.buf2 =
      clCreateBuffer(ctx->clContext, CL_MEM_READ_ONLY, sizeof(MatrixType) * ctx->data.sizeB, NULL, &resBuff);
  if (resBuff != CL_SUCCESS) {
    return libUtReleaseContext(ctx, resBuff, "clCreateBuffer 2");
  }
  ctx->buffers.buf3 =
      clCreateBuffer(ctx->clContext, CL_MEM_READ_WRITE, sizeof(MatrixType) * ctx->data.sizeC, NULL, &resBuff);
  if (resBuff != CL_SUCCESS) {
    return libUtReleaseContext(ctx, resBuff, "clCreateBuffer 3");
  }
  return CL_SUCCESS;
}

// between device and abstract bullsheet
cl_int libUtenqueue2Dev(UtLibContext* ctx, cl_event* evFirstBuf, cl_event* evSecondBuf) {
  libUtLogg("enqueue2Dev");
  cl_int resEnqueue;
  resEnqueue = clEnqueueWriteBuffer(
      ctx->clCommandQueue,
      ctx->buffers.buf1,
      CL_FALSE,
      0,
      sizeof(MatrixType) * ctx->data.sizeA,
      ctx->data.a,
      0,
      NULL,
      evFirstBuf
  );
  if (resEnqueue != CL_SUCCESS) {
    return libUtReleaseContext(ctx, resEnqueue, "clEnqueueWriteBuffer buf1");
  }

  resEnqueue = clEnqueueWriteBuffer(
      ctx->clCommandQueue,
      ctx->buffers.buf2,
      CL_FALSE,
      0,
      sizeof(MatrixType) * ctx->data.sizeB,
      ctx->data.b,
      0,
      NULL,
      evSecondBuf
  );
  if (resEnqueue != CL_SUCCESS) {
    return libUtReleaseContext(ctx, resEnqueue, "clEnqueueWriteBuffer buf2");
  }

  return CL_SUCCESS;
}

cl_int libUtcreateKernel(UtLibContext* ctx) {
  libUtLogg("createKernel");
  cl_int resCrKernel;
  ctx->clKernel = clCreateKernel(ctx->clProgram, "mul", &resCrKernel);
  if (resCrKernel != CL_SUCCESS) {
    return libUtReleaseContext(ctx, resCrKernel, "clCreateKernel");
  }
  cl_int res = clSetKernelArg(ctx->clKernel, 0, sizeof(cl_mem), &ctx->buffers.buf1);
  if (res != CL_SUCCESS) {
    return libUtReleaseContext(ctx, res, "clSetKernelArg arg0");
  }
  res = clSetKernelArg(ctx->clKernel, 1, sizeof(cl_mem), &ctx->buffers.buf2);
  if (res != CL_SUCCESS) {
    return libUtReleaseContext(ctx, res, "clSetKernelArg arg1");
  }
  res = clSetKernelArg(ctx->clKernel, 2, sizeof(cl_mem), &ctx->buffers.buf3);
  if (res != CL_SUCCESS) {
    return libUtReleaseContext(ctx, res, "clSetKernelArg arg2");
  }
  res = clSetKernelArg(ctx->clKernel, 3, sizeof(DeviceInfo), &ctx->data.deviceInfo);
  if (res != CL_SUCCESS) {
    return libUtReleaseContext(ctx, res, "clSetKernelArg arg3");
  }
  return CL_SUCCESS;
}

size_t libUtgetNaiveGlobalDim(size_t size) {
  if (size % libParam.SHORT_TILE_SIZE == 0) {
    return size;
  }
  return size + (libParam.SHORT_TILE_SIZE - size % libParam.SHORT_TILE_SIZE);
}

cl_int libUtAllocateMemory(size_t** dst, size_t size) {
  *dst = malloc(size);
  if (*dst == NULL) {
    libUtErrLog("cant allocate memory");
    return CL_OUT_OF_HOST_MEMORY;
  }
  return CL_SUCCESS;
}

const size_t DIMENSIONS_GLOBAL = 2;
const size_t DIMENSIONS_LOCAL = 2;

static cl_int setV_2Dimensions(UtLibContext* ctx) {
  cl_int resAlloc = libUtAllocateMemory(&ctx->data.runDimsInfo.local.arr, DIMENSIONS_LOCAL * sizeof(size_t));
  if (resAlloc != CL_SUCCESS) {
    return resAlloc;
  }

  ctx->data.runDimsInfo.local.arr[0] = libUtPotolok(libParam.v2Choosen.TILE_SIZE_W, libParam.v2Choosen.THREAD_RANGE_W);
  ctx->data.runDimsInfo.local.arr[1] = libUtPotolok(libParam.v2Choosen.TILE_SIZE_H, libParam.v2Choosen.THREAD_RANGE_H);

  ctx->data.runDimsInfo.global.arr[0] =
      libUtPotolok(ctx->data.deviceInfo.nDim, libParam.v2Choosen.TILE_SIZE_W) * ctx->data.runDimsInfo.local.arr[0];
  ctx->data.runDimsInfo.global.arr[1] =
      libUtPotolok(ctx->data.deviceInfo.mDim, libParam.v2Choosen.TILE_SIZE_H) * ctx->data.runDimsInfo.local.arr[1];
  return CL_SUCCESS;
}

cl_int libUtsetDimensions(UtLibContext* ctx) {
  ctx->data.runDimsInfo.size = DIMENSIONS_GLOBAL;
  cl_int resAlloc = libUtAllocateMemory(&ctx->data.runDimsInfo.global.arr, DIMENSIONS_GLOBAL * sizeof(size_t));
  if (resAlloc != CL_SUCCESS) {
    return resAlloc;
  }

  size_t m = ctx->data.deviceInfo.mDim;
  size_t n = ctx->data.deviceInfo.nDim;

  switch (libParam.CALCULATING_MODE) {
  case V_0:
    ctx->data.runDimsInfo.global.arr[0] = n;
    ctx->data.runDimsInfo.global.arr[1] = m;

    ctx->data.runDimsInfo.local.arr = NULL;
    break;
  case V_1:
    ctx->data.runDimsInfo.global.arr[0] = libUtgetNaiveGlobalDim(n);
    ctx->data.runDimsInfo.global.arr[1] = libUtgetNaiveGlobalDim(m);

    cl_int resAlloc = libUtAllocateMemory(&ctx->data.runDimsInfo.local.arr, DIMENSIONS_LOCAL * sizeof(size_t));
    if (resAlloc != CL_SUCCESS) {
      return resAlloc;
    }

    ctx->data.runDimsInfo.local.arr[0] = libParam.SHORT_TILE_SIZE;
    ctx->data.runDimsInfo.local.arr[1] = libParam.SHORT_TILE_SIZE;
    break;
  case V_2:
    cl_int resSet = setV_2Dimensions(ctx);
    if (resSet != CL_SUCCESS) {
      return resSet;
    }
    break;
  case V_2_SHORT:
    ctx->data.runDimsInfo.global.arr[0] = libUtgetNaiveGlobalDim(n);
    ctx->data.runDimsInfo.global.arr[1] = libUtgetNaiveGlobalDim(m) / libParam.SHORT_TILE_SIZE;

    resAlloc = libUtAllocateMemory(&ctx->data.runDimsInfo.local.arr, DIMENSIONS_LOCAL * sizeof(size_t));
    if (resAlloc != CL_SUCCESS) {
      return resAlloc;
    }

    ctx->data.runDimsInfo.local.arr[0] = libParam.SHORT_TILE_SIZE;
    ctx->data.runDimsInfo.local.arr[1] = 1;
    break;
  case V_2_ACCESS:
    resSet = setV_2Dimensions(ctx);
    if (resSet != CL_SUCCESS) {
      return resSet;
    }
    break;
  case V_2_BANKS:
    assert(false);
    break;
  default:
    libUtErrLog("absolute wrong calc_mode");
    break;
  }

  if (libParam.CALCULATING_MODE > V_0) {
    if (ctx->data.runDimsInfo.local.arr[0] * ctx->data.runDimsInfo.local.arr[1] > libGetMaxWorkGroupSize()) {
      return libUtReleaseContext(
          ctx,
          INVAID_TILE_THREAD_SIZES,
          "[validation] wrong dimensions: local_0 * local_1 > maxGroupSize"
      );
    }
  }
  return CL_SUCCESS;
}

cl_int libUtenqueueRunKernel(UtLibContext* ctx, cl_event* event) {
  libUtLogg("enqueueRunKernel");

  cl_int resSetDimensions = libUtsetDimensions(ctx);
  if (resSetDimensions != CL_SUCCESS) {
    return resSetDimensions;
  }

  cl_int resEnqueue = clEnqueueNDRangeKernel(
      ctx->clCommandQueue,
      ctx->clKernel,
      ctx->data.runDimsInfo.size,
      NULL,
      ctx->data.runDimsInfo.global.arr,
      ctx->data.runDimsInfo.local.arr,
      0,
      NULL,
      event
  );
  if (resEnqueue != CL_SUCCESS) {
    return libUtReleaseContext(ctx, resEnqueue, "clEnqueueNDRangeKernel");
  }
  return CL_SUCCESS;
}

cl_int libUtenqueue2Host(UtLibContext* ctx, cl_event* event) {
  libUtLogg("enqueue2Host");
  cl_int resEnqReadBuf = clEnqueueReadBuffer(
      ctx->clCommandQueue,
      ctx->buffers.buf3,
      CL_TRUE,
      0,
      sizeof(MatrixType) * ctx->data.sizeC,
      ctx->data.c,
      0,
      NULL,
      event
  );
  if (resEnqReadBuf != CL_SUCCESS) {
    return libUtReleaseContext(ctx, resEnqReadBuf, "resEnqReadBuf");
  }
  return CL_SUCCESS;
}

ProfTimes libUtgetProfilingInfo(cl_event* event) {
  libUtLogg("getProfilingInfo");

  cl_ulong queued;
  clGetEventProfilingInfo(*event, CL_PROFILING_COMMAND_QUEUED, sizeof(cl_ulong), &queued, NULL); // ns

  cl_ulong submit;
  clGetEventProfilingInfo(*event, CL_PROFILING_COMMAND_SUBMIT, sizeof(cl_ulong), &submit, NULL); // ns

  cl_ulong start;
  clGetEventProfilingInfo(*event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL); // ns

  cl_ulong end;
  clGetEventProfilingInfo(*event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL); // ns

  ProfTimes result = {queued, submit, start, end};

  return result;
}

cl_int libUtgetAdvancedProfilingInfo(
    UtLibContext* ctx,
    cl_event* event,
    cl_ulong amountOperations,
    cl_ulong* time,
    cl_float* flops
) {
  libUtLogg("getAdvancedProfilingInfo");
  cl_ulong start;
  cl_int resProfilingInfo =
      clGetEventProfilingInfo(*event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL); // ns
  if (resProfilingInfo != CL_SUCCESS) {
    return libUtReleaseContext(ctx, resProfilingInfo, "clGetEventProfilingInfo 1");
  }

  cl_ulong end;
  resProfilingInfo = clGetEventProfilingInfo(*event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL); // ns
  if (resProfilingInfo != CL_SUCCESS) {
    return libUtReleaseContext(ctx, resProfilingInfo, "clGetEventProfilingInfo 2");
  }

  *time = end - start;
  if (flops != nullptr) {
    *flops = amountOperations / (*time / pow(10, 9));
  }
  return CL_SUCCESS;
}

cl_int libUtcreateAllDevs() {
  size_t numDevices = 0;
  for (size_t i = 0; i < ctx.allPlats.size; i++) {
    for (size_t j = 0; j < ctx.allPlats.platsDevs[i].size; j++) {
      numDevices++;
    }
  }
  ctx.allDevices.size = numDevices;
  ctx.allDevices.devicesPtrs = malloc(numDevices * sizeof(UtDevice*));
  if (ctx.allDevices.devicesPtrs == NULL) {
    return libUtReleaseContext(&ctx, 1, "malloc");
  }

  size_t curDevice = 0;
  for (size_t i = 0; i < ctx.allPlats.size; i++) {
    for (size_t j = 0; j < ctx.allPlats.platsDevs[i].size; j++) {
      ctx.allDevices.devicesPtrs[curDevice] = ctx.allPlats.platsDevs[i].devices + j;
      curDevice++;
    }
  }

  // post production device-index
  if (libIni.deviceIndex >= ctx.allDevices.size) {
    ctx.allDevices.choosenDeviceIdx = 0;
  } else {
    ctx.allDevices.choosenDeviceIdx = libIni.deviceIndex;
  }

  return CL_SUCCESS;
}

int libUtfillPriorityMap() {
  DeviceType curType = DISCRETE_GPU;
  size_t curGrade = 0;
  ctx.mapDev2Priority[libIni.deviceType] = curGrade;
  curGrade++;
  while (curType != 4) {
    if (curType != libIni.deviceType) {
      ctx.mapDev2Priority[curType] = curGrade;
      curGrade++;
    }
    curType++;
  }
  return CL_SUCCESS;
}

int libUtcompareAllDevices(const void* inLhs, const void* inRhs) {
  UtDevice* lhs = *((UtDevice**) inLhs);
  UtDevice* rhs = *((UtDevice**) inRhs);

  if (ctx.mapDev2Priority[lhs->deviceType] < ctx.mapDev2Priority[rhs->deviceType]) {
    return -1;
  } else if (ctx.mapDev2Priority[lhs->deviceType] > ctx.mapDev2Priority[rhs->deviceType]) {
    return 1;
  } else {
    if (lhs->isAmd) {
      return -1;
    }
    return 1;
  }
}

cl_int libUtReleaseContext(UtLibContext* ctx, cl_int errCode, const char* message) {
  libUtLogg("#____________[releaseContext]\n");

  if (errCode != CL_SUCCESS) {
    char buf[BUFF];
    snprintf(buf, sizeof(buf), "error code = %d", errCode);
    libUtErrLog(buf);
    libUtErrLog(message);
  }
  if (ctx->allPlatIDs.arr) {
    free(ctx->allPlatIDs.arr);
    ctx->allPlatIDs.arr = NULL;
    ctx->allPlatIDs.size = 0;
  }

  if (ctx->allPlats.names) {
    for (size_t i = 0; i < ctx->allPlats.size; i++) {
      if (ctx->allPlats.names[i].string) {
        free(ctx->allPlats.names[i].string);
        ctx->allPlats.names[i].string = NULL;
        ctx->allPlats.names[i].size = 0;
      }
      if (ctx->allPlats.platsDevs && ctx->allPlats.platsDevs[i].devices) {
        for (size_t j = 0; j < ctx->allPlats.platsDevs[i].size; j++) {
          if (ctx->allPlats.platsDevs[i].devices[j].clName.string) {
            free(ctx->allPlats.platsDevs[i].devices[j].clName.string);
            ctx->allPlats.platsDevs[i].devices[j].clName.string = NULL;
          }
          if (ctx->allPlats.platsDevs[i].devices[j].clVersion.string) {
            free(ctx->allPlats.platsDevs[i].devices[j].clVersion.string);
            ctx->allPlats.platsDevs[i].devices[j].clVersion.string = NULL;
          }
          if (ctx->allPlats.platsDevs[i].devices[j].clDriverVersion.string) {
            free(ctx->allPlats.platsDevs[i].devices[j].clDriverVersion.string);
            ctx->allPlats.platsDevs[i].devices[j].clDriverVersion.string = NULL;
          }
        }
        free(ctx->allPlats.platsDevs[i].devices);
        ctx->allPlats.platsDevs[i].devices = NULL;
        ctx->allPlats.platsDevs[i].size = 0;
      }
    }
    free(ctx->allPlats.names);
    ctx->allPlats.names = NULL;
  }

  if (ctx->allPlats.platsDevs) {
    free(ctx->allPlats.platsDevs);
    ctx->allPlats.platsDevs = NULL;
    ctx->allPlats.size = 0;
  }

  if (ctx->allDevices.devicesPtrs) {
    free(ctx->allDevices.devicesPtrs);
    ctx->allDevices.devicesPtrs = NULL;
    ctx->allDevices.size = 0;
  }

  if (ctx->allDevices.choosenIdsList) {
    free(ctx->allDevices.choosenIdsList);
    ctx->allDevices.choosenIdsList = NULL;
  }

  if (ctx->clProgram) {
    clReleaseProgram(ctx->clProgram);
    ctx->clProgram = NULL;
  }

  if (ctx->clCommandQueue) {
    clReleaseCommandQueue(ctx->clCommandQueue);
    ctx->clCommandQueue = NULL;
  }

  if (ctx->buffers.buf1) {
    clReleaseMemObject(ctx->buffers.buf1);
    ctx->buffers.buf1 = NULL;
  }
  if (ctx->buffers.buf2) {
    clReleaseMemObject(ctx->buffers.buf2);
    ctx->buffers.buf2 = NULL;
  }
  if (ctx->buffers.buf3) {
    clReleaseMemObject(ctx->buffers.buf3);
    ctx->buffers.buf3 = NULL;
  }

  if (ctx->clKernel) {
    clReleaseKernel(ctx->clKernel);
    ctx->clKernel = NULL;
  }

  if (ctx->data.runDimsInfo.global.arr) {
    free(ctx->data.runDimsInfo.global.arr);
    ctx->data.runDimsInfo.global.arr = NULL;
  }

  if (ctx->data.runDimsInfo.local.arr) {
    free(ctx->data.runDimsInfo.local.arr);
    ctx->data.runDimsInfo.local.arr = NULL;
  }
  if (ctx->clContext != NULL) {
    clReleaseContext(ctx->clContext);
    ctx->clContext = NULL;
  }

  return errCode;
}

void libUtErrLog(const char* message) {
  if (printf("[lib err] %s\n", message) < 0) {
    perror("error logging");
  }
}

void libUtLogg(const char* message) {
  if (!libParam.ENABLE_GPU_LOGG) {
    return;
  }
  printf("%s\n", message);
}
