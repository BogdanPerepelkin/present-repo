#include "utils.h"

#include "coro/coroutine.h"

#include <cstddef>
#include <string>
#include <thread>

namespace ct::utils {

SpinGuard::SpinGuard(std::atomic<bool>& lock)
    : _lock(lock) {
  while (_lock.exchange(true, std::memory_order_acq_rel)) {
    std::this_thread::yield();
  }
}

SpinGuard::~SpinGuard() {
  _lock.store(false, std::memory_order_release);
}

void helperSpawn(std::coroutine_handle<> handle) {
  using Promise = coro::Coroutine::promise_type;
  auto typed = std::coroutine_handle<Promise>::from_address(handle.address());

  auto* scheduler = typed.promise().scheduler();
  scheduler->spawn(typed.promise());
}

} // namespace ct::utils
