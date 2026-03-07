#include "run-loop.h"

#include "resumable.h"
#include "scheduler.h"

#include <cstddef>

namespace ct::sched {

std::size_t RunLoop::run_at_most(std::size_t limit) {
  std::size_t count = 0;

  while (count < limit) {
    if (!run_next()) {
      break;
    }
    count++;
  }
  return count;
}

bool RunLoop::run_next() {
  if (empty()) {
    return false;
  }
  intrusive::utils::ElementImpl* tmp = _next;
  _next->unlink();
  auto* resumable = static_cast<Resumable<IntrusiveListScheduler>*>(tmp);
  resumable->resume(*this);

  return true;
}

std::size_t RunLoop::run() {
  std::size_t count = 0;
  while (run_next()) {
    count++;
  }
  return count;
}

void RunLoop::spawn(Resumable<ct::sched::IntrusiveListScheduler>& task) {
  push_back(task);
}

} // namespace ct::sched
