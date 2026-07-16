/*
global - single-pass adaptive look-back (cub)
local - kogge-stone
*/

#pragma once

#ifdef __APPLE__
#include "device_atomic_functions.h"
#include "device_functions.h"
#include "host_defines.h"
#endif

#include "../include/kernel-utils.cuh"

extern __shared__ uint locMem[];

__global__ void kernelKoggeStone(uint* src, uint* dst, KernelKoggeStoneParameters param) {
  size_t gx = blockIdx.x * blockDim.x + threadIdx.x;
  size_t lx = threadIdx.x;
  size_t locSize = blockDim.x;
  size_t groupIdx = blockIdx.x;

  // ==================
  // WITHIN GROUP
  // ==================

  // global -> local
  locMem[lx] = src[gx];
  __syncthreads();

  // kogge-stone
  size_t operationsPerThread = ceilLog2(lx + 1) + 1;
  size_t maxOperations = ceilLog2(locSize) + 1;
  for (size_t localEpoch = 1; localEpoch < maxOperations; localEpoch++) {
    uint cur;
    uint left;
    bool active = localEpoch < operationsPerThread;
    if (active) {
      size_t backStep = pow(2, localEpoch - 1);
      cur = locMem[lx];
      left = locMem[lx - backStep];
    }
    __syncthreads();
    if (active) {
      locMem[lx] = cur + left;
    }
    __syncthreads();
  }

  // ==================
  // INTERGROUP
  // ==================

  // for future A-exclusive
  if (lx == locSize - 1) {
    src[gx] = locMem[lx];
  }
  __threadfence();
  __syncthreads();
  if (lx == 0) {
    atomic::set(&param.descriptors[groupIdx].condition, A);
  }
  // __syncthreads();

  // one thread wolking for exclusive
  if (lx == 0) {
    uint exclusivePrefix = 0;
    size_t groupIdxNow = groupIdx;
    while (groupIdxNow != 0) {
      Condition condition = atomic::read(&param.descriptors[groupIdxNow - 1].condition);
      __threadfence();

      size_t lastInNeighbour = (groupIdxNow - 1) * locSize + locSize - 1;
      if (condition == P) {
        exclusivePrefix += dst[lastInNeighbour];
        break;
      } else if (condition == A) {
        exclusivePrefix += src[lastInNeighbour];
        groupIdxNow--;
      }
      // spin!
    }
    locMem[locSize] = exclusivePrefix;
  }
  __syncthreads();

  // inclusive
  locMem[lx] += locMem[locSize];

  // answer, for future P-exclusive
  dst[gx] = locMem[lx];
  __threadfence();
  // __syncthreads();
  if (lx == 0) {
    atomic::set(&param.descriptors[groupIdx].condition, P);
  }
  // __syncthreads();
}
