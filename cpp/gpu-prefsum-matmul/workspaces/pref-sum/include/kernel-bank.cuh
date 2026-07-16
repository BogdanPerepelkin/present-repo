/*
global - single-pass adaptive look-back (cub)
local -  Brent-Kung scan-then-propagate strategy embedding Kogge-Stone warpscans and propagation fans

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

__global__ void kernelBank(uint* src, uint* dst, KernelBrentStoneParameters param) {
  size_t gx = blockIdx.x * blockDim.x + threadIdx.x;
  size_t lx = threadIdx.x;
  size_t locSize = blockDim.x;
  size_t groupIdx = blockIdx.x;

  const int PADDING = 1;

  size_t lowRange = locSize / param.partitioin;
  size_t stride = lowRange + PADDING;
  size_t part = lx / lowRange;
  uint localX = lx % lowRange;
  size_t idx = part * stride + localX;

  // ==================
  // WITHIN GROUP
  // ==================

  // global -> local (with padded layout)
  locMem[idx] = src[gx];

  // low local calc
  uint val = locMem[idx];
  // uint localx = off;
  unsigned mask = __activemask();
  int width = lowRange;

  for (uint delta = 1; delta < lowRange; delta <<= 1) {
    uint cur = __shfl_up_sync(mask, val, delta, width);
    if (localX >= delta) {
      val += cur;
    }
  }

  locMem[idx] = val;

  __syncthreads(); // до сюда рано дойдут лишь немногие, и доходить будут только варпами, потому что есть __shfl_up_sync

  // пишем в краешек (1 варп работает)
  width = param.partitioin;

  if (lx < warpSize) {
    size_t shiftedIdx = lx * stride + lowRange - 1;
    uint val = 0;
    if (lx < param.partitioin) {
      val = locMem[shiftedIdx];
    }
    for (uint delta = 1; delta <= param.partitioin; delta <<= 1) {
      uint cur = __shfl_up_sync(mask, val, delta, width);
      if (lx >= delta && lx < param.partitioin) {
        val += cur;
      }
    }

    if (lx < param.partitioin) {
      locMem[shiftedIdx] = val;
    }
  }

  __syncthreads();

  // propogate (вертуханы)
  if (lx >= lowRange && (lx + 1) % lowRange != 0) {
    size_t partIdx = lx / lowRange;
    if (partIdx > 0) {
      size_t neighbourIdx = (partIdx - 1) * stride + (lowRange - 1);
      locMem[idx] += locMem[neighbourIdx];
    }
  }

  // ==================
  // INTERGROUP
  // ==================

  // for future A-exclusive
  if (lx == locSize - 1) {
    // last element of last partition
    size_t lastIdx = (param.partitioin - 1) * stride + (lowRange - 1);
    src[gx] = locMem[lastIdx];
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
    // store exclusive prefix in the reserved slot after partitions
    size_t exclusiveIdx = param.partitioin * stride;
    locMem[exclusiveIdx] = exclusivePrefix;
  }
  __syncthreads();

  // inclusive
  locMem[idx] += locMem[param.partitioin * stride];

  // answer, for future P-exclusive
  dst[gx] = locMem[idx];
  __threadfence();
  if (lx == 0) {
    atomic::set(&param.descriptors[groupIdx].condition, P);
  }
}
