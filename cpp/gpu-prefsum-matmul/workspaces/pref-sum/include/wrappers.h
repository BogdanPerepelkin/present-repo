#ifdef __APPLE__
#include "driver_types.h"
#endif

#include <cuda_runtime.h>

#include <string>

namespace prefsum {

void checkErr(cudaError_t errCode, const std::string& mes);

template <typename T>
struct RaiMalloc {
  RaiMalloc(size_t size) {
    checkErr(cudaMalloc(&_ptr, sizeof(T) * size), "malloc");
  }

  ~RaiMalloc() {
    cudaFree(_ptr);
  }

  operator T*() const {
    return _ptr;
  }

  operator const T*() const {
    return _ptr;
  }

  T* _ptr;
};

template <typename T>
struct RaiHostAlloc {
  RaiHostAlloc(size_t size) {
    checkErr(cudaHostAlloc(reinterpret_cast<void**>(&_ptr), sizeof(T) * size, cudaHostAllocDefault), "host alloc");
  }

  ~RaiHostAlloc() {
    cudaFreeHost(_ptr);
  }

  operator T*() const {
    return _ptr;
  }

  T* _ptr;
};

struct RaiEvent {
  RaiEvent();

  ~RaiEvent();

  operator cudaEvent_t() const;

  cudaEvent_t _cudaEvent;
};

} // namespace prefsum
