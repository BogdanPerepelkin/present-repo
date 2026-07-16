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

__global__ void kernelGlobal(uint* src, uint* dst) {
  size_t gx = blockIdx.x * blockDim.x + threadIdx.x;
  size_t lx = threadIdx.x;
  size_t locSize = blockDim.x;
  size_t groupIdx = blockIdx.x;

  uint reg = src[gx];

  dst[gx] = reg;
}
