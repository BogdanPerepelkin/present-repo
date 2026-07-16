#include "kernel-utils.cl"


kernel void mul(__global MatrixType* srcA, __global MatrixType* srcB, __global MatrixType* dstC, DeviceInfo dims) {
  __local MatrixType aTile[TILE_SIZE_H][TILE_SIZE_K];
  __local MatrixType bTile[TILE_SIZE_K][TILE_SIZE_W];

  const uint GROUP_SHIFT_W = get_group_id(0) * TILE_SIZE_W;
  const uint GROUP_SHIFT_H = get_group_id(1) * TILE_SIZE_H;

  const uint localX = get_local_id(0);
  const uint localY = get_local_id(1);

  const uint LOCAL_SHIFT_A_H = localY * THREAD_RANGE_H;
  const uint LOCAL_SHIFT_B_W = localX * THREAD_RANGE_W;

  const uint LOCAL_SIZE_W = get_local_size(0);
  const uint LOCAL_SIZE_H = get_local_size(1);

  MatrixType singleSum[THREAD_RANGE_H][THREAD_RANGE_W] = {0};

  const uint TILES_PER_ELEMENT = potolok(dims.kDim, TILE_SIZE_K);
  for (uint curTileIdx = 0; curTileIdx < TILES_PER_ELEMENT; curTileIdx++) {
    const uint groupShiftK = curTileIdx * TILE_SIZE_K;
    // TILE A
    for (size_t y = localY; y < TILE_SIZE_H; y += LOCAL_SIZE_H) {
      const uint absY = GROUP_SHIFT_H + y;
      for (size_t x = localX; x < TILE_SIZE_K; x += LOCAL_SIZE_W) {
        const uint absX = groupShiftK + x;
        MatrixType val = 0;
        if (absX < dims.kDim && absY < dims.mDim) {
          val = srcA[idx(absX, absY, dims.kDim)];
        }
        aTile[y][x] = val;
      }
    }

    // TILE B
    for (uint y = localY; y < TILE_SIZE_K; y += LOCAL_SIZE_H) {
      const uint absY = groupShiftK + y;
      for (uint x = localX; x < TILE_SIZE_W; x += LOCAL_SIZE_W) {
        const uint absX = GROUP_SHIFT_W + x;
        MatrixType val = 0;
        if (absX < dims.nDim && absY < dims.kDim) {
          val = srcB[idx(absX, absY, dims.nDim)];
        }
        bTile[y][x] = val;
      }
    }

    barrier(CLK_LOCAL_MEM_FENCE);

    for (uint inTileIdx = 0; inTileIdx < TILE_SIZE_K; inTileIdx++) {
      for (uint y = 0; y < THREAD_RANGE_H; y++) {
        MatrixType aValue = aTile[LOCAL_SHIFT_A_H + y][inTileIdx];
        for (uint x = 0; x < THREAD_RANGE_W; x++) {
          singleSum[y][x] += aValue * bTile[inTileIdx][LOCAL_SHIFT_B_W + x];
        }
      }
    }

    barrier(CLK_LOCAL_MEM_FENCE);
  }

  for (uint y = 0; y < THREAD_RANGE_H; y++) {
    const uint absY = GROUP_SHIFT_H + LOCAL_SHIFT_A_H + y;
    if (absY >= dims.mDim) {
      break;
    }
    for (uint x = 0; x < THREAD_RANGE_W; x++) {
      const uint absX = GROUP_SHIFT_W + LOCAL_SHIFT_B_W + x;
      if (absX >= dims.nDim) {
        break;
      }

      dstC[idx(absX, absY, dims.nDim)] = singleSum[y][x];
    }
  }
}
