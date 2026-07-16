#include "kernel-utils.cl"

kernel void mul(__global MatrixType* a, __global MatrixType* b, __global MatrixType* c, DeviceInfo dims) {
  for (size_t artificialCnt = 0; artificialCnt < dims.artificialFactorIterations; ++artificialCnt) {
    uint x = get_global_id(0); // n
    uint y = get_global_id(1); // m

    float sum = 0;
    for (uint i = 0; i < dims.kDim; i++) {
      sum += a[idx(i, y, dims.kDim)] * b[idx(x, i, dims.nDim)];
    }
    c[idx(x, y, dims.nDim)] = sum;
  }
}
