/* Compiler options:
-c -emit-llvm -target amdgcn-amd-amdhsa -x cl -O3 -cl-kernel-arg-info -Iinclude -DTILE_SIZE_W=96 -DTILE_SIZE_H=16 -DTILE_SIZE_K=6 -DTHREAD_RANGE_W=6 -DTHREAD_RANGE_H=8 -DLOCAL_GR_SIZE_W=16 -DLOCAL_GR_SIZE_H=2 -DN_SIZE=2 -D__OPENCL_VERSION__=200 -D__IMAGE_SUPPORT__=1 -Xclang -cl-ext=+cl_khr_fp64,+cl_khr_global_int32_base_atomics,+cl_khr_global_int32_extended_atomics,+cl_khr_local_int32_base_atomics,+cl_khr_local_int32_extended_atomics,+cl_khr_int64_base_atomics,+cl_khr_int64_extended_atomics,+cl_khr_3d_image_writes,+cl_khr_byte_addressable_store,+cl_khr_fp16,+cl_khr_gl_sharing,+cl_amd_device_attribute_query,+cl_amd_media_ops,+cl_amd_media_ops2,+cl_khr_d3d10_sharing,+cl_khr_d3d11_sharing,+cl_khr_dx9_media_sharing,+cl_khr_image2d_from_buffer,+cl_khr_subgroups,+cl_khr_gl_event,+cl_khr_mipmap_image,+cl_khr_mipmap_image_writes,+cl_amd_copy_buffer_p2p,+cl_amd_planar_yuv -mllvm -amdgpu-prelink -mcode-object-version=5  -include opencl-c.h 
Hash to override:
  Source: 0xfc14117c7c0089e8
  Source + clang options: 0x967f041c7d9721f3
*/

// TILE_SIZE_W % N_SIZE == 0
// TILE_SIZE_K % N_SIZE == 0
// THREAD_RANGE_W % N_SIZE == 0
// dims.nDim % N_SIZE == 0
// dims.kDim % N_SIZE == 0

#ifndef TILE_SIZE_W
#define TILE_SIZE_W 1
#endif

#ifndef TILE_SIZE_H
#define TILE_SIZE_H 1
#endif

#ifndef TILE_SIZE_K
#define TILE_SIZE_K 1
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

#ifndef N_SIZE
#define N_SIZE 2
#endif

#ifndef VECTOR_TILE_SIZE_W
#define VECTOR_TILE_SIZE_W (TILE_SIZE_W / N_SIZE)
#endif

#ifndef VECTOR_TILE_SIZE_K
#define VECTOR_TILE_SIZE_K (TILE_SIZE_K / N_SIZE)
#endif

#define CONCAT(l, r) l##r
#define FLOAT_PROXY(N) CONCAT(float, N)
typedef FLOAT_PROXY(N_SIZE) FLN;

typedef float MatrixType;

typedef union {
  FLN fln;
  MatrixType arr[N_SIZE];
} FLNUnion;

// optimizations
#ifndef NOUNROLL
#define NOUNROLL 0
#endif

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

kernel void mul(__global MatrixType* srcA, __global MatrixType* srcB, __global MatrixType* dstC, DeviceInfo dims) {
  const uint VECTOR_THREAD_RANGE_W = THREAD_RANGE_W / N_SIZE;

  __local FLN aTile[TILE_SIZE_H][VECTOR_TILE_SIZE_K];
  __local FLN bTile[VECTOR_TILE_SIZE_K][N_SIZE][VECTOR_TILE_SIZE_W];

  const uint GROUP_SHIFT_W = get_group_id(0) * TILE_SIZE_W;
  const uint GROUP_SHIFT_H = get_group_id(1) * TILE_SIZE_H;
  const uint GROUP_SHIFT_WV = get_group_id(0) * VECTOR_TILE_SIZE_W;

  const uint localX = get_local_id(0);
  const uint localY = get_local_id(1);

  const uint LOCAL_SHIFT_A_H = localY * THREAD_RANGE_H;
  const uint LOCAL_SHIFT_B_WV = localX * VECTOR_THREAD_RANGE_W;

  const uint LOCAL_SIZE_W = get_local_size(0);
  const uint LOCAL_SIZE_H = get_local_size(1);
  const uint THREADS_IN_GROUP = LOCAL_SIZE_W * LOCAL_SIZE_H;
  const uint THREAD_ID = localY * LOCAL_SIZE_W + localX;

  const uint kDimVec = dims.kDim / N_SIZE;
  const uint nDimVec = dims.nDim / N_SIZE;

  const __global FLN* vecSrcA = (const __global FLN*) srcA;
  const __global FLN* vecSrcB = (const __global FLN*) srcB;
  __global FLN* vecDstC = (__global FLN*) dstC;

  FLN singleSum[THREAD_RANGE_H][VECTOR_THREAD_RANGE_W];
  for (uint y = 0; y < THREAD_RANGE_H; y++) {
    for (uint w = 0; w < VECTOR_THREAD_RANGE_W; w++) {
      singleSum[y][w] = (FLN) (0);
    }
  }

  const uint TILES_PER_ELEMENT = potolok(dims.kDim, TILE_SIZE_K);
  for (uint curTileIdx = 0; curTileIdx < TILES_PER_ELEMENT; curTileIdx++) {
    const uint groupShiftK = curTileIdx * TILE_SIZE_K;

    // TILE A
    const uint groupShiftKV = curTileIdx * VECTOR_TILE_SIZE_K;
    for (uint y = 0; y < TILE_SIZE_H; y++) {
      const uint absY = GROUP_SHIFT_H + y;
      for (uint xVec = THREAD_ID; xVec < VECTOR_TILE_SIZE_K; xVec += THREADS_IN_GROUP) {
        const uint absX = groupShiftKV + xVec;
        FLN packed = (FLN)(0);
        if (absY < dims.mDim && absX < kDimVec) {
          packed = vecSrcA[idx(absX, absY, kDimVec)];
        }
        aTile[y][xVec] = packed;
      }
    }

    // TILE B
    for (uint kVec = 0; kVec < VECTOR_TILE_SIZE_K; kVec++) {
      for (uint lane = 0; lane < N_SIZE; lane++) {
        const uint absY = groupShiftK + kVec * N_SIZE + lane;
        for (uint xVec = THREAD_ID; xVec < VECTOR_TILE_SIZE_W; xVec += THREADS_IN_GROUP) {
          const uint absX = GROUP_SHIFT_WV + xVec;
          FLN packed = (FLN)(0);
          if (absY < dims.kDim && absX < nDimVec) {
            packed = vecSrcB[idx(absX, absY, nDimVec)];
          }
          bTile[kVec][lane][xVec] = packed;
        }
      }
    }

    barrier(CLK_LOCAL_MEM_FENCE);

    for (uint kVec = 0; kVec < VECTOR_TILE_SIZE_K; kVec++) {
      for (uint y = 0; y < THREAD_RANGE_H; y++) {
        FLNUnion aPacked;
        aPacked.fln = aTile[LOCAL_SHIFT_A_H + y][kVec];
        for (uint lane = 0; lane < N_SIZE; lane++) {
          const MatrixType aValue = aPacked.arr[lane];
          for (uint wv = 0; wv < VECTOR_THREAD_RANGE_W; wv++) {
            singleSum[y][wv] += aValue * bTile[kVec][lane][LOCAL_SHIFT_B_WV + wv];
          }
        }
      }
    }

    barrier(CLK_LOCAL_MEM_FENCE);
  }

  for (uint y = 0; y < THREAD_RANGE_H; y++) {
    const uint absY = GROUP_SHIFT_H + LOCAL_SHIFT_A_H + y;
    for (uint wv = 0; wv < VECTOR_THREAD_RANGE_W; wv++) {
      const uint absXVec = GROUP_SHIFT_WV + LOCAL_SHIFT_B_WV + wv;
      if (absY < dims.mDim && absXVec < nDimVec) {
        vecDstC[idx(absXVec, absY, nDimVec)] = singleSum[y][wv];
      }
    }
  }
}
