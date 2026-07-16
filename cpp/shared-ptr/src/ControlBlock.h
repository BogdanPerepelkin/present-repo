#ifndef CONTROLBLOCK_H
#define CONTROLBLOCK_H

#include <cstddef>
#include <iostream>
#include <memory>

namespace detail {

class ControlBlockBase {
public:
  ControlBlockBase() = default;

  virtual ~ControlBlockBase() = default;

  void add_strong_ref() noexcept {
    ++_strong_refs;
  }

  void add_weak_ref() noexcept {
    ++_weak_refs;
  }

  void release_strong_ref() noexcept {
    --_strong_refs;
    if (_strong_refs == 0) {
      delete_data();
      release_weak_ref();
    }
  }

  void release_weak_ref() noexcept {
    --_weak_refs;
    if (_strong_refs == 0 && _weak_refs == 0) {
      delete this;
    }
  }

  size_t amount_strong_refs() const noexcept {
    return _strong_refs;
  }

protected:
  virtual void delete_data() noexcept = 0;

private:
  std::size_t _strong_refs = 0;
  std::size_t _weak_refs = 0;
};

template <typename T, typename Deleter = std::default_delete<T>>
class RegularControlBlock : public ControlBlockBase {
public:
  explicit RegularControlBlock(T* ptr) noexcept
      : _data(ptr)
      , _deleter() {}

  explicit RegularControlBlock(T* ptr, Deleter&& deleter) noexcept
      : _data(ptr)
      , _deleter(std::move(deleter)) {}

protected:
  void delete_data() noexcept override {
    _deleter(_data);
  }

private:
  T* _data;
  [[no_unique_address]] Deleter _deleter;
};

template <typename T>
class InplaceControlBlock : public ControlBlockBase {
public:
  template <typename... Args>
  explicit InplaceControlBlock(Args&&... args)
      : _data(std::forward<Args>(args)...) {}

  ~InplaceControlBlock() override {}

  T* get() noexcept {
    return &_data;
  }

protected:
  void delete_data() noexcept override {
    _data.~T();
  }

private:
  union {
    T _data;
  };
};

} // namespace detail

#endif
