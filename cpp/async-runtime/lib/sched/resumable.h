#pragma once

#include "intrusive-list/intrusive-list.h"

namespace ct::sched {

template <typename Scheduler>
class Resumable : public intrusive::ListElement<Resumable<Scheduler>> {
public:
  virtual void resume(Scheduler&) noexcept = 0;
  virtual ~Resumable() = default;
};

} // namespace ct::sched
