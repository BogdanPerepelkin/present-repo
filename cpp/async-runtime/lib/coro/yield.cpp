#include "yield.h"

namespace ct::coro {

namespace details {

// using Handle = std::coroutine_handle<coro::Coroutine::promise_type>;

bool YieldAwaitable::await_ready() const noexcept {
  return false;
}

bool YieldAwaitable::await_suspend(Handle handle) noexcept {
  utils::helperSpawn(handle);
  return true;
}

void YieldAwaitable::await_resume() noexcept {}

} // namespace details

details::YieldAwaitable yield() {
  return details::YieldAwaitable{};
}

} // namespace ct::coro
