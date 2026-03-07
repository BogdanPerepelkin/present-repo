#pragma once

#include "coro/coroutine.h"
#include "sched/scheduler.h"

#include <cassert>
#include <coroutine>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

namespace ct::sync {

class Event {
private:
  struct WaitAwaitable {
    using Handle = std::coroutine_handle<coro::Coroutine::promise_type>;

    WaitAwaitable(const WaitAwaitable&) = delete;
    WaitAwaitable& operator=(const WaitAwaitable&) = delete;
    WaitAwaitable(WaitAwaitable&&) = delete;
    WaitAwaitable& operator=(WaitAwaitable&&) = delete;

    WaitAwaitable(Event* event);

    bool await_ready() const noexcept;

    bool await_suspend(Handle handle) noexcept;

    void await_resume() noexcept;

    Event* _event;
    WaitAwaitable* _next = nullptr;
    Handle _handle;
  };

public:
  ~Event();

  void emit() noexcept;

  bool emitted() const noexcept;

  WaitAwaitable wait() noexcept;

private:
  std::mutex _mutex;
  std::atomic<bool> _ready = false;
  WaitAwaitable* _list = nullptr;

  friend WaitAwaitable;
};

} // namespace ct::sync
