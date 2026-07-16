#include "../include/errors.h"

#include <cuda_runtime.h>

namespace errors {

AbstractError::AbstractError(const std::string& message) {
  _message = message;
  _errCode = cudaError_t::cudaSuccess;
}

AbstractError::AbstractError(const std::string& message, cudaError_t errorCode) {
  _message = message;
  _errCode = errorCode;
}

void AbstractError::addErrorCode() const {
  if (_errCode != cudaError_t::cudaSuccess) {
    _resultString.append(
        "; error_code: " + std::to_string(_errCode) + "; error_message: " + cudaGetErrorString(_errCode)
    );
  }
}

const char* ValidationArgsError::what() const noexcept {
  _resultString = std::string("[my error] parsing error: ").append(_message);
  addErrorCode();
  return _resultString.c_str();
}

const char* LibInitializeError::what() const noexcept {
  _resultString = std::string("[my error] parsing error: ").append(_message);
  addErrorCode();
  return _resultString.c_str();
}

const char* MemoryError::what() const noexcept {
  _resultString = std::string("[my error] memory error: ").append(_message);
  addErrorCode();
  return _resultString.c_str();
}

const char* CudaRuntimeError::what() const noexcept {
  _resultString = std::string("[my error] cuda runtime error: ").append(_message);
  addErrorCode();
  return _resultString.c_str();
}

const char* UncorrectError::what() const noexcept {
  _resultString = std::string("[my error] uncorrect result: ").append(_message);
  addErrorCode();
  return _resultString.c_str();
}

} // namespace errors
