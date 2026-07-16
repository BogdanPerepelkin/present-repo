#pragma once

#include <cuda_runtime.h>

#include <exception>
#include <string>

namespace errors {

enum ERROR_CODES {
  SUCCESS = 0,
  INVAID_TILE_THREAD_SIZES = 67,
};

struct AbstractError : std::exception {
  AbstractError(const std::string& message);

  AbstractError(const std::string& message, cudaError_t errorCode);

  virtual const char* what() const noexcept = 0;

protected:
  std::string _message;
  cudaError_t _errCode;
  mutable std::string _resultString;

  void addErrorCode() const ;
};

struct ValidationArgsError : AbstractError {
  using AbstractError::AbstractError;
  const char* what() const noexcept override;
};

struct LibInitializeError : AbstractError {
  using AbstractError::AbstractError;
  const char* what() const noexcept override;
};

struct MemoryError : AbstractError {
  using AbstractError::AbstractError;
  const char* what() const noexcept override;
};

struct CudaRuntimeError : AbstractError {
  using AbstractError::AbstractError;
  const char* what() const noexcept override;
};

struct UncorrectError : AbstractError {
  using AbstractError::AbstractError;
  const char* what() const noexcept override;
};


} // namespace errors
