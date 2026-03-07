#pragma once

#include "intrusive-list/intrusive-list.h"
#include "resumable.h"

namespace ct::sched {

class IntrusiveListScheduler
    : public intrusive::List<Resumable<IntrusiveListScheduler>, Resumable<IntrusiveListScheduler>> {
public:
  // All tasks that were spawned must be eventually executed
  virtual void spawn(Resumable<IntrusiveListScheduler>&) = 0;
  virtual ~IntrusiveListScheduler() = default;
};

} // namespace ct::sched
