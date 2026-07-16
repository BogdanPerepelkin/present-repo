#include "shared-generator.h"

#include <gtest/gtest.h>

#include <optional>
#include <utility>
#include <vector>

// 180
// 227

namespace ct_test {

TEST(GeneratorUniqueTest, Simple) {
  auto gen = [] -> ct::SharedGenerator<int> {
    co_yield 10;
    // std::cout << "[cor] after 10" << std::endl;
    // в генераторе уже 10, я вернул генератор, корутина в суспенде
    co_yield 20;
    // std::cout << "[cor] after 20" << std::endl;
    // co_return -> return_void
    // final_suspend() = suspend_always() -> объект жив и можно вызвать .done()
  };
  // std::cout << "[main] kfore gen" << std::endl;
  auto g = gen(); // здесь нет _currentValue
  // std::cout << "[main] after gen" << std::endl;
  // я принял генератоор, корутина в суспенде

  EXPECT_EQ(g.next(), 10); // _currentValue = 10
  EXPECT_EQ(g.next(), 20); // _current_Value = 20
  // g.next();
  EXPECT_EQ(g.next(), std::nullopt);
}

TEST(GeneratorUniqueTest, TryReallyHardAfterExhaustion) {
  auto gen = [] -> ct::SharedGenerator<int> {
    co_yield 10;
  };
  auto g = gen();

  EXPECT_EQ(g.next(), 10);
  EXPECT_EQ(g.next(), std::nullopt);
  EXPECT_EQ(g.next(), std::nullopt);
  EXPECT_EQ(g.next(), std::nullopt);
}

TEST(GeneratorUniqueTest, SideEffects) {
  int stage = 0;

  auto gen = [&stage] -> ct::SharedGenerator<int> {
    ++stage;
    co_yield 10;
    ++stage;
    co_yield 20;
    ++stage;
  };
  auto g = gen();

  EXPECT_EQ(stage, 0);

  while (auto x = g.next()) {
    EXPECT_EQ(*x, stage * 10);
  }

  EXPECT_EQ(stage, 3);
}

TEST(GeneratorUniqueTest, Empty) {
  auto gen = [] -> ct::SharedGenerator<int> {
    co_return;
  };
  auto g = gen();

  EXPECT_EQ(g.next(), std::nullopt);
}

TEST(GeneratorUniqueTest, ExceptionAfterYields) {
  struct Error {};

  auto gen = [] -> ct::SharedGenerator<int> {
    co_yield 10;
    co_yield 20;
    throw Error{};
  };
  auto g = gen();

  EXPECT_EQ(g.next(), 10);
  EXPECT_EQ(g.next(), 20);
  EXPECT_THROW(g.next(), Error);
  EXPECT_EQ(g.next(), std::nullopt);
}

TEST(GeneratorUniqueTest, ExceptionBeforeYields) {
  struct Error {};

  auto gen = [] -> ct::SharedGenerator<int> {
    throw Error{};
    co_return;
  };
  auto g = gen();

  EXPECT_THROW(g.next(), Error);
  EXPECT_EQ(g.next(), std::nullopt);
}

TEST(GeneratorUniqueTest, Arguments) {
  auto gen = [](std::vector<int> values, int y, int& z) -> ct::SharedGenerator<int> {
    for (int x : values) {
      co_yield x;
      co_yield y;
      co_yield z;
      y += 2;
      z += 2;
    }
  };

  int y = 42;
  int z = 123;

  auto g = gen({10, 20}, y, z);

  EXPECT_EQ(g.next(), 10);
  EXPECT_EQ(g.next(), 42);
  EXPECT_EQ(g.next(), 123);
  EXPECT_EQ(g.next(), 20);
  EXPECT_EQ(g.next(), 44);
  EXPECT_EQ(g.next(), 125);
  EXPECT_EQ(g.next(), std::nullopt);

  EXPECT_EQ(y, 42);
  EXPECT_EQ(z, 127);
}

TEST(GeneratorUniqueTest, Nested) {
  auto gen1 = [] -> ct::SharedGenerator<int> {
    // std::cout << "2.1" << std::endl;
    co_yield 20;
    // std::cout << "2.2" << std::endl;
    co_yield 30;
    // std::cout << "2.3" << std::endl;
  };

  auto gen2 = [](ct::SharedGenerator<int> nested) -> ct::SharedGenerator<int> {
    // std::cout << "1.0" << std::endl;
    co_yield 10;
    // std::cout << "1.1" << std::endl;
    while (auto x = nested.next()) {
      // std::cout << "1.2" << std::endl;
      co_yield *x;
      // std::cout << "1.3" << std::endl;
    }
    // std::cout << "1.4" << std::endl;
    co_yield 40;
    // std::cout << "1.5" << std::endl;
  };

  // [my]
  // gen2(gen1());

  auto g = gen2(gen1());

  // std::cout << "[test] 1" << std::endl;
  EXPECT_EQ(g.next(), 10);
  // std::cout << "[test] 2" << std::endl;
  EXPECT_EQ(g.next(), 20);
  // std::cout << "[test] 3" << std::endl;
  // EXPECT_EQ(g.next(), 30);
  // EXPECT_EQ(g.next(), 40);
  // EXPECT_EQ(g.next(), std::nullopt);
  // std::cout << "end" << std::endl;
}

TEST(GeneratorUniqueTest, DefaultConstructor) {
  // std::cout << "[main] before create g" << std::endl;
  ct::SharedGenerator<int> g;
  // std::cout << "[main] after create g" << std::endl;
  // g.next();
  EXPECT_EQ(g.next(), std::nullopt);
}

TEST(GeneratorUniqueTest, MoveConstructor) {
  auto gen = [] -> ct::SharedGenerator<int> {
    co_yield 10;
    co_yield 20;
    co_yield 30;
  };

  auto g1 = gen();
  EXPECT_EQ(g1.next(), 10);

  auto g2 = std::move(g1);
  EXPECT_EQ(g1.next(), std::nullopt);
  EXPECT_EQ(g2.next(), 20);

  auto g3 = std::move(g2);
  EXPECT_EQ(g1.next(), std::nullopt);
  EXPECT_EQ(g2.next(), std::nullopt);
  EXPECT_EQ(g3.next(), 30);

  EXPECT_EQ(g3.next(), std::nullopt);
}

TEST(GeneratorUniqueTest, MoveConstructorFromEmpty) {
  ct::SharedGenerator<int> g1;
  ct::SharedGenerator<int> g2 = std::move(g1);

  EXPECT_EQ(g1.next(), std::nullopt);
  EXPECT_EQ(g2.next(), std::nullopt);
}

TEST(GeneratorUniqueTest, MoveAssignment) {
  auto gen = [](int x) -> ct::SharedGenerator<int> {
    while (true) {
      co_yield x;
      x += 10;
    }
  };

  auto g1 = gen(10); // here
  EXPECT_EQ(g1.next(), 10);


  ct::SharedGenerator<int> g2;
  g2 = std::move(g1);
  EXPECT_EQ(g1.next(), std::nullopt);
  EXPECT_EQ(g2.next(), 20);

  g1 = gen(15);
  EXPECT_EQ(g1.next(), 15);

  g2 = std::move(g1);
  EXPECT_EQ(g1.next(), std::nullopt);
  EXPECT_EQ(g2.next(), 25);
}

TEST(GeneratorUniqueTest, MoveAssignmentEmptyFromEmpty) {
  ct::SharedGenerator<int> g1;
  ct::SharedGenerator<int> g2;

  g2 = std::move(g1);
  EXPECT_EQ(g1.next(), std::nullopt);
  EXPECT_EQ(g2.next(), std::nullopt);
}

TEST(GeneratorUniqueTest, MoveAssignmentEmptyFromNonEmpty) {
  auto gen = [] -> ct::SharedGenerator<int> {
    co_yield 10;
    co_yield 20;
  };

  ct::SharedGenerator<int> g1;
  ct::SharedGenerator<int> g2 = gen();

  g1 = std::move(g2);
  EXPECT_EQ(g1.next(), 10);
  EXPECT_EQ(g2.next(), std::nullopt);
}

TEST(GeneratorUniqueTest, MoveAssignmentNonEmptyFromEmpty) {
  auto gen = [] -> ct::SharedGenerator<int> { // leak
    co_yield 10;
    co_yield 20;
  };

  ct::SharedGenerator<int> g1 = gen(); // here
  ct::SharedGenerator<int> g2;

  g1 = std::move(g2);
  EXPECT_EQ(g1.next(), std::nullopt);
  EXPECT_EQ(g2.next(), std::nullopt);
}

} // namespace ct_test
