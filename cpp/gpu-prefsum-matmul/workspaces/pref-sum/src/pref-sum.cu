
#ifdef __APPLE__
#include "cuda_runtime_api.h"
#include "driver_types.h"
#endif

#include "../include/errors.h"
#include "../include/kernel-bank.cuh"
#include "../include/kernel-cub-brentstone.cuh"
#include "../include/kernel-cub-koggestone.cuh"
#include "../include/kernel-cub-optimize.cuh"
#include "../include/kernel-cub-reducestone.cuh"
#include "../include/kernel-global.cuh"
#include "../include/kernel-utils.cuh"
#include "../include/pref-sum.h"
#include "../include/utils.h"
#include "../include/wrappers.h"

#include <cuda_runtime.h>

#include <vector>

namespace prefsum {

PrefSum::PrefSum() {
  initialize();

  gyperLibraryParameters.block = {64, 1, 1};
  gyperLibraryParameters.deviceIndex = 0;
  gyperLibraryParameters.enableCpuProfiling = false;
  gyperLibraryParameters.calculatingMode = V_BrentStone;
  gyperLibraryParameters.partition = 0;
}

void PrefSum::initialize() {
  // test
  // cudaDeviceSynchronize();

  int amountDevices;
  cudaError_t resGetDevCount = cudaGetDeviceCount(&amountDevices);
  if (resGetDevCount != cudaSuccess) {
    throw errors::ValidationArgsError("cudaGetDeviceCount", resGetDevCount);
  }

  if (amountDevices <= 0) {
    throw errors::LibInitializeError("amount devices <= 0");
  }

  _properties.assign(amountDevices, {});

  for (size_t deviceIdx = 0; deviceIdx < amountDevices; deviceIdx++) {
    cudaError_t resGetDevProp = cudaGetDeviceProperties(&_properties[deviceIdx], deviceIdx);
  }

  // int maxAllocInt;
  // cudaDeviceGetAttribute(&maxAllocInt, cudaDevAttrMaxMemAllocSize, _properties[0]);
  // logg::log("max alloc size = %i\n", maxAllocInt);

  cudaSetDevice(0);
}

CpuResult::CpuResult(const std::vector<uint>& elements, ull time) {
  _elements = elements;
  _time = time;
}

GpuResult::GpuResult(const std::vector<uint>& elements, GpuTime time, const std::vector<Descriptor>& descriptors) {
  _elements = elements;
  _time = time;
  _descriptors = descriptors;
}

GpuResult::GpuResult(const std::vector<uint>& elements, GpuTime time, std::vector<Descriptor>&& descriptors) {
  _elements = elements;
  _time = time;
  _descriptors = std::move(descriptors);
}

struct DirtyPairTime {
  RaiEvent before;
  RaiEvent after;
};

void validation(const GyperParametersLibrary& gyperLibraryParameters) {
  if (gyperLibraryParameters.partition == 0) {
    throw errors::ValidationArgsError("invalid partition: 0");
  }
  if (gyperLibraryParameters.block.x % gyperLibraryParameters.partition != 0) {
    throw errors::ValidationArgsError("invalid partition: block size must be divisible by partition");
  }
  size_t lowRange = gyperLibraryParameters.block.x / gyperLibraryParameters.partition;
  if (lowRange == 0) {
    throw errors::ValidationArgsError("invalid partition: resulting lowRange == 0");
  }
  const size_t WARP_SIZE = 32;
  if (lowRange > WARP_SIZE) {
    throw errors::ValidationArgsError("invalid partition: lowRange > warp size (contract violated)");
  }
  if (gyperLibraryParameters.partition > WARP_SIZE) {
    throw errors::ValidationArgsError("invalid partition: partition > warp size (contract violated)");
  }
}

GpuResult PrefSum::gpuCalculate(const CalculateParameters& params) {
  ull n = params.elements.size();

  size_t numThreadWork = 1;
  size_t blockSize = gyperLibraryParameters.block.x * numThreadWork;
  size_t gridSize = utils::potolok(n, blockSize);
  size_t paddingVectorSize = gridSize * blockSize;
  // if (gyperLibraryParameters.calculatingMode == V_Vector) {
  //   paddingVectorSize = utils::potolok(paddingVectorSize, 4) * 4;
  // }

  RaiHostAlloc<uint> hostVector(paddingVectorSize);
  for (size_t i = 0; i < paddingVectorSize; i++) {
    hostVector[i] = 0;
  }
  for (size_t i = 0; i < params.elements.size(); i++) {
    hostVector[i] = params.elements[i];
  }

  DirtyPairTime host2Dev, calc, dev2Host;

  RaiMalloc<uint> deviceSrcPtr(paddingVectorSize);

  RaiMalloc<uint> deviceDstPtr(paddingVectorSize);

  RaiHostAlloc<Descriptor> hostDescriptors(gridSize);
  for (size_t i = 0; i < gridSize; i++) {
    hostDescriptors[i] = Descriptor{};
  }
  RaiMalloc<Descriptor> deviceDescriptors(gridSize);

  checkErr(cudaEventRecord(host2Dev.before), "host2Dev");
  checkErr(
      cudaMemcpy(deviceSrcPtr, hostVector, sizeof(uint) * paddingVectorSize, cudaMemcpyKind::cudaMemcpyHostToDevice),
      "memcpy fst"
  );

  checkErr(
      cudaMemcpy(
          deviceDescriptors,
          hostDescriptors,
          sizeof(Descriptor) * gridSize,
          cudaMemcpyKind::cudaMemcpyHostToDevice
      ),
      "memcpy descriptors"
  );
  checkErr(cudaEventRecord(host2Dev.after), "host2Dev");

  dim3 grid(gridSize, 1, 1);

  // +1 element for exclusivePrefix
  size_t localMemSize = sizeof(uint) * (blockSize + gyperLibraryParameters.partition + 1);

  KernelBrentStoneParameters kernelParameters =
      {n, deviceDescriptors, gyperLibraryParameters.block.x, gyperLibraryParameters.partition};

  if (gyperLibraryParameters.calculatingMode == V_GlobalTransfer) {
    checkErr(cudaEventRecord(calc.before), "calc");
    kernelGlobal<<<grid, gyperLibraryParameters.block>>>(deviceSrcPtr, deviceDstPtr);
    checkErr(cudaEventRecord(calc.after), "calc");
  } else if (gyperLibraryParameters.calculatingMode == V_KoggeStone) {
    checkErr(cudaEventRecord(calc.before), "calc");
    KernelKoggeStoneParameters locKernelParameters = {n, deviceDescriptors, gyperLibraryParameters.block.x};

    kernelKoggeStone<<<grid, gyperLibraryParameters.block, localMemSize>>>(
        deviceSrcPtr,
        deviceDstPtr,
        locKernelParameters
    );
    checkErr(cudaEventRecord(calc.after), "calc");
  } else if (gyperLibraryParameters.calculatingMode == V_BrentStone) {
    validation(gyperLibraryParameters);

    checkErr(cudaEventRecord(calc.before), "calc");
    kernelBrentStone<<<grid, gyperLibraryParameters.block, localMemSize>>>(
        deviceSrcPtr,
        deviceDstPtr,
        kernelParameters
    );
    checkErr(cudaEventRecord(calc.after), "calc");
  } else if (gyperLibraryParameters.calculatingMode == V_Optimize) {
    validation(gyperLibraryParameters);

    checkErr(cudaEventRecord(calc.before), "calc");
    kernelOptimize<<<grid, gyperLibraryParameters.block, localMemSize>>>(deviceSrcPtr, deviceDstPtr, kernelParameters);
    checkErr(cudaEventRecord(calc.after), "calc");
  } else if (gyperLibraryParameters.calculatingMode == V_ReduceStone) {
    validation(gyperLibraryParameters);

    checkErr(cudaEventRecord(calc.before), "calc");
    kernelReduseStone<<<grid, gyperLibraryParameters.block, localMemSize>>>(
        deviceSrcPtr,
        deviceDstPtr,
        kernelParameters
    );
    checkErr(cudaEventRecord(calc.after), "calc");
  } else if (gyperLibraryParameters.calculatingMode == V_Bank) {
    validation(gyperLibraryParameters);

    checkErr(cudaEventRecord(calc.before), "calc");
    kernelBank<<<grid, gyperLibraryParameters.block, localMemSize>>>(deviceSrcPtr, deviceDstPtr, kernelParameters);
    checkErr(cudaEventRecord(calc.after), "calc");
  } else {
    throw errors::ValidationArgsError(
        std::to_string(gyperLibraryParameters.calculatingMode) + " is not calculating mode"
    );
  }

  // instant error
  cudaError_t executionError = cudaGetLastError();
  if (executionError != cudaSuccess) {
    throw errors::CudaRuntimeError("kernel launch error", executionError);
  }

  executionError = cudaDeviceSynchronize();
  if (executionError != cudaSuccess) {
    throw errors::CudaRuntimeError("kernel execution error", executionError);
  }

  RaiHostAlloc<uint> result(n);

  checkErr(cudaEventRecord(dev2Host.before), "dev2Host");
  checkErr(
      cudaMemcpy(result, deviceDstPtr, sizeof(uint) * n, cudaMemcpyKind::cudaMemcpyDeviceToHost),
      "memcpy dev2host"
  );

  checkErr(
      cudaMemcpy(
          hostDescriptors,
          deviceDescriptors,
          sizeof(Descriptor) * gridSize,
          cudaMemcpyKind::cudaMemcpyDeviceToHost
      ),
      "memcpy dev host"
  );
  checkErr(cudaEventRecord(dev2Host.after), "dev2Host");

  float tmRawCalc, tmHost2Dev, tmDev2Host;

  cudaEventElapsedTime(&tmHost2Dev, host2Dev.before, host2Dev.after);
  cudaEventElapsedTime(&tmRawCalc, calc.before, calc.after);
  cudaEventElapsedTime(&tmDev2Host, dev2Host.before, dev2Host.after);

  GpuTime gpuTime = {utils::ms2ns(tmRawCalc), utils::ms2ns(tmHost2Dev), utils::ms2ns(tmDev2Host)};

  std::vector<uint> vecResult(n);
  for (size_t i = 0; i < n; i++) {
    vecResult[i] = result[i];
  }

  std::vector<Descriptor> vecDescriptors(gridSize);
  for (size_t i = 0; i < gridSize; i++) {
    vecDescriptors[i] = hostDescriptors[i];
  }

  GpuResult gpuResult = GpuResult(vecResult, gpuTime, std::move(vecDescriptors));
  return gpuResult;
}

} // namespace prefsum
