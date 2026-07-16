#include "shared-generator.h"

#include <gtest/gtest.h>

#include <atomic>
#include <cstddef>
#include <latch>
#include <mutex>
#include <optional>
#include <set>
#include <thread>
#include <vector>

namespace ct_test {

TEST(GeneratorParallelTest, TwoThreadsOneValue) {
  auto gen = [] -> ct::SharedGenerator<int> {
    // std::cout << "[test] before co_yield " << std::this_thread::get_id() << std::endl;
    co_yield 42;
    // std::cout << "[test] after co_yield " << std::this_thread::get_id() << std::endl;
  };
  auto g = gen();

  std::optional<int> x = 0;
  std::optional<int> y = 0;

  {
    std::latch latch(2);

    // std::cout << "before jthreads" << std::endl;
    std::jthread t1([g, &latch, &x] mutable {
      latch.arrive_and_wait();
      // стартовали в 1 момент
      x = g.next();
    });
    std::jthread t2([g, &latch, &y] mutable {
      latch.arrive_and_wait();
      // стартовали в 1 момент
      y = g.next();
    });
  }

  EXPECT_TRUE(x == 42 || y == 42);
  EXPECT_TRUE(x == std::nullopt || y == std::nullopt);
}

TEST(GeneratorParallelTest, TwoThreadsOneValueOneException) {
  struct Error {};

  auto gen = [] -> ct::SharedGenerator<int> {
    co_yield 42;
    throw Error{};
  };
  auto g = gen();

  std::optional<int> x = 0;
  std::optional<int> y = 0;
  std::atomic<std::size_t> caught = 0;

  {
    std::latch latch(2);

    std::jthread t1([g, &latch, &x, &caught] mutable {
      latch.arrive_and_wait();
      try {
        x = g.next();
      } catch (Error) {
        ++caught;
      }
    });
    std::jthread t2([g, &latch, &y, &caught] mutable {
      latch.arrive_and_wait();
      try {
        y = g.next();
      } catch (Error) {
        ++caught;
      }
    });
  }

  EXPECT_TRUE(x == 42 || y == 42);
  EXPECT_TRUE(x == 0 || y == 0);
  EXPECT_EQ(caught, 1);
}

TEST(GeneratorParallelTest, ThreeThreadsThreeValues) {
  auto gen = [] -> ct::SharedGenerator<int> {
    co_yield 2;
    co_yield 3;
    co_yield 5;
  };
  auto g = gen();

  std::optional<int> x = 0;
  std::optional<int> y = 0;
  std::optional<int> z = 0;

  {
    std::latch latch(3);

    std::jthread t1([g, &latch, &x] mutable {
      latch.arrive_and_wait();
      x = g.next();
    });
    std::jthread t2([g, &latch, &y] mutable {
      latch.arrive_and_wait();
      y = g.next();
    });
    std::jthread t3([g, &latch, &z] mutable {
      latch.arrive_and_wait();
      z = g.next();
    });
  }

  ASSERT_TRUE(x);
  ASSERT_TRUE(y);
  ASSERT_TRUE(z);
  EXPECT_EQ(*x + *y + *z, 2 + 3 + 5);
}

TEST(GeneratorParallelTest, ManyThreadsOneValue) {
  constexpr std::size_t N_THREADS = 20; // 20

  auto gen = [] -> ct::SharedGenerator<int> {
    co_yield 42;
  };
  auto g = gen();

  std::vector<std::optional<int>> results(N_THREADS);

  {
    std::latch latch(N_THREADS);
    std::vector<std::jthread> threads;

    for (std::size_t i = 0; i < N_THREADS; ++i) {
      // std::cout << i << std::endl;
      threads.emplace_back([g, i, &results, &latch] mutable {
        latch.arrive_and_wait();
        results[i] = g.next();
      });
    }
  }

  std::optional<std::size_t> value_idx;
  for (std::size_t idx = 0; const auto& opt : results) {
    if (opt) {
      EXPECT_EQ(*opt, 42) << "with idx = " << idx;
      EXPECT_TRUE(!value_idx) << "duplicated value; with idx = " << idx << ", prev idx = " << *value_idx;
      value_idx = idx;
    }
    ++idx;
  }
  EXPECT_TRUE(value_idx) << "no thread got the value";
}

TEST(GeneratorParallelTest, ManyThreadsThreeValues) {
  constexpr std::size_t N_THREADS = 20;

  auto gen = [] -> ct::SharedGenerator<int> {
    co_yield 2;
    co_yield 3;
    co_yield 5;
  };
  auto g = gen();

  std::vector<std::optional<int>> results(N_THREADS);

  {
    std::latch latch(N_THREADS);
    std::vector<std::jthread> threads;

    for (std::size_t i = 0; i < N_THREADS; ++i) {
      threads.emplace_back([g, i, &results, &latch] mutable {
        latch.arrive_and_wait();
        results[i] = g.next();
      });
    }
  }

  std::set<int> expected_values = {2, 3, 5};
  for (std::size_t idx = 0; const auto& opt : results) {
    if (opt) {
      EXPECT_EQ(expected_values.erase(*opt), 1) << "with idx = " << idx;
    }
    ++idx;
  }
  EXPECT_TRUE(expected_values.empty());
}

TEST(GeneratorParallelTest, ManyThreadsManyValues) {
  constexpr std::size_t N_VALUES = 1'000'000;
  constexpr std::size_t N_THREADS = 20;

  auto gen = [] -> ct::SharedGenerator<int> {
    for (int i = 1; i <= static_cast<int>(N_VALUES); ++i) {
      co_yield i;
    }
  };
  auto g = gen();

  std::vector<int> all_values;
  all_values.reserve(N_VALUES);

  {
    std::mutex all_values_mutex;

    std::vector<std::jthread> threads;
    threads.reserve(N_THREADS);

    for (std::size_t i = 0; i < N_THREADS; ++i) {
      threads.emplace_back([g, &all_values_mutex, &all_values] mutable {
        std::vector<int> local_values;
        local_values.reserve(N_VALUES / N_THREADS * 2);

        while (auto x = g.next()) {
          local_values.push_back(*x);
        }

        std::lock_guard lg(all_values_mutex);
        all_values.insert(all_values.end(), local_values.begin(), local_values.end());
      });
    }
  }

  std::multiset all_values_set(all_values.begin(), all_values.end());
  for (int i = 1; i <= static_cast<int>(N_VALUES); ++i) {
    ASSERT_EQ(all_values_set.count(i), 1) << "with i = " << i;
  }
}

} // namespace ct_test
