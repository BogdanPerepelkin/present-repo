#pragma once

#include "coroutine.h"

namespace ct::coro {

namespace details {

struct YieldAwaitable {
  using Handle = std::coroutine_handle<coro::Coroutine::promise_type>;

  bool await_ready() const noexcept;

  bool await_suspend(Handle handle) noexcept;

  void await_resume() noexcept;
};

} // namespace details

details::YieldAwaitable yield();

} // namespace ct::coro
