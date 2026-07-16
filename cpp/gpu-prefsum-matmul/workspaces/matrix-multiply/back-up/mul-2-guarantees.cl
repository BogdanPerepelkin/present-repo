// Guarantees:
// 0. tsw >= tsh && tsw % trw == 0 && tsh % trh == 0 && tsw % localY == 0 && tsw % localX == 0

#ifndef TILE_SIZE_W
#define TILE_SIZE_W 1
#endif

#ifndef TILE_SIZE_H
#define TILE_SIZE_H 1
#endif

#ifndef THREAD_RANGE_W
#define THREAD_RANGE_W 1
#endif

#ifndef THREAD_RANGE_H
#define THREAD_RANGE_H 1
#endif

#ifndef LOCAL_GR_SIZE_W
#define LOCAL_GR_SIZE_W 1
#endif

#ifndef LOCAL_GR_SIZE_H
#define LOCAL_GR_SIZE_H 1
#endif

// optimizations
#ifndef NOUNROLL
#define NOUNROLL 0
#endif

typedef float MatrixType;

typedef struct {
  uint mDim;
  uint nDim;
  uint kDim;
  uint artificialFactorIterations;
} DeviceInfo;

inline uint idx(const uint x, const uint y, const uint width) {
  return y * width + x;
}

inline uint potolok(const uint a, const uint b) {
  return a / b + (a % b != 0 ? 1 : 0);
}

inline size_t myMax(const size_t a, const size_t b) {
  return a > b ? a : b;
}

kernel void mul(__global MatrixType* srcA, __global MatrixType* srcB, __global MatrixType* dstC, DeviceInfo dims) {
  __local MatrixType aTile[TILE_SIZE_W][TILE_SIZE_H]; // маленький прямоугольничек
  __local MatrixType bTile[TILE_SIZE_W][TILE_SIZE_W]; // большой квадратик

  const uint groupX = get_group_id(0);
  const uint groupY = get_group_id(1);

  const uint groupShiftW = groupX * TILE_SIZE_W;
  const uint groupShiftH = groupY * TILE_SIZE_H;

  const uint localX = get_local_id(0);
  const uint localY = get_local_id(1);

  const uint localShiftAW = localX * THREAD_RANGE_W;
  const uint localShiftAH = localY * THREAD_RANGE_H;

  const uint deltaX = potolok(TILE_SIZE_W, LOCAL_GR_SIZE_H);
  const uint deltaY = potolok(TILE_SIZE_W, LOCAL_GR_SIZE_W);

  const uint localShiftBW = localY * deltaX;
  const uint localShiftBH = localX * deltaY;

  MatrixType singleSum[THREAD_RANGE_W][THREAD_RANGE_H] = {0};

  const uint tilesPerElement = dims.kDim / TILE_SIZE_W + (dims.kDim % TILE_SIZE_W != 0 ? 1 : 0);

#if NOUNROLL == 1
#pragma nounroll
#endif
  for (uint curTileIdx = 0; curTileIdx < tilesPerElement; curTileIdx++) {
#if NOUNROLL == 1
#pragma nounroll
#endif
    // TILE A
    for (uint inThreadX = 0; inThreadX < THREAD_RANGE_W; inThreadX++) {
      const uint inTileShiftX = localShiftAW + inThreadX;
#if NOUNROLL == 1
#pragma nounroll
#endif
      for (uint inThreadY = 0; inThreadY < THREAD_RANGE_H; inThreadY++) {
        const uint inTileShiftY = localShiftAH + inThreadY;

        const uint absCurAX = curTileIdx * TILE_SIZE_W + inTileShiftX;
        const uint absCurAY = groupShiftH + inTileShiftY;

        MatrixType value = 0;
        if (!(absCurAX >= dims.kDim || absCurAY >= dims.mDim)) {
          value = srcA[idx(absCurAX, absCurAY, dims.kDim)];
        }
        aTile[inTileShiftX][inTileShiftY] = value;
      }
    }

#if NOUNROLL == 1
#pragma nounroll
#endif
    // TILE B
    for (size_t inThreadX = 0; inThreadX < deltaX; inThreadX++) {
      const uint inTileShiftX = localShiftBW + inThreadX;
#if NOUNROLL == 1
#pragma nounroll
#endif
      for (size_t inThreadY = 0; inThreadY < deltaY; inThreadY++) {
        const uint inTileShiftY = localShiftBH + inThreadY;

        const uint absCurBX = groupShiftW + inTileShiftX;
        const uint absCurBY = curTileIdx * TILE_SIZE_W + inTileShiftY;

        MatrixType value = 0;
        if (!(absCurBX >= dims.nDim || absCurBY >= dims.kDim)) {
          value = srcB[idx(absCurBX, absCurBY, dims.nDim)];
        }
        bTile[inTileShiftX][inTileShiftY] = value;
      }
    }

    barrier(CLK_LOCAL_MEM_FENCE);

#if NOUNROLL == 1
#pragma nounroll
#endif
    for (uint inTileIdx = 0; inTileIdx < TILE_SIZE_W; inTileIdx++) {
#if NOUNROLL == 1
#pragma nounroll
#endif
      for (uint inThreadY = 0; inThreadY < THREAD_RANGE_H; inThreadY++) {
        const uint inTileShiftY = localShiftAH + inThreadY;
        const MatrixType aValue = aTile[inTileIdx][inTileShiftY];
#if NOUNROLL == 1
#pragma nounroll
#endif
        for (uint inThreadX = 0; inThreadX < THREAD_RANGE_W; inThreadX++) {
          const uint inTileShiftX = localShiftAW + inThreadX;

          singleSum[inThreadX][inThreadY] += aValue * bTile[inTileShiftX][inTileIdx];
        }
      }
    }

    barrier(CLK_LOCAL_MEM_FENCE);
  }

#if NOUNROLL == 1
#pragma nounroll
#endif
  for (uint inThreadX = 0; inThreadX < THREAD_RANGE_W; inThreadX++) {
    const uint inTileShiftX = localShiftAW + inThreadX;
    const uint absX = groupShiftW + inTileShiftX;
    if (absX >= dims.nDim) {
      continue;
    }
#if NOUNROLL == 1
#pragma nounroll
#endif
    for (uint inThreadY = 0; inThreadY < THREAD_RANGE_H; inThreadY++) {
      const uint inTileShiftY = localShiftAH + inThreadY;

      const uint absY = groupShiftH + inTileShiftY;
      if (absY >= dims.mDim) {
        break;
      }

      dstC[idx(absX, absY, dims.nDim)] = singleSum[inThreadX][inThreadY];
    }
  }
}
