#pragma once

#include <gtest/gtest.h>

#include <atomic>
#include <concepts>
#include <coroutine>
#include <cstddef>
#include <memory>
#include <mutex>
#include <optional>
#include <ranges>
#include <thread>
#include <variant>

namespace ct {

template <typename T>
class SharedGenerator {
private:
  struct PromiseType;

  using Handle = std::coroutine_handle<PromiseType>;

  class Iter;

public:
  using promise_type = PromiseType;

  explicit SharedGenerator(const Handle coroutine)
      : _handle(coroutine) {}

  SharedGenerator() = default;

  ~SharedGenerator() {
    clear();
  }

  SharedGenerator(const SharedGenerator& other)
      : _handle(other._handle) {
    increase();
  }

  SharedGenerator& operator=(const SharedGenerator& other) {
    clear();
    _handle = other._handle;
    increase();
    return *this;
  }

  SharedGenerator(SharedGenerator&& other) noexcept
      : _handle(std::move(other._handle)) {
    other._handle = {};
    if (!_handle) {
      return;
    }
  }

  SharedGenerator& operator=(SharedGenerator&& other) noexcept {
    auto copy(std::move(other));
    std::swap(_handle, copy._handle);
    return *this;
  }

  struct Visitor {
    std::optional<T> operator()(const T* value) const {
      return *value;
    }

    std::optional<T> operator()(T* value) const {
      return std::move(*value);
    }
  };

  std::optional<T> next() {
    if (!_handle) {
      return std::nullopt;
    }
    {
      const std::lock_guard<std::mutex> lock(_handle.promise()._mutexHandleOp);

      if (_handle.done()) {
        return std::nullopt;
      }

      _handle.resume();

      if (!_handle.done()) {
        PromiseType& promise = _handle.promise();
        return std::visit(Visitor{}, promise._value);
      }
      return std::nullopt;
    }
  }

  Iter begin() {
    return Iter(_handle);
  }

  std::default_sentinel_t end() {
    return std::default_sentinel_t{};
  }

  bool empty() const {
    return !_handle || _handle.done();
  }

private:
  struct PromiseType {
    SharedGenerator<T> get_return_object() {
      return SharedGenerator{Handle::from_promise(*this)};
    }

    static std::suspend_always initial_suspend() noexcept {
      return {};
    }

    static std::suspend_always final_suspend() noexcept {
      return {};
    }

    std::suspend_always yield_value(const T& value) noexcept {
      _value.template emplace<const T*>(std::addressof(value));
      return {};
    }

    std::suspend_always yield_value(T&& value) noexcept {
      _value.template emplace<T*>(std::addressof(value));
      return {};
    }

    void return_void() const noexcept {}

    void await_transform() = delete;

    [[noreturn]]
    static void unhandled_exception() {
      throw;
    }

    std::variant<T*, const T*> _value;
    std::mutex _mutexHandleOp;
    std::atomic<std::size_t> _counter = 1;
  };

  class Iter {
  public:
    Iter(std::coroutine_handle<> handle)
        : _coroutine(handle) {}

    void operator++() {
      const std::lock_guard<std::mutex> lock(_coroutine.promise()._mutexHandleOp);

      _coroutine.resume();
    }

    std::optional<T> operator*() const {
      const std::lock_guard<std::mutex> lock(_coroutine.promise()._mutexHandleOp);

      auto& promise = _coroutine.promise();
      if (promise._value.index() == 0) {
        if (std::get<0>(promise._value) == nullptr) {
          _coroutine.resume();
        }
      }
      return std::visit(Visitor{}, promise._value);
    }

    bool operator==(std::default_sentinel_t /*unused*/) const {
      const std::lock_guard<std::mutex> lock(_coroutine.promise()._mutexHandleOp);

      return !_coroutine || _coroutine.done();
    }

    bool operator!=(std::default_sentinel_t /*unused*/) const {
      return !(*this == std::default_sentinel_t{});
    }

    explicit Iter(const Handle coroutine)
        : _coroutine{coroutine} {}

  private:
    Handle _coroutine;
  };

  Handle _handle;

  void increase() {
    if (_handle) {
      _handle.promise()._counter.fetch_add(1, std::memory_order_acq_rel);
    }
  }

  void clear() {
    if (!_handle) {
      return;
    }
    auto& promise = _handle.promise();
    if (promise._counter.fetch_sub(1, std::memory_order_acq_rel) == 1) {
      _handle.destroy();
    }
  }
};

} // namespace ct
