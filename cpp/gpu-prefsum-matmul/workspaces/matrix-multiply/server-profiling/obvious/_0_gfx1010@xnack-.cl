/* Compiler options:
-c -emit-llvm -target amdgcn-amd-amdhsa -x cl -O3 -cl-kernel-arg-info -Iinclude -DTILE_SIZE_W=64 -DTILE_SIZE_H=16 -DTHREAD_RANGE_W=32 -DTHREAD_RANGE_H=2 -DLOCAL_GR_SIZE_W=2 -DLOCAL_GR_SIZE_H=8 -D__OPENCL_VERSION__=200 -D__IMAGE_SUPPORT__=1 -Xclang -cl-ext=+cl_khr_fp64,+cl_khr_global_int32_base_atomics,+cl_khr_global_int32_extended_atomics,+cl_khr_local_int32_base_atomics,+cl_khr_local_int32_extended_atomics,+cl_khr_int64_base_atomics,+cl_khr_int64_extended_atomics,+cl_khr_3d_image_writes,+cl_khr_byte_addressable_store,+cl_khr_fp16,+cl_khr_gl_sharing,+cl_amd_device_attribute_query,+cl_amd_media_ops,+cl_amd_media_ops2,+cl_khr_d3d10_sharing,+cl_khr_d3d11_sharing,+cl_khr_dx9_media_sharing,+cl_khr_image2d_from_buffer,+cl_khr_subgroups,+cl_khr_gl_event,+cl_khr_mipmap_image,+cl_khr_mipmap_image_writes,+cl_amd_copy_buffer_p2p,+cl_amd_planar_yuv -mllvm -amdgpu-prelink -mcode-object-version=5  -include opencl-c.h 
Hash to override:
  Source: 0x2c83d775bd7f2e31
  Source + clang options: 0xa05698fe268c5897
*/

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
  __local MatrixType aTile[TILE_SIZE_W * TILE_SIZE_H]; // маленький прямоугольничек
  __local MatrixType bTile[TILE_SIZE_W * TILE_SIZE_W]; // большой квадратик

  for (size_t artificial = 0; artificial < dims.artificialFactorIterations; artificial++) {
    uint groupX = get_group_id(0);
    uint groupY = get_group_id(1);

    uint localX = get_local_id(0);
    uint localY = get_local_id(1);

    MatrixType singleSum[THREAD_RANGE_W * THREAD_RANGE_H] = {0};

    const uint tilesPerElement = dims.kDim / TILE_SIZE_W + (dims.kDim % TILE_SIZE_W != 0 ? 1 : 0);
    // для B локалка TS_W * TS_W, а для A TS_W * TS_H
    // для B работает TR_W * TR_W потоков, а для A TR_W * TR_H
    for (uint curTileIdx = 0; curTileIdx < tilesPerElement; curTileIdx++) {
      for (uint inThreadX = 0; inThreadX < THREAD_RANGE_W; inThreadX++) {
        for (uint inThreadY = 0; inThreadY < THREAD_RANGE_H; inThreadY++) {
          uint inTileShiftX = localX * THREAD_RANGE_W + inThreadX;
          uint inTileShiftY = localY * THREAD_RANGE_H + inThreadY;

          uint absCurAX = curTileIdx * TILE_SIZE_W + inTileShiftX;
          uint absCurAY = groupY * TILE_SIZE_H + inTileShiftY;

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
              TILE_SIZE_W,
              inTileShiftX >= TILE_SIZE_W,
              inTileShiftY >= TILE_SIZE_H,
              isOOBAbsAX,
              isOOBAbsAY
          );
        }
      }

      uint deltaX = potolok(TILE_SIZE_W, LOCAL_GR_SIZE_H);
      uint deltaY = potolok(TILE_SIZE_W, LOCAL_GR_SIZE_W);
      for (size_t inThreadX = 0; inThreadX < deltaX; inThreadX++) {
        for (size_t inThreadY = 0; inThreadY < deltaY; inThreadY++) {
          uint inTileShiftX = localY * deltaX + inThreadX;
          uint inTileShiftY = localX * deltaY + inThreadY;

          uint absCurBX = groupX * TILE_SIZE_W + inTileShiftX;
          uint absCurBY = curTileIdx * TILE_SIZE_W + inTileShiftY;

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
              TILE_SIZE_W,
              inTileShiftX >= TILE_SIZE_W,
              inTileShiftY >= TILE_SIZE_W,
              isOOBAbsBX,
              isOOBAbsBY
          );
        }
      }

      barrier(CLK_LOCAL_MEM_FENCE);
      // ? стоит ли разбивать чтение и подсчет ?
      // стоит потому что меньше оверхед на barrier, но не стоит потом что оверхед на for

      for (uint inThreadX = 0; inThreadX < THREAD_RANGE_W; inThreadX++) {
        for (uint inThreadY = 0; inThreadY < THREAD_RANGE_H; inThreadY++) { // H
          uint inTileShiftX = localX * THREAD_RANGE_W + inThreadX;
          uint inTileShiftY = localY * THREAD_RANGE_H + inThreadY;

          bool isOOBTileX = inTileShiftX >= TILE_SIZE_W;
          bool isOOBTileY = inTileShiftY >= TILE_SIZE_H;

          uint absCurAX = curTileIdx * TILE_SIZE_W + inTileShiftX;
          uint absCurAY = groupY * TILE_SIZE_H + inTileShiftY;

          uint absCurBX = groupX * TILE_SIZE_W + inTileShiftX;
          uint absCurBY = curTileIdx * TILE_SIZE_W + inTileShiftY;

          bool isOOBAbsAX = absCurAX >= dims.kDim;
          bool isOOBAbsAY = absCurAY >= dims.mDim;
          bool isOOBAbsBX = absCurBX >= dims.nDim;
          bool isOOBAbsBY = absCurBY >= dims.kDim;

          if (!(isOOBTileX || isOOBTileY)) {
            for (uint inTileIdx = 0; inTileIdx < TILE_SIZE_W; inTileIdx++) {
              singleSum[idx(inThreadX, inThreadY, THREAD_RANGE_W)] +=
                  aTile[idx(inTileIdx, inTileShiftY, TILE_SIZE_W)] * bTile[idx(inTileShiftX, inTileIdx, TILE_SIZE_W)];
            }
          }
        }
      }

      barrier(CLK_LOCAL_MEM_FENCE);
    }

    for (uint inThreadX = 0; inThreadX < THREAD_RANGE_W; inThreadX++) {
      for (uint inThreadY = 0; inThreadY < THREAD_RANGE_H; inThreadY++) {
        uint inTileShiftX = localX * THREAD_RANGE_W + inThreadX;
        uint inTileShiftY = localY * THREAD_RANGE_H + inThreadY;
        uint absX = groupX * TILE_SIZE_W + inTileShiftX;
        uint absY = groupY * TILE_SIZE_H + inTileShiftY;

        bool isOOBTileX = inTileShiftX >= TILE_SIZE_W;
        bool isOOBTileY = inTileShiftY >= TILE_SIZE_H;

        bool isBOOBSAbsX = absX >= dims.nDim;
        bool isBOOBSAbsY = absY >= dims.mDim;

        if (!(isBOOBSAbsX || isBOOBSAbsY || isOOBTileX || isOOBTileY)) {
          c[idx(absX, absY, dims.nDim)] = singleSum[idx(inThreadX, inThreadY, THREAD_RANGE_W)];
        }
      }
    }
  }
}
