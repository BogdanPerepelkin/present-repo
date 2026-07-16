#include "kernel-utils.cl"

kernel void mul(const __global MatrixType* a, const __global MatrixType* b, __global MatrixType* c, DeviceInfo dims) {
  __local MatrixType aTile[TILE_SIZE * TILE_SIZE];
  __local MatrixType bTile[TILE_SIZE * TILE_SIZE];

  for (size_t artificial = 0; artificial < dims.artificialFactorIterations; artificial++) {
    uint groupX = get_group_id(0);
    uint groupY = get_group_id(1);

    uint threadIdx = get_local_id(0);

    uint absRealY = threadIdx + groupY * TILE_SIZE;

    MatrixType singleSum[SINGLE_THREAD_RANGE];

    const uint tilesPerElement = dims.kDim / TILE_SIZE + (dims.kDim % TILE_SIZE != 0 ? 1 : 0);
    for (uint tileIdx = 0; tileIdx < tilesPerElement; tileIdx++) {
      // read
      uint curRealBY = threadIdx + tileIdx * TILE_SIZE;

      for (uint inThreadIdx = 0; inThreadIdx < SINGLE_THREAD_RANGE; inThreadIdx++) {
        uint absRealX = inThreadIdx + groupX * TILE_SIZE;

        uint curRealAX = inThreadIdx + tileIdx * TILE_SIZE;
        int isLast = tileIdx == tilesPerElement - 1;

        tryWrite(
            a,
            curRealAX,
            absRealY,
            dims.kDim,
            curRealAX >= dims.kDim,
            absRealY >= dims.mDim,
            aTile,
            inThreadIdx,
            threadIdx,
            isLast
        );

        tryWrite(
            b,
            absRealX,
            curRealBY,
            dims.nDim,
            absRealX >= dims.nDim,
            curRealBY >= dims.kDim,
            bTile,
            inThreadIdx,
            threadIdx,
            isLast
        );
      }

      barrier(CLK_LOCAL_MEM_FENCE);

      // todo только для S_T_R = T_S
      // calculate
      for (uint inThreadIdx = 0; inThreadIdx < SINGLE_THREAD_RANGE; inThreadIdx++) {
        for (uint curTileIdx = 0; curTileIdx < TILE_SIZE; curTileIdx++) {
          if (!(tileIdx * TILE_SIZE + curTileIdx >= dims.kDim)) {
            singleSum[inThreadIdx] +=
                aTile[idx(curTileIdx, threadIdx, TILE_SIZE)] * bTile[idx(inThreadIdx, curTileIdx, TILE_SIZE)];
          }
        }
      }

      barrier(CLK_LOCAL_MEM_FENCE);
    }

    for (uint inThreadIdx = 0; inThreadIdx < SINGLE_THREAD_RANGE; inThreadIdx++) {
      uint absRealX = inThreadIdx + groupX * TILE_SIZE;
      int isInC = (absRealX < dims.nDim) && (absRealY < dims.mDim);
      if (isInC) {
        c[idx(absRealX, absRealY, dims.nDim)] = singleSum[inThreadIdx];
      }
    }
  }
}
