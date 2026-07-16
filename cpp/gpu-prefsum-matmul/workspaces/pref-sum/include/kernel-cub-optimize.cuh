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

__global__ void kernelOptimize(uint* src, uint* dst, KernelBrentStoneParameters param) {
  size_t gx = blockIdx.x * blockDim.x + threadIdx.x;
  size_t lx = threadIdx.x;
  size_t locSize = blockDim.x;
  size_t groupIdx = blockIdx.x;

  // ==================
  // WITHIN GROUP
  // ==================

  // global -> local

  // locMem[lx] = src[gx];

  // low local calc
  size_t lowRange = locSize / param.partitioin;
  // uint val = locMem[lx];
  uint val = src[gx];
  uint localx = lx % lowRange;
  unsigned mask = __activemask();
  int width = lowRange;

  for (uint delta = 1; delta < lowRange; delta <<= 1) {
    uint cur = __shfl_up_sync(mask, val, delta, width);
    if (localx >= delta) {
      val += cur;
    }
  }

  // Записываем в shared memory только крайние элементы каждой lowRange
  if (localx == lowRange - 1) {
    locMem[lx] = val;
  }

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
  // Для экономии операций с shared memory: добавляем значение предыдущей границы в регистр `val`
  if (lx >= lowRange && (lx + 1) % lowRange != 0) {
    size_t neighbourIdx = (lx / lowRange) * lowRange - 1;
    val += locMem[neighbourIdx];
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
    // atomic::set(&param.descriptors[groupIdx].condition, A);
    param.descriptors[groupIdx].condition = Condition::A;
  }

  // one thread wolking for exclusive
  if (lx == 0) {
    uint exclusivePrefix = 0;
    size_t groupIdxNow = groupIdx;
    while (groupIdxNow != 0) {
      // Condition condition = atomic::read(&param.descriptors[groupIdxNow - 1].condition);
      Condition condition = static_cast<Condition>(param.descriptors[groupIdxNow - 1].condition);
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

  // inclusive: добавляем межгрупповой префикс к результату в регистре или в shared (для крайних)
  uint finalVal;
  if ((lx + 1) % lowRange == 0) {
    finalVal = locMem[lx] + locMem[locSize];
  } else {
    finalVal = val + locMem[locSize];
  }

  // answer, for future P-exclusive
  dst[gx] = finalVal;
  __threadfence();
  if (lx == 0) {
    // atomic::set(&param.descriptors[groupIdx].condition, P);
    param.descriptors[groupIdx].condition = Condition::P;
  }
}
