#include "fault-injection.h"

#include <gtest/gtest.h>

#include <cassert>
#include <iostream>
#include <vector>

namespace {

struct FaultInjectionContext {
  std::vector<std::size_t> skip_ranges;
  std::size_t error_index = 0;
  std::size_t skip_index = 0;
  bool fault_registered = false;
};

thread_local bool g_disabled = false;
thread_local FaultInjectionContext* g_context = nullptr;

struct ContextGuard {
  explicit ContextGuard(FaultInjectionContext& ctx) noexcept {
    g_context = &ctx;
  }

  ~ContextGuard() {
    g_context = nullptr;
  }
};

void dump_state() {
#if 0
  ct_test::FaultInjectionDisable dg;
  std::cout << "skip_ranges: {";
  if (!g_context->skip_ranges.empty()) {
    std::cout << g_context->skip_ranges[0];
    for (std::size_t i = 1; i != g_context->skip_ranges.size(); ++i) {
      std::cout << ", " << g_context->skip_ranges[i];
    }
  }
  std::cout << "}\nerror_index: " << g_context->error_index << "\nskip_index: " << g_context->skip_index << '\n'
            << std::flush;
#endif
}

} // namespace

namespace ct_test {

bool should_inject_fault() {
  if (g_context == nullptr) {
    return false;
  }

  if (g_disabled) {
    return false;
  }

  assert(g_context->error_index <= g_context->skip_ranges.size());
  if (g_context->error_index == g_context->skip_ranges.size()) {
    FaultInjectionDisable dg;
    ++g_context->error_index;
    g_context->skip_ranges.push_back(0);
    g_context->fault_registered = true;
    return true;
  }

  assert(g_context->skip_index <= g_context->skip_ranges[g_context->error_index]);

  if (g_context->skip_index == g_context->skip_ranges[g_context->error_index]) {
    ++g_context->error_index;
    g_context->skip_index = 0;
    g_context->fault_registered = true;
    return true;
  }

  ++g_context->skip_index;
  return false;
}

void fault_injection_point() {
  if (should_inject_fault()) {
    FaultInjectionDisable dg;
    throw InjectedFault("injected fault");
  }
}

void faulty_run(const std::function<void()>& f) {
  assert(!g_context);
  FaultInjectionContext ctx;
  ContextGuard cg(ctx);
  for (;;) {
    try {
      f();
    } catch (...) {
      FaultInjectionDisable dg;
      dump_state();
      if (!ctx.fault_registered) {
        GTEST_FAIL();
      }
      ctx.skip_ranges.resize(ctx.error_index);
      ++ctx.skip_ranges.back();
      ctx.error_index = 0;
      ctx.skip_index = 0;
      ctx.fault_registered = false;
      continue;
    }
    if (ctx.fault_registered) {
      FaultInjectionDisable dg;
      GTEST_FAIL() << "A fault was injected during testing, but the test suite didn't detect the error. "
                   << "If you see this message, check if exceptions are properly rethrown in your solution";
    }
    break;
  }
}

void assert_nothrow(const std::function<void()>& f) {
  assert(!g_context);
  FaultInjectionContext ctx;
  ContextGuard cg(ctx);
  try {
    f();
  } catch (...) {
    FaultInjectionDisable dg;
    GTEST_FAIL() << "Exception thrown while no were expected";
  }
}

FaultInjectionDisable::FaultInjectionDisable()
    : was_disabled(g_disabled) {
  g_disabled = true;
}

void FaultInjectionDisable::reset() const {
  g_disabled = was_disabled;
}

FaultInjectionDisable::~FaultInjectionDisable() {
  reset();
}

} // namespace ct_test
