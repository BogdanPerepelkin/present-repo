#pragma once

#include "coro/coroutine.h"
#include "intrusive-list/intrusive-list.h"

#include <queue>

#include <atomic>
#include <coroutine>
#include <cstddef>

namespace ct::utils {

struct SpinGuard {
  explicit SpinGuard(std::atomic<bool>& lock);

  ~SpinGuard();

  std::atomic<bool>& _lock;
};

void helperSpawn(std::coroutine_handle<> handle);

} // namespace ct::utils
