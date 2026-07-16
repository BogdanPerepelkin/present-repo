// naive
#ifndef TILE_SIZE
#define TILE_SIZE 1
#endif

#ifndef SINGLE_THREAD_RANGE
#define SINGLE_THREAD_RANGE 1
#endif

// advanced
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

inline static uint potolok(const uint a, const uint b) {
  return a / b + (a % b != 0 ? 1 : 0);
}

inline void tryWrite(
    __global MatrixType* src,
    uint srcX,
    uint srcY,
    uint wide,
    int isOOBX,
    int isOOBY,
    __local MatrixType* tile,
    uint lX,
    uint lY,
    int isLast
) {
  MatrixType value = 0;
  if (!isLast || !(isOOBX || isOOBY)) {
    value = src[idx(srcX, srcY, wide)];
  }
  tile[idx(lX, lY, TILE_SIZE)] = value;
}