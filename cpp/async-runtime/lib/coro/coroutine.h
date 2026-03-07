#pragma once

#include "sched/resumable.h"
#include "sched/scheduler.h"
#include "utils.h"

#include <atomic>
#include <coroutine>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>

namespace ct::sync {

template <typename T>
class BufferedChannel;

class Event;

} // namespace ct::sync

namespace ct::utils {

void helperSpawn(std::coroutine_handle<>);

} // namespace ct::utils

namespace ct::coro {

class Coroutine;

namespace detail {

struct PromiseType;

extern thread_local sched::IntrusiveListScheduler* curScheduler;
extern thread_local Coroutine* curCoro;

} // namespace detail

class Coroutine {
public:
  using promise_type = detail::PromiseType;

private:
  using Handle = std::coroutine_handle<detail::PromiseType>;

public:
  Coroutine(const Handle h, Coroutine*& coro);

  Coroutine(Coroutine&& other) noexcept;

  Coroutine& operator=(Coroutine&& other) noexcept;

  promise_type& promise();

  static Coroutine& current();

private:
  Handle _handle;

  Handle handle();

  template <typename Scheduler, typename Routine>
  friend void go(Scheduler&, const Routine&);

  template <typename Routine>
  friend void go(const Routine& routine);
};

namespace detail {

struct PromiseType : sched::Resumable<sched::IntrusiveListScheduler> {
  using Handle = std::coroutine_handle<PromiseType>;

  Coroutine get_return_object() {
    return Coroutine{Handle::from_promise(*this), _coroutine};
  }

  std::suspend_always initial_suspend() noexcept {
    return {};
  }

  struct FinalCoroAwaitable {
    bool await_ready() const noexcept {
      return false;
    }

    void await_suspend(Handle handle) noexcept {
      handle.destroy();
    }

    void await_resume() noexcept {}
  };

  FinalCoroAwaitable final_suspend() noexcept {
    return {};
  }

  void return_void() const noexcept {}

  [[noreturn]]
  void unhandled_exception() {
    std::terminate();
  }

  void resume(sched::IntrusiveListScheduler& scheduler) noexcept override {
    auto* lastScheduler = detail::curScheduler;
    detail::curScheduler = std::addressof(scheduler);

    auto* lastCoro = detail::curCoro;
    detail::curCoro = _coroutine;

    _scheduler = std::addressof(scheduler);
    Handle::from_promise(*this).resume();

    detail::curScheduler = lastScheduler;

    detail::curCoro = lastCoro;
  }

private:
  sched::IntrusiveListScheduler* _scheduler = nullptr;
  Coroutine* _coroutine = nullptr;

  template <typename Scheduler, typename Routine>
  friend void go(Scheduler& scheduler, const Routine& routine);

  template <typename Routine>
  friend void go(const Routine& routine);

  friend class sync::Event;

  template <typename T>
  friend class sync::BufferedChannel;

  sched::IntrusiveListScheduler* scheduler() {
    return _scheduler;
  }

  friend void utils::helperSpawn(std::coroutine_handle<>);
};

} // namespace detail

} // namespace ct::coro
