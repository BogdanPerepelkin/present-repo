#pragma once

#include "coro/coroutine.h"
#include "intrusive-list/intrusive-list.h"

#include <coroutine>
#include <mutex>

namespace ct::sync {

// Must preserve FIFO ordering

class Mutex;

namespace detail {

struct LockAwaitable : intrusive::ListElement<LockAwaitable> {
  using Handle = std::coroutine_handle<coro::Coroutine::promise_type>;

  LockAwaitable(Mutex* parent);

  bool await_ready() const noexcept;

  bool await_suspend(Handle) noexcept;

  void await_resume() noexcept;

  Mutex* _parent;
  Handle _handle;
};

} // namespace detail

class Mutex : intrusive::List<detail::LockAwaitable, detail::LockAwaitable> {
private:
  struct UnlockAwaitable {
    using Handle = std::coroutine_handle<coro::Coroutine::promise_type>;

    UnlockAwaitable(Mutex* parent)
        : _parent(parent) {}

    bool await_ready() const noexcept;

    bool await_suspend(Handle) noexcept;

    void await_resume() noexcept;

    Mutex* _parent;
  };

public:
  bool try_lock() noexcept;

  detail::LockAwaitable lock() noexcept;

  UnlockAwaitable unlock() noexcept;

private:
  std::atomic<bool> _localSync{false};
  bool _locked = false;

  friend struct detail::LockAwaitable;
  friend struct UnlockAwaitable;
};

} // namespace ct::sync
