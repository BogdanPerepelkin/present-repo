#pragma once

#ifdef __APPLE__
#include "driver_types.h"
#endif

#include "../include/descriptor-types.h"

#include <cuda_runtime.h>

#include <cstddef>
#include <cstdint>
#include <vector>

using uint = unsigned int;
using ull = unsigned long long;

namespace prefsum {

enum GPUCalculatingMode : int64_t {
  V_GlobalTransfer,
  V_KoggeStone,
  V_BrentStone,
  V_ReduceStone,
  V_Bank,
  V_Optimize,
};

struct LibInitializeParameters {
  size_t deviceIndex;
  size_t mapDevType2Priority[4];
};

struct GyperParametersLibrary {
  // non zero if choosen many devices (not in this lab, but optional)
  size_t deviceIndex;

  bool enableCpuProfiling = false;

  GPUCalculatingMode calculatingMode;

  dim3 block;

  size_t partition;
};

struct CalculateParameters {
  std::vector<uint> elements;
};

struct CpuResult {
  CpuResult(const std::vector<uint>& elements, ull time);

  std::vector<uint> _elements;
  ull _time;
};

struct GpuTime { // ns
  ull rawCalc;
  ull host2Dev;
  ull dev2Host;
};

struct GpuResult {
  GpuResult(const std::vector<uint>& elements, GpuTime time, const std::vector<Descriptor>& descriptors);

  GpuResult(const std::vector<uint>& elements, GpuTime time, std::vector<Descriptor>&& descriptors);

  std::vector<uint> _elements;
  GpuTime _time;
  std::vector<Descriptor> _descriptors;
};

class PrefSum {
public:
  GyperParametersLibrary gyperLibraryParameters;
  LibInitializeParameters ini;

  PrefSum();

  void initialize();

  GpuResult gpuCalculate(const CalculateParameters&);

  CpuResult cpuCalculate(const CalculateParameters&);

private:
  std::vector<cudaDeviceProp> _properties;
};

} // namespace prefsum
