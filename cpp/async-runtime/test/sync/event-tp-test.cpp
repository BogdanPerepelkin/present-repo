
#include "coro/go.h"
#include "sched/thread-pool.h"
#include "sync/event.h"
#include "wait-group.h"

#include <gtest/gtest.h>

#include <cstddef>

namespace ct_test {

using namespace ct;

// gpt
// debugger (надо научиться стартовать со 150 итерации)

// ++ 1. spawner - 2 ptr с type_erasure
// ++ 2. wrapper в resume умеет только в resume
// ++ 3. операции в co_await сами знают планировать или нет
// ++ 4. final suspend в Coroutine и запоминать в Wrapper 2 ptr-а

TEST(EventThreadPoolTest, ConsumerProducer) {
  sched::ThreadPool pool{4};
  pool.run();

  size_t iterations = 0;
  const size_t LIMIT = 10000;
  const size_t STEP = 7;

  while (iterations++ < LIMIT) {
    WaitGroup wg;
    sync::Event event;
    int data = 0;

    for (std::size_t i = 0; i < iterations % STEP; ++i) {
      wg.add(1);
      coro::go(pool, [&](this auto) -> coro::Coroutine {
        co_await event.wait();
        EXPECT_EQ(data, 1);
        wg.done();
      });
    }

    wg.add(1);
    coro::go(pool, [&](this auto) -> coro::Coroutine {
      data = 1;
      event.emit();
      wg.done();
      co_return;
    });

    wg.wait();
  }
}

} // namespace ct_test
