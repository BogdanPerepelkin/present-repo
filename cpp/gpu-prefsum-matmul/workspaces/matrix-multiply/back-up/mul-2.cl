// #include "utils.cl"

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
#ifndef UNROLL
#define UNROLL 0
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
  // для B локалка TS_W * TS_W, а для A TS_W * TS_H
  // для B работает TR_W * TR_W потоков, а для A TR_W * TR_H

  const size_t edgeWidth = myMax(THREAD_RANGE_W, deltaX);
  const size_t edgeHigh = myMax(THREAD_RANGE_H, deltaY);
#ifdef UNROLL
#pragma nounroll
#endif
  for (uint curTileIdx = 0; curTileIdx < tilesPerElement; curTileIdx++) {
    // ============================================================================================================

    // #ifdef UNROLL
    // #pragma nounroll
    // #endif
    //     for (uint inThreadX = 0; inThreadX < edgeWidth; inThreadX++) {
    //       const uint inTileShiftAX = localShiftAW + inThreadX;
    //       const uint inTileShiftBX = localShiftBW + inThreadX;
    // #ifdef UNROLL
    // #pragma nounroll
    // #endif
    //       for (uint inThreadY = 0; inThreadY < edgeHigh; inThreadY++) {
    //         const uint inTileShiftAY = localShiftAH + inThreadY;
    //         const uint inTileShiftBY = localShiftBH + inThreadY;

    // const uint absCurAX = curTileIdx * TILE_SIZE_W + inTileShiftAX;
    // const uint absCurAY = groupShiftH + inTileShiftAY;

    // // A
    // if (inTileShiftAX < TILE_SIZE_W && inTileShiftAY < TILE_SIZE_H) {
    //   MatrixType val = 0;
    //   if (!(absCurAX >= dims.kDim || absCurAY >= dims.mDim)) {
    //     val = srcA[idx(absCurAX, absCurAY, dims.kDim)];
    //   }
    //   aTile[inTileShiftAX][inTileShiftAY] = val;
    // }

    // const uint absCurBX = groupShiftW + inTileShiftBX;
    // const uint absCurBY = curTileIdx * TILE_SIZE_W + inTileShiftBY;

    // // B
    // if (inTileShiftBX < TILE_SIZE_W && inTileShiftBY < TILE_SIZE_W) {
    //   MatrixType val = 0;
    //   if (!(absCurBX >= dims.nDim || absCurBY >= dims.kDim)) {
    //     val = srcB[idx(absCurBX, absCurBY, dims.nDim)];
    //   }
    //   bTile[inTileShiftBX][inTileShiftBY] = val;
    // }
    // }
    // }

    // ============================================================================================================

#ifdef UNROLL
#pragma nounroll
#endif
    // TILE A
    for (uint inThreadX = 0; inThreadX < THREAD_RANGE_W; inThreadX++) {
      const uint inTileShiftX = localShiftAW + inThreadX;
      if (inTileShiftX >= TILE_SIZE_W) {
        continue;
      }
#ifdef UNROLL
#pragma nounroll
#endif
      for (uint inThreadY = 0; inThreadY < THREAD_RANGE_H; inThreadY++) {
        const uint inTileShiftY = localShiftAH + inThreadY;
        if (inTileShiftY >= TILE_SIZE_H) {
          break;
        }

        const uint absCurAX = curTileIdx * TILE_SIZE_W + inTileShiftX;
        const uint absCurAY = groupShiftH + inTileShiftY;

        if (!(absCurAX >= dims.kDim || absCurAY >= dims.mDim)) {
          aTile[inTileShiftX][inTileShiftY] = srcA[idx(absCurAX, absCurAY, dims.kDim)];
        }
      }
    }

#ifdef UNROLL
#pragma nounroll
#endif
    // TILE B
    for (size_t inThreadX = 0; inThreadX < deltaX; inThreadX++) {
      const uint inTileShiftX = localShiftBW + inThreadX;
      if (inTileShiftX >= TILE_SIZE_W) {
        continue;
      }
#ifdef UNROLL
#pragma nounroll
#endif
      for (size_t inThreadY = 0; inThreadY < deltaY; inThreadY++) {
        const uint inTileShiftY = localShiftBH + inThreadY;
        if (inTileShiftY >= TILE_SIZE_W) {
          break;
        }

        const uint absCurBX = groupShiftW + inTileShiftX;
        const uint absCurBY = curTileIdx * TILE_SIZE_W + inTileShiftY;

        if (!(absCurBX >= dims.nDim || absCurBY >= dims.kDim)) {
          bTile[inTileShiftX][inTileShiftY] = srcB[idx(absCurBX, absCurBY, dims.nDim)];
        }
      }
    }

    barrier(CLK_LOCAL_MEM_FENCE);

#ifdef UNROLL
#pragma nounroll
#endif
    for (uint inThreadX = 0; inThreadX < THREAD_RANGE_W; inThreadX++) {
      const uint inTileShiftX = localShiftAW + inThreadX;
      if (inTileShiftX >= TILE_SIZE_W) {
        continue;
      }
#ifdef UNROLL
#pragma nounroll
#endif
      for (uint inThreadY = 0; inThreadY < THREAD_RANGE_H; inThreadY++) { // H
        const uint inTileShiftY = localShiftAH + inThreadY;
        if (inTileShiftY >= TILE_SIZE_H) {
          break;
        }

        for (uint inTileIdx = 0; inTileIdx < TILE_SIZE_W; inTileIdx++) {
          singleSum[inThreadX][inThreadY] += aTile[inTileIdx][inTileShiftY] * bTile[inTileShiftX][inTileIdx];
        }
      }
    }

    barrier(CLK_LOCAL_MEM_FENCE);
  }

#ifdef UNROLL
#pragma nounroll
#endif
  for (uint inThreadX = 0; inThreadX < THREAD_RANGE_W; inThreadX++) {
    const uint inTileShiftX = localShiftAW + inThreadX;
    if (inTileShiftX >= TILE_SIZE_W) {
      continue;
    }
    const uint absX = groupShiftW + inTileShiftX;
    if (absX >= dims.nDim) {
      continue;
    }
#ifdef UNROLL
#pragma nounroll
#endif
    for (uint inThreadY = 0; inThreadY < THREAD_RANGE_H; inThreadY++) {
      const uint inTileShiftY = localShiftAH + inThreadY;
      if (inTileShiftY >= TILE_SIZE_H) {
        break;
      }

      const uint absY = groupShiftH + inTileShiftY;
      if (absY >= dims.mDim) {
        break;
      }

      dstC[idx(absX, absY, dims.nDim)] = singleSum[inThreadX][inThreadY];
    }
  }
}
