#pragma once

#include <functional>
#include <stdexcept>

namespace ct_test {

struct InjectedFault : std::runtime_error {
  using runtime_error::runtime_error;
};

bool should_inject_fault();
void fault_injection_point();
void faulty_run(const std::function<void()>& f);
void assert_nothrow(const std::function<void()>& f);

struct FaultInjectionDisable {
  FaultInjectionDisable();

  void reset() const;

  FaultInjectionDisable(const FaultInjectionDisable&) = delete;
  FaultInjectionDisable& operator=(const FaultInjectionDisable&) = delete;

  ~FaultInjectionDisable();

private:
  bool was_disabled;
};

class AllocationCounter {
public:
  AllocationCounter();

  void assert_allocated(std::size_t total, std::size_t remaining) const;

private:
  std::size_t _new_calls_before;
  std::size_t _delete_calls_before;
};

} // namespace ct_test
