#include "coroutine.h"

#include <cstddef>
#include <string>

namespace ct::coro {

namespace detail {

thread_local sched::IntrusiveListScheduler* curScheduler = nullptr;
thread_local Coroutine* curCoro = nullptr;

} // namespace detail

Coroutine::Coroutine(const Handle h, Coroutine*& coro)
    : _handle(h) {
  coro = this;
}

Coroutine::Coroutine(Coroutine&& other) noexcept
    : _handle(other._handle) {
  if (other._handle) {
    other._handle = {};
  }
}

Coroutine& Coroutine::operator=(Coroutine&& other) noexcept {
  if (this != &other) {
    if (_handle) {
      _handle.destroy();
    }
    _handle = other._handle;
    other._handle = {};
  }
  return *this;
}

Coroutine::promise_type& Coroutine::promise() {
  return _handle.promise();
}

Coroutine& Coroutine::current() {
  return *detail::curCoro;
}

Coroutine::Handle Coroutine::handle() {
  return _handle;
}

} // namespace ct::coro
