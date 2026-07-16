#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include "ControlBlock.h"

namespace ct {

template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr {
public:
  using ControlBlock = detail::ControlBlockBase;

  ~SharedPtr() noexcept {
    if (cb_ == nullptr) {
      return;
    }
    cb_->release_strong_ref();
  }

  SharedPtr() noexcept
      : cb_(nullptr)
      , data_(nullptr) {}

  SharedPtr(std::nullptr_t) noexcept
      : cb_(nullptr)
      , data_(nullptr) {}

  template <typename Y>
    requires std::convertible_to<Y*, T*>
  explicit SharedPtr(Y* ptr) {
    try {
      cb_ = new detail::RegularControlBlock<Y>(ptr);
    } catch (...) {
      delete ptr;
      throw;
    }
    data_ = ptr;
    cb_->add_strong_ref();
    cb_->add_weak_ref();
  }

  template <typename Y, typename Deleter>
    requires std::convertible_to<Y*, T*>
  SharedPtr(Y* ptr, Deleter deleter) {
    static_assert(std::is_nothrow_move_constructible_v<Deleter>, "Deleter must be nothrow-move-constructible");
    try {
      cb_ = new detail::RegularControlBlock<Y, Deleter>(ptr, std::move(deleter));
    } catch (...) {
      deleter(ptr);
      throw;
    }
    data_ = ptr;
    cb_->add_strong_ref();
    cb_->add_weak_ref();
  }

  template <typename Y>
  SharedPtr(const SharedPtr<Y>& other, T* ptr) noexcept
      : cb_(other.cb())
      , data_(ptr) {
    if (cb_ != nullptr) {
      cb_->add_strong_ref();
    }
  }

  template <typename Y>
  SharedPtr(SharedPtr<Y>&& other, T* ptr) noexcept
      : cb_(other.cb()) {
    other.clear();
    data_ = ptr;
  }

  SharedPtr(const SharedPtr& other) noexcept {
    initCopy(other);
  }

  template <typename Y>
    requires std::convertible_to<Y*, T*>
  SharedPtr(const SharedPtr<Y>& other) noexcept {
    initCopy(other);
  }

  SharedPtr(SharedPtr&& other) noexcept {
    steal(std::move(other));
  }

  template <typename Y>
    requires std::convertible_to<Y*, T*>
  SharedPtr(SharedPtr<Y>&& other) noexcept {
    steal(std::move(other));
  }

  SharedPtr& operator=(const SharedPtr& other) noexcept {
    SharedPtr(other).swap(*this);
    return *this;
  }

  template <typename Y>
    requires std::convertible_to<Y*, T*>
  SharedPtr& operator=(const SharedPtr<Y>& other) noexcept {
    SharedPtr copy(other);
    swap(copy);
    return *this;
  }

  SharedPtr& operator=(SharedPtr&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    if (cb_) {
      cb_->release_strong_ref();
    }
    steal(std::move(other));
    return *this;
  }

  template <typename Y>
    requires std::convertible_to<Y*, T*>
  SharedPtr& operator=(SharedPtr<Y>&& other) noexcept {
    if (cb_ != nullptr) {
      cb_->release_strong_ref();
    }
    steal(std::move(other));
    return *this;
  }

  T* get() const noexcept {
    return data_;
  }

  explicit operator bool() const noexcept {
    return data_ != nullptr ? true : false;
  }

  T& operator*() const noexcept {
    return *get();
  }

  T* operator->() const noexcept {
    return get();
  }

  std::size_t use_count() const noexcept {
    if (cb_ == nullptr) {
      return 0;
    }
    return cb_->amount_strong_refs();
  }

  void reset() noexcept {
    SharedPtr().swap(*this);
  }

  template <typename Y>
  void reset(Y* new_ptr) {
    SharedPtr(new_ptr).swap(*this);
  }

  template <typename Y, typename Deleter>
  void reset(Y* new_ptr, Deleter deleter) {
    SharedPtr(new_ptr, std::move(deleter)).swap(*this);
  }

  template <typename Y>
  friend bool operator==(const SharedPtr& lhs, const SharedPtr<Y>& rhs) noexcept {
    return lhs.get() == rhs.get();
  }

  friend bool operator!=(const SharedPtr& lhs, const SharedPtr& rhs) noexcept {
    return !(lhs == rhs);
  }

  ControlBlock* const& cb() const noexcept {
    return cb_;
  }

  ControlBlock*& cb() noexcept {
    return cb_;
  }

  void clear() {
    cb_ = nullptr;
    data_ = nullptr;
  }

private:
  ControlBlock* cb_;
  T* data_;

  void swap(SharedPtr& other) noexcept {
    std::swap(cb_, other.cb());
    std::swap(data_, other.data_);
  }

  template <typename Y>
  void copy(const SharedPtr<Y>& other) {
    cb_ = other.cb();
    data_ = other.get();
  }

  template <typename Y>
  void steal(SharedPtr<Y>&& other) {
    copy(other);
    other.clear();
  }

  SharedPtr(ControlBlock* cb, T* data) noexcept
      : cb_(cb)
      , data_(data) {}

  template <typename Y>
    requires (std::is_convertible_v<Y*, T*>)
  void initCopy(const SharedPtr<Y>& other) noexcept {
    copy(other);
    if (cb_ != nullptr) {
      cb_->add_strong_ref();
    }
  }

  template <typename V, typename... Args>
  friend SharedPtr<V> make_shared(Args&&... args);

  template <typename V>
  friend class WeakPtr;
};

template <typename T>
class WeakPtr {
public:
  using ControlBlock = detail::ControlBlockBase;

  ~WeakPtr() noexcept {
    if (cb_ != nullptr) {
      cb_->release_weak_ref();
    }
  }

  WeakPtr() noexcept {
    cb_ = nullptr;
    data_ = nullptr;
  }

  template <typename Y>
    requires std::convertible_to<Y*, T*>
  WeakPtr(const SharedPtr<Y>& other) noexcept
      : cb_(other.cb())
      , data_(other.get()) {
    if (cb_ != nullptr) {
      cb_->add_weak_ref();
    }
  }

  template <typename Y>
    requires (std::is_convertible_v<Y*, T*>)
  void initCopy(const WeakPtr<Y>& other) noexcept {
    copy(other);
    if (cb_ != nullptr) {
      cb_->add_weak_ref();
    }
  }

  WeakPtr(const WeakPtr& other) noexcept {
    initCopy(other);
  }

  template <typename Y>
    requires (std::convertible_to<Y*, T*>)
  WeakPtr(const WeakPtr<Y>& other) noexcept {
    initCopy(other);
  }

  WeakPtr(WeakPtr&& other) noexcept {
    steal(std::move(other));
  }

  template <typename Y>
    requires (std::convertible_to<Y*, T*>)
  WeakPtr(WeakPtr<Y>&& other) noexcept {
    steal(std::move(other));
  }

  template <typename Y>
    requires (std::convertible_to<Y*, T*>)
  WeakPtr& operator=(const SharedPtr<Y>& other) noexcept {
    WeakPtr(other).swap(*this);
    return *this;
  }

  WeakPtr& operator=(const WeakPtr& other) noexcept {
    WeakPtr(other).swap(*this);
    return *this;
  }

  template <typename Y>
    requires (std::convertible_to<Y*, T*>)
  WeakPtr& operator=(const WeakPtr<Y>& other) noexcept {
    WeakPtr(other).swap(*this);
    return *this;
  }

  WeakPtr& operator=(WeakPtr&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    if (cb_) {
      cb_->release_weak_ref();
    }
    steal(std::move(other));
    return *this;
  }

  template <typename Y>
    requires (std::convertible_to<Y*, T*>)
  WeakPtr& operator=(WeakPtr<Y>&& other) noexcept {
    if (cb_) {
      cb_->release_weak_ref();
    }
    steal(std::move(other));
    return *this;
  }

  SharedPtr<T> lock() const noexcept {
    if (cb_ == nullptr) {
      return SharedPtr<T>();
    }
    if (cb_->amount_strong_refs() == 0) {
      return SharedPtr<T>();
    }
    cb_->add_strong_ref();
    return SharedPtr<T>(cb_, data_);
  }

  void reset() noexcept {
    WeakPtr().swap(*this);
  }

  T* get() const noexcept {
    return data_;
  }

  ControlBlock* const& cb() const noexcept {
    return cb_;
  }

  ControlBlock*& cb() noexcept {
    return cb_;
  }

  void clear() {
    cb_ = nullptr;
    data_ = nullptr;
  }

private:
  ControlBlock* cb_;
  T* data_;

  void swap(WeakPtr& other) noexcept {
    std::swap(cb_, other.cb());
    std::swap(data_, other.data_);
  }

  template <typename Y>
  void copy(const WeakPtr<Y>& other) {
    cb_ = other.cb();
    data_ = other.get();
  }

  template <typename Y>
  void steal(WeakPtr<Y>&& other) {
    copy(other);
    other.clear();
  }
};

template <typename V, typename... Args>
SharedPtr<V> make_shared(Args&&... args) {
  auto* cb = new detail::InplaceControlBlock<V>(std::forward<Args>(args)...);
  cb->add_strong_ref();
  cb->add_weak_ref();
  return SharedPtr<V>(cb, cb->get());
}

} // namespace ct

#endif
