/*
global - single-pass adaptive look-back (cub)
local -   A raking radix-4 reduce-then-scan strategy embedding serial reductions, serial scans, and a Kogge-Stone warpscan at the root

Contract:
locSize / param.partition <= 32
*/

#pragma once

#ifdef __APPLE__
#include "device_atomic_functions.h"
#include "device_functions.h"
#include "host_defines.h"
#endif

#include "../include/kernel-utils.cuh"

extern __shared__ uint locMem[];

__global__ void kernelReduseStone(uint* src, uint* dst, KernelBrentStoneParameters param) {
  size_t gx = blockIdx.x * blockDim.x + threadIdx.x;
  size_t lx = threadIdx.x;
  size_t locSize = blockDim.x;
  size_t groupIdx = blockIdx.x;

  // ==================
  // WITHIN GROUP
  // ==================

  // global -> local
  uint tmp = src[gx];
  locMem[lx] = tmp;

  // low local calc
  size_t lowRange = locSize / param.partitioin;
  uint val = tmp;
  uint localx = lx % lowRange;
  unsigned mask = __activemask();
  int width = lowRange;

  for (uint i = 1; i < lowRange; i++) {
    uint cur = __shfl_up_sync(mask, val, 1, width);
    if (localx == i) {
      val += cur;
    }
  }

  locMem[lx] = val;

  __syncthreads(); // до сюда рано дойдут лишь немногие, и доходить будут только варпами, потому что есть __shfl_up_sync

  // пишем в краешек (1 варп работает)
  width = param.partitioin;

  if (lx < warpSize) {
    size_t shiftedLx = lx * lowRange + lowRange - 1;
    uint val = 0;
    if (lx < param.partitioin) {
      val = locMem[shiftedLx];
    }
    for (uint delta = 1; delta <= param.partitioin; delta <<= 1) {
      uint cur = __shfl_up_sync(mask, val, delta, width);
      if (lx >= delta && lx < param.partitioin) {
        val += cur;
      }
    }

    if (lx < param.partitioin) {
      locMem[shiftedLx] = val;
    }
  }

  __syncthreads();

  // propogate (вертуханы)
  if (lx >= lowRange && (lx + 1) % lowRange != 0) {
    size_t neighbourIdx = (lx / lowRange) * lowRange - 1;
    locMem[lx] += locMem[neighbourIdx];
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
  if (lx == 0) {
    atomic::set(&param.descriptors[groupIdx].condition, P);
  }
}
