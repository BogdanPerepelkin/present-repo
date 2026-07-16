
#ifdef __APPLE__
#include "driver_types.h"
#endif

#include "../include/errors.h"
#include "../include/wrappers.h"

#include <cuda_runtime.h>

namespace prefsum {

void checkErr(cudaError_t errCode, const std::string& mes) {
  if (errCode != cudaSuccess) {
    throw errors::CudaRuntimeError(mes, errCode);
  }
}

RaiEvent::RaiEvent() {
  checkErr(cudaEventCreate(&_cudaEvent), "create event");
}

RaiEvent::~RaiEvent() {
  cudaEventDestroy(_cudaEvent);
}

RaiEvent::operator cudaEvent_t() const {
  return _cudaEvent;
}

} // namespace prefsum
