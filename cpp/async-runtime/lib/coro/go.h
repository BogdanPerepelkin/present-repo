#pragma once

#include "coroutine.h"

namespace ct::coro {

template <typename Scheduler, typename Routine>
void go(Scheduler& scheduler, const Routine& routine) {
  coro::Coroutine::Handle handle = routine().handle();
  scheduler.spawn(handle.promise());
}

template <typename Routine>
void go(const Routine& routine) {
  go(*detail::curScheduler, routine);
}

} // namespace ct::coro
