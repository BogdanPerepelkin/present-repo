
#include "mutex.h"

#include "utils.h"

namespace ct::sync {

namespace detail {

LockAwaitable::LockAwaitable(Mutex* parent)
    : _parent(parent) {}

bool LockAwaitable::await_ready() const noexcept {
  return false;
}

bool LockAwaitable::await_suspend(Handle handle) noexcept {
  _handle = handle;
  utils::SpinGuard sg(_parent->_localSync);

  if (_parent->_locked) {
    _parent->push_front(*this);
    return true;
  }
  _parent->_locked = true;
  return false;
}

void LockAwaitable::await_resume() noexcept {}

} // namespace detail

bool Mutex::UnlockAwaitable::await_ready() const noexcept {
  detail::LockAwaitable* coro2Resume = nullptr;
  {
    utils::SpinGuard sg(_parent->_localSync);

    if (!_parent->empty()) {
      coro2Resume = static_cast<detail::LockAwaitable*>(_parent->_prev);
      coro2Resume->unlink();
    } else {
      _parent->_locked = false;
    }
  }

  if (coro2Resume != nullptr) {
    utils::helperSpawn(coro2Resume->_handle);
  }
  return false;
}

bool Mutex::UnlockAwaitable::await_suspend(Handle) noexcept {
  return false;
}

void Mutex::UnlockAwaitable::await_resume() noexcept {}

bool Mutex::try_lock() noexcept {
  utils::SpinGuard sg(_localSync);

  if (_locked) {
    return false;
  }
  return _locked = true;
}

detail::LockAwaitable Mutex::lock() noexcept {
  return detail::LockAwaitable(this);
}

Mutex::UnlockAwaitable Mutex::unlock() noexcept {
  return UnlockAwaitable(this);
}

} // namespace ct::sync
