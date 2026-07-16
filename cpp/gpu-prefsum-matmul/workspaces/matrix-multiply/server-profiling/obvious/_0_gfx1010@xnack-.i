# 1 "C:\\Users\\SKKV_S~1\\AppData\\Local\\Temp\\comgr-aed740\\input\\CompileSource"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 404 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Users\\SKKV_S~1\\AppData\\Local\\Temp\\comgr-aed740\\input\\CompileSource" 2
# 27 "C:\\Users\\SKKV_S~1\\AppData\\Local\\Temp\\comgr-aed740\\input\\CompileSource"
typedef float MatrixType;

typedef struct {
  uint mDim;
  uint nDim;
  uint kDim;
  uint artificialFactorIterations;
} DeviceInfo;

uint idx(uint x, uint y, uint width) {
  return y * width + x;
}

void tryWrite(
    __global MatrixType* from,
    uint srcX,
    uint srcY,
    uint wide,
    __local MatrixType* to,
    uint lX,
    uint lY,
    uint localWidth,
    bool isOOBTileX,
    bool isOOBTileY,
    bool isOOBAbsX,
    bool isOOBAbsY
) {
  if (!(isOOBTileX || isOOBTileY)) {
    MatrixType value = 0;
    if (!(isOOBAbsX || isOOBAbsY)) {
      value = from[idx(srcX, srcY, wide)];
    }
    to[idx(lX, lY, localWidth)] = value;
  }
}

uint potolok(uint a, uint b) {
  return a / b + (a % b != 0 ? 1 : 0);
}

kernel void mul(__global MatrixType* a, __global MatrixType* b, __global MatrixType* c, DeviceInfo dims) {
  __local MatrixType aTile[64 * 16];
  __local MatrixType bTile[64 * 64];

  for (size_t artificial = 0; artificial < dims.artificialFactorIterations; artificial++) {
    uint groupX = get_group_id(0);
    uint groupY = get_group_id(1);

    uint localX = get_local_id(0);
    uint localY = get_local_id(1);

    MatrixType singleSum[32 * 2] = {0};

    const uint tilesPerElement = dims.kDim / 64 + (dims.kDim % 64 != 0 ? 1 : 0);


    for (uint curTileIdx = 0; curTileIdx < tilesPerElement; curTileIdx++) {
      for (uint inThreadX = 0; inThreadX < 32; inThreadX++) {
        for (uint inThreadY = 0; inThreadY < 2; inThreadY++) {
          uint inTileShiftX = localX * 32 + inThreadX;
          uint inTileShiftY = localY * 2 + inThreadY;

          uint absCurAX = curTileIdx * 64 + inTileShiftX;
          uint absCurAY = groupY * 16 + inTileShiftY;

          bool isOOBAbsAX = absCurAX >= dims.kDim;
          bool isOOBAbsAY = absCurAY >= dims.mDim;

          tryWrite(
              a,
              absCurAX,
              absCurAY,
              dims.kDim,
              aTile,
              inTileShiftX,
              inTileShiftY,
              64,
              inTileShiftX >= 64,
              inTileShiftY >= 16,
              isOOBAbsAX,
              isOOBAbsAY
          );
        }
      }

      uint deltaX = potolok(64, 8);
      uint deltaY = potolok(64, 2);
      for (size_t inThreadX = 0; inThreadX < deltaX; inThreadX++) {
        for (size_t inThreadY = 0; inThreadY < deltaY; inThreadY++) {
          uint inTileShiftX = localY * deltaX + inThreadX;
          uint inTileShiftY = localX * deltaY + inThreadY;

          uint absCurBX = groupX * 64 + inTileShiftX;
          uint absCurBY = curTileIdx * 64 + inTileShiftY;

          bool isOOBAbsBX = absCurBX >= dims.nDim;
          bool isOOBAbsBY = absCurBY >= dims.kDim;

          tryWrite(
              b,
              absCurBX,
              absCurBY,
              dims.nDim,
              bTile,
              inTileShiftX,
              inTileShiftY,
              64,
              inTileShiftX >= 64,
              inTileShiftY >= 64,
              isOOBAbsBX,
              isOOBAbsBY
          );
        }
      }

      barrier(CLK_LOCAL_MEM_FENCE);



      for (uint inThreadX = 0; inThreadX < 32; inThreadX++) {
        for (uint inThreadY = 0; inThreadY < 2; inThreadY++) {
          uint inTileShiftX = localX * 32 + inThreadX;
          uint inTileShiftY = localY * 2 + inThreadY;

          bool isOOBTileX = inTileShiftX >= 64;
          bool isOOBTileY = inTileShiftY >= 16;

          uint absCurAX = curTileIdx * 64 + inTileShiftX;
          uint absCurAY = groupY * 16 + inTileShiftY;

          uint absCurBX = groupX * 64 + inTileShiftX;
          uint absCurBY = curTileIdx * 64 + inTileShiftY;

          bool isOOBAbsAX = absCurAX >= dims.kDim;
          bool isOOBAbsAY = absCurAY >= dims.mDim;
          bool isOOBAbsBX = absCurBX >= dims.nDim;
          bool isOOBAbsBY = absCurBY >= dims.kDim;

          if (!(isOOBTileX || isOOBTileY)) {
            for (uint inTileIdx = 0; inTileIdx < 64; inTileIdx++) {
              singleSum[idx(inThreadX, inThreadY, 32)] +=
                  aTile[idx(inTileIdx, inTileShiftY, 64)] * bTile[idx(inTileShiftX, inTileIdx, 64)];
            }
          }
        }
      }

      barrier(CLK_LOCAL_MEM_FENCE);
    }

    for (uint inThreadX = 0; inThreadX < 32; inThreadX++) {
      for (uint inThreadY = 0; inThreadY < 2; inThreadY++) {
        uint inTileShiftX = localX * 32 + inThreadX;
        uint inTileShiftY = localY * 2 + inThreadY;
        uint absX = groupX * 64 + inTileShiftX;
        uint absY = groupY * 16 + inTileShiftY;

        bool isOOBTileX = inTileShiftX >= 64;
        bool isOOBTileY = inTileShiftY >= 16;

        bool isBOOBSAbsX = absX >= dims.nDim;
        bool isBOOBSAbsY = absY >= dims.mDim;

        if (!(isBOOBSAbsX || isBOOBSAbsY || isOOBTileX || isOOBTileY)) {
          c[idx(absX, absY, dims.nDim)] = singleSum[idx(inThreadX, inThreadY, 32)];
        }
      }
    }
  }
}
