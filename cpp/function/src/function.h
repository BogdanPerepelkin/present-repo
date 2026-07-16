#pragma once
#include <cstddef>
#include <functional>
#include <new>
#include <ostream>
#include <type_traits>

namespace ct::utils {

inline constexpr std::size_t SMALL_SIZE = sizeof(void*) * 2;
inline constexpr std::size_t SMALL_ALIGNMENT = alignof(void*);

template <typename F>
concept small_object =
    sizeof(F) <= SMALL_SIZE && SMALL_ALIGNMENT % alignof(F) == 0 && (std::is_nothrow_move_constructible_v<F>);

} // namespace ct::utils

namespace ct {

class FunctionError : public std::exception {
public:
  explicit FunctionError(const char* message)
      : _message(message) {}

  const char* what() const noexcept override {
    return _message;
  }

private:
  const char* _message = nullptr;
};

class BadFunctionCall : public FunctionError {
public:
  using FunctionError::FunctionError;
};

template <typename F>
using FunctionPtr = F*;

template <typename F>
class Function;

template <typename R, typename... Args>
class Function<R(Args...)> {
private:
  using Storage = std::array<std::byte, utils::SMALL_SIZE>;

  struct AbsOperations {
    FunctionPtr<void(Storage&)> destroy;
    FunctionPtr<void(const Storage&, Storage&)> copy;
    FunctionPtr<void(Storage&, Storage&)> move;
    FunctionPtr<R(Storage&, Args&&...)> invoke;
  };

public:
  Function() noexcept
      : _ops(&OP<void>) {}

  template <typename F>
    requires utils::small_object<F>
  Function(F func)
      : _ops(&OP<F>) {
    new (&_storage[0]) F(std::move(func));
  }

  template <typename F>
  Function(F func)
      : _ops(&OP<F>) {
    static_assert(utils::small_object<F*>);
    new (&_storage[0]) F*(new F(std::move(func)));
  }

  Function(const Function& other)
      : _ops(other._ops) {
    _ops->copy(other._storage, _storage);
  }

  Function(Function&& other) noexcept
      : _ops(other._ops) {
    _ops->move(other._storage, _storage);
    other._ops = &OP<void>;
  }

  Function& operator=(const Function& other) {
    return *this = Function(other);
  }

  Function& operator=(Function&& other) noexcept {
    if (this != &other) {
      _ops->destroy(_storage);
      _ops = other._ops;
      _ops->move(other._storage, _storage);
      other._ops = &OP<void>;
    }
    return *this;
  }

  ~Function() {
    _ops->destroy(_storage);
  }

  explicit operator bool() const noexcept {
    return _ops != &OP<void>;
  }

  R operator()(Args... args) const {
    return _ops->invoke(rmCv(_storage), std::forward<Args>(args)...);
  }

  template <typename T>
  T* target() noexcept {
    if (_ops == &OP<T>) {
      return Operations<T>::get(_storage);
    }
    return nullptr;
  }

  template <typename T>
  const T* target() const noexcept {
    if (_ops == &OP<T>) {
      return Operations<T>::get(_storage);
    }
    return nullptr;
  }

private:
  // todo можно элегантнее (см. any-iterator)
  template <typename OnType>
  static constexpr OnType* ptrOn(Storage& storage) noexcept {
    return reinterpret_cast<OnType*>(&(storage)[0]);
  }

  template <typename OnType>
  static constexpr OnType* ptrOn(const Storage& storage) noexcept {
    return reinterpret_cast<OnType*>(&(storage)[0]);
  }

  template <typename F, bool Collable = std::is_invocable_r_v<R, F, Args...>, bool IsSmall = utils::small_object<F>>
  struct Operations {
    static constexpr AbsOperations vtable{
        .destroy = [](Storage&) {},
        .copy = [](const Storage&, Storage&) {},
        .move = [](Storage&, Storage&) {},
        .invoke = [](Storage&, Args&&...) -> R {
          throw BadFunctionCall("Function: call operator() with empty object");
        },
    };

    [[maybe_unused]] static F* get(const Storage& /*unused*/) {
      return nullptr;
    }
  };

  template <typename F>
  struct Operations<F, true, true> {
    static F* get(Storage& storage) {
      return std::launder(ptrOn<F>(storage));
    }

    static const F* get(const Storage& storage) noexcept {
      return std::launder(ptrOn<const F>(storage));
    }

    static constexpr AbsOperations vtable{
        .destroy = [](Storage& storage) { std::launder(ptrOn<F>(storage))->~F(); },
        .copy = [](const Storage& from, Storage& to) { new (&to[0]) F(*std::launder(ptrOn<const F>(from))); },
        .move =
            [](Storage& from, Storage& to) {
              auto* funcPtr = std::launder(ptrOn<F>(from));
              new (&to[0]) F(std::move(*funcPtr));
              funcPtr->~F();
            },
        .invoke = [](Storage& storage, Args&&... args) -> R { return (*get(storage))(std::forward<Args>(args)...); },
    };
  };

  template <typename F>
  struct Operations<F, true, false> {
    static F* get(Storage& storage) {
      return *ptrOn<F*>(storage);
    }

    static const F* get(const Storage& storage) noexcept {
      return *ptrOn<F* const>(storage);
    }

    static constexpr AbsOperations vtable{
        .destroy = [](Storage& storage) { delete *ptrOn<F*>(storage); },
        .copy = [](const Storage& from, Storage& to) { new (&to[0]) F*(new F(**ptrOn<F* const>(from))); },
        .move =
            [](Storage& from, Storage& to) {
              auto* ptr = ptrOn<F*>(from);
              new (&to[0]) F*(std::move(*ptr));
              *ptr = nullptr;
            },
        .invoke = [](Storage& storage, Args&&... args) -> R { return (*get(storage))(std::forward<Args>(args)...); },
    };
  };

  template <typename F>
  static constexpr AbsOperations OP = Operations<F>::vtable;

  const AbsOperations* _ops;

  alignas(utils::SMALL_ALIGNMENT) Storage _storage;

  Storage& rmCv(const Storage& storage) const noexcept {
    return const_cast<Storage&>(storage);
  }
};

} // namespace ct
