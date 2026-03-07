#include "event.h"

#include <cassert>
#include <coroutine>
#include <mutex>
#include <ostream>

namespace ct::sync {

Event::~Event() {}

void Event::emit() noexcept {
  WaitAwaitable* cur = nullptr;
  {
    std::lock_guard<std::mutex> lg(_mutex);
    _ready = true;
    cur = _list;
    _list = nullptr;
  }

  while (cur != nullptr) {
    auto* following = cur->_next;
    utils::helperSpawn(cur->_handle);
    cur = following;
  }
}

bool Event::emitted() const noexcept {
  return _ready.load();
}

Event::WaitAwaitable Event::wait() noexcept {
  return WaitAwaitable(this);
}

Event::WaitAwaitable::WaitAwaitable(Event* event)
    : _event(event) {}

bool Event::WaitAwaitable::await_ready() const noexcept {
  assert(_event != nullptr);

  std::lock_guard<std::mutex> lg(_event->_mutex);
  return _event->emitted();
}

bool Event::WaitAwaitable::await_suspend(Handle handle) noexcept {
  assert(_event != nullptr);

  std::lock_guard<std::mutex> lg(_event->_mutex);
  if (_event->emitted()) {
    return false;
  }

  _handle = handle;
  _next = _event->_list;
  _event->_list = this;
  return true;
}

void Event::WaitAwaitable::await_resume() noexcept {}

} // namespace ct::sync
