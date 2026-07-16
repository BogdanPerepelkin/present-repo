# 1 "C:\\Users\\SKKV_S~1\\AppData\\Local\\Temp\\comgr-3142a1\\input\\CompileSource"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 404 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Users\\SKKV_S~1\\AppData\\Local\\Temp\\comgr-3142a1\\input\\CompileSource" 2
# 49 "C:\\Users\\SKKV_S~1\\AppData\\Local\\Temp\\comgr-3142a1\\input\\CompileSource"
typedef float2 FLN;

typedef float MatrixType;

typedef union {
  FLN fln;
  MatrixType arr[2];
} FLNUnion;






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

__attribute__((reqd_work_group_size(16, 2, 1))) kernel void
mul(__global MatrixType* srcA, __global MatrixType* srcB, __global MatrixType* dstC, DeviceInfo dims) {
  const uint VECTOR_THREAD_RANGE_W = 6 / 2;

  __local FLN aTile[16][(66 / 2)];
  __local FLN bTile[(66 / 2)][2][(96 / 2)];

  const uint GROUP_SHIFT_W = get_group_id(0) * 96;
  const uint GROUP_SHIFT_H = get_group_id(1) * 16;
  const uint GROUP_SHIFT_WV = get_group_id(0) * (96 / 2);

  const uint localX = get_local_id(0);
  const uint localY = get_local_id(1);

  const uint LOCAL_SHIFT_A_H = localY * 8;
  const uint LOCAL_SHIFT_B_WV = localX * VECTOR_THREAD_RANGE_W;

  const uint LOCAL_SIZE_W = get_local_size(0);
  const uint LOCAL_SIZE_H = get_local_size(1);
  const uint THREADS_IN_GROUP = LOCAL_SIZE_W * LOCAL_SIZE_H;
  const uint THREAD_ID = localY * LOCAL_SIZE_W + localX;

  const uint kDimVec = dims.kDim / 2;
  const uint nDimVec = dims.nDim / 2;

  const __global FLN* vecSrcA = (const __global FLN*) srcA;
  const __global FLN* vecSrcB = (const __global FLN*) srcB;
  __global FLN* vecDstC = (__global FLN*) dstC;

  FLN singleSum[8][VECTOR_THREAD_RANGE_W];
  for (uint y = 0; y < 8; y++) {
    for (uint w = 0; w < VECTOR_THREAD_RANGE_W; w++) {
      singleSum[y][w] = (FLN) (0);
    }
  }

  const uint TILES_PER_ELEMENT = potolok(dims.kDim, 66);
  for (uint curTileIdx = 0; curTileIdx < TILES_PER_ELEMENT; curTileIdx++) {
    const uint groupShiftK = curTileIdx * 66;


    const uint groupShiftKV = curTileIdx * (66 / 2);
    for (uint y = 0; y < 16; y++) {
      const uint absY = GROUP_SHIFT_H + y;
      for (uint xVec = THREAD_ID; xVec < (66 / 2); xVec += THREADS_IN_GROUP) {
        const uint absX = groupShiftKV + xVec;
        FLN packed = (FLN) (0);
        if (absY < dims.mDim && absX < kDimVec) {
          packed = vecSrcA[idx(absX, absY, kDimVec)];
        }
        aTile[y][xVec] = packed;
      }
    }
# 148 "C:\\Users\\SKKV_S~1\\AppData\\Local\\Temp\\comgr-3142a1\\input\\CompileSource"
    for (uint kVec = 0; kVec < (66 / 2); kVec++) {
      for (uint lane = 0; lane < 2; lane++) {
        const uint absY = groupShiftK + kVec * 2 + lane;
        for (uint xVec = THREAD_ID; xVec < (96 / 2); xVec += THREADS_IN_GROUP) {
          const uint absX = GROUP_SHIFT_WV + xVec;
          FLN packed = (FLN) (0);
          if (absY < dims.kDim && absX < nDimVec) {
            packed = vecSrcB[idx(absX, absY, nDimVec)];
          }
          bTile[kVec][lane][xVec] = packed;
        }
      }
    }

    barrier(CLK_LOCAL_MEM_FENCE);

    for (uint kVec = 0; kVec < (66 / 2); kVec++) {
      for (uint y = 0; y < 8; y++) {
        FLNUnion aPacked;
        aPacked.fln = aTile[LOCAL_SHIFT_A_H + y][kVec];
        for (uint lane = 0; lane < 2; lane++) {
          const MatrixType aValue = aPacked.arr[lane];
          for (uint wv = 0; wv < VECTOR_THREAD_RANGE_W; wv++) {
            singleSum[y][wv] += aValue * bTile[kVec][lane][LOCAL_SHIFT_B_WV + wv];
          }
        }
      }
    }

    barrier(CLK_LOCAL_MEM_FENCE);
  }

  for (uint y = 0; y < 8; y++) {
    const uint absY = GROUP_SHIFT_H + LOCAL_SHIFT_A_H + y;
    for (uint wv = 0; wv < VECTOR_THREAD_RANGE_W; wv++) {
      const uint absXVec = GROUP_SHIFT_WV + LOCAL_SHIFT_B_WV + wv;
      if (absY < dims.mDim && absXVec < nDimVec) {
        vecDstC[idx(absXVec, absY, nDimVec)] = singleSum[y][wv];
      }
    }
  }
}
