#include "kernel-utils.cl"

kernel void mul(__global MatrixType* a, __global MatrixType* b, __global MatrixType* c, DeviceInfo dims) {

  __local MatrixType aTile[TILE_SIZE][TILE_SIZE];
  __local MatrixType bTile[TILE_SIZE][TILE_SIZE];

  for (size_t artificialCnt = 0; artificialCnt < dims.artificialFactorIterations; ++artificialCnt) {
    uint groupX = get_group_id(0);
    uint groupY = get_group_id(1);

    uint lX = get_local_id(0);
    uint lY = get_local_id(1);

    uint absRealX = lX + groupX * TILE_SIZE;
    uint absRealY = lY + groupY * TILE_SIZE;

    int isInC = (absRealX < dims.nDim) && (absRealY < dims.mDim);

    MatrixType singleSum = 0;
    const uint tilesPerElement = dims.kDim / TILE_SIZE + (dims.kDim % TILE_SIZE != 0 ? 1 : 0);
    for (uint i = 0; i < tilesPerElement; i++) {
      // load values
      uint curRealAX = lX + i * TILE_SIZE;
      uint curRealBY = lY + i * TILE_SIZE;

      int isLast = i == tilesPerElement - 1;

      MatrixType value = 0;
      if (!isLast || !(curRealAX >= dims.kDim || absRealY >= dims.mDim)) {
        value = a[idx(curRealAX, absRealY, dims.kDim)];
      }
      aTile[lX][lY] = value;

      value = 0;
      if (!isLast || !(absRealX >= dims.nDim || curRealBY >= dims.kDim)) {
        value = b[idx(absRealX, curRealBY, dims.nDim)];
      }
      bTile[lX][lY] = value;

      barrier(CLK_LOCAL_MEM_FENCE);

      // count part, save in register
      for (uint j = 0; j < TILE_SIZE; j++) {
        if (!(i * TILE_SIZE + j >= dims.kDim)) {
          singleSum += aTile[j][lY] * bTile[lX][j];
        }
      }

      barrier(CLK_LOCAL_MEM_FENCE);
    }
    if (isInC) {
      c[idx(absRealX, absRealY, dims.nDim)] = singleSum;
    }
  }
}
